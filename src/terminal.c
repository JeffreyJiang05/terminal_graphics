#include "ui/terminal.h"

#include <stdarg.h>
#include <unistd.h>
#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>
#include <sched.h>
#include <locale.h>

typedef void(*generic_function_t)();

static const wchar_t *ENABLE_MOUSE_TRACKING = L"\033[?1003h\033[?1015h\033[?1006h";
static const wchar_t *DISABLE_MOUSE_TRACKING = L"\033[?1000l";
static const wchar_t *GOTO_ORIGIN = L"\033[1;1H";
static const wchar_t *CLEAR_TERMINAL = L"\033[2J";
static const wchar_t *HIDE_CURSOR = L"\033[?25l";
static const wchar_t *SHOW_CURSOR = L"\033[?25h";
static const wchar_t *ENABLE_ALT_BUFFER = L"\033[?1049h";
static const wchar_t *DISABLE_ALT_BUFFER = L"\033[?1049l";

const terminal_setting_t TERMINAL_DEFAULT = 0x0;
const terminal_setting_t TERMINAL_MAX_SCREEN_SIZE = 0x1 << 0;
const terminal_setting_t TERMINAL_REGISTER_KEY_EVENTS = 0x1 << 1;
const terminal_setting_t TERMINAL_REGISTER_MOUSE_EVENTS = 0x1 << 2;

static struct terminal terminal;
static atomic_bool run_stdin;

static void on_sigwinch_signal(int n)
{
    struct winsize window_size;
    
    ioctl(STDIN_FILENO, TIOCGWINSZ, &window_size);
    terminal.size.width = window_size.ws_col;
    terminal.size.height = window_size.ws_row;

    if (terminal.on_resize_event) 
        terminal.on_resize_event();
}

static void end_terminal(int n)
{
    terminal_show_cursor();
    terminal_untrack_mouse();
    terminal_disable_alternate_buffer();

    terminal_clear();
    terminal_goto_origin();

    tcsetattr(STDIN_FILENO, TCSANOW, &terminal.startup_stdin_config);
    exit(0);
}

static void terminal_setup_callbacks(generic_function_t *callbacks)
{
    terminal.on_resize_event = (on_resize_event_function_t) callbacks[0];
    terminal.on_key_event = (on_key_event_function_t) callbacks[1];
    terminal.on_mouse_event = (on_mouse_event_function_t) callbacks[2];
}

static void *parse_key_stdin_thread(void* param)
{
    // ensure run_stdin is set to true
    while (!atomic_load(&run_stdin)) sched_yield();
    while (atomic_load(&run_stdin))
    {
        struct key_event result;
        if (parse_key_input(&result))
        {
            pthread_mutex_lock(&terminal.key_mutex);
            terminal.key_event = result;
            pthread_mutex_unlock(&terminal.key_mutex);
        }
    }
    return NULL;
}

static void *parse_mouse_stdin_thread(void *param)
{
    // ensure run_stdin is set to true
    while (!atomic_load(&run_stdin)) sched_yield();
    while (atomic_load(&run_stdin))
    {
        struct mouse_event result;
        if (parse_mouse_input(&result))
        {
            pthread_mutex_lock(&terminal.mouse_mutex);
            terminal.mouse_event = result;
            pthread_mutex_unlock(&terminal.mouse_mutex);
        }
    }
    return NULL;
}

static void *parse_key_mouse_stdin_thread(void *param)
{
    // ensure run_stdin is set to true
    while (!atomic_load(&run_stdin)) sched_yield();
    while (atomic_load(&run_stdin))
    {
        struct key_event k_result;
        struct mouse_event m_result;
        
        int res = parse_key_mouse_input(&k_result, &m_result);
        if (res == INPUT_KEY_SUCCESS)
        {
            pthread_mutex_lock(&terminal.key_mutex);
            terminal.key_event = k_result;
            pthread_mutex_unlock(&terminal.key_mutex);
        }
        else if (res == INPUT_MOUSE_SUCCESS)
        {
            pthread_mutex_lock(&terminal.mouse_mutex);
            terminal.mouse_event = m_result;
            pthread_mutex_unlock(&terminal.mouse_mutex);
        }
    }
    return NULL;
}

static void terminal_launch_stdin_thread()
{
    terminal.current_stdin_config.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &terminal.current_stdin_config);

    atomic_store(&run_stdin, 1);
    if ((terminal.settings & (TERMINAL_REGISTER_KEY_EVENTS | TERMINAL_REGISTER_MOUSE_EVENTS)) == (TERMINAL_REGISTER_KEY_EVENTS | TERMINAL_REGISTER_MOUSE_EVENTS))
    {
        pthread_create(&terminal.input_thread, NULL, parse_key_mouse_stdin_thread, NULL);
    }
    else if (terminal.settings & TERMINAL_REGISTER_KEY_EVENTS)
    {
        pthread_create(&terminal.input_thread, NULL, parse_key_stdin_thread, NULL);
    }
    else if (terminal.settings & TERMINAL_REGISTER_MOUSE_EVENTS)
    {
        pthread_create(&terminal.input_thread, NULL, parse_mouse_stdin_thread, NULL);
    }
}

void terminal_init(terminal_setting_t settings, ...)
{
    setlocale(LC_CTYPE, "");

    terminal.settings = settings;
    struct winsize window_size;
    
    ioctl(STDIN_FILENO, TIOCGWINSZ, &window_size);
    terminal.size.width = window_size.ws_col;
    terminal.size.height = window_size.ws_row;

    tcgetattr(STDIN_FILENO, &terminal.startup_stdin_config);
    terminal.current_stdin_config = terminal.startup_stdin_config;

    terminal.key_mutex = terminal.mouse_mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    terminal.key_event = (struct key_event) { .is_key_pressed = 0, .id = KEY_NONE };
    terminal.mouse_event = (struct mouse_event) { .id = MOUSE_NONE, .x = 0, .y = 0 };

    generic_function_t callbacks[3] = { NULL };
    va_list va;
    va_start(va, settings);
    if (settings & TERMINAL_MAX_SCREEN_SIZE) callbacks[0] = va_arg(va, generic_function_t);
    if (settings & TERMINAL_REGISTER_KEY_EVENTS) callbacks[1] = va_arg(va, generic_function_t);
    if (settings & TERMINAL_REGISTER_MOUSE_EVENTS) callbacks[2] = va_arg(va, generic_function_t);
    va_end(va);
    terminal_setup_callbacks(callbacks);

    if (settings & TERMINAL_REGISTER_MOUSE_EVENTS)
    {
        terminal_track_mouse();
    }

    if (settings & (TERMINAL_REGISTER_KEY_EVENTS | TERMINAL_REGISTER_MOUSE_EVENTS))
    {
        terminal_launch_stdin_thread();
    }

    terminal.sigint_action.sa_handler = end_terminal;
    sigaction(SIGINT, &terminal.sigint_action, NULL);

    if (settings & TERMINAL_MAX_SCREEN_SIZE)
    {
        terminal.sigwinch_action.sa_handler = on_sigwinch_signal;
        sigaction(SIGWINCH, &terminal.sigwinch_action, NULL);
        if (terminal.on_resize_event) terminal.on_resize_event();
    }

    terminal.frame_count = 0;
}

void terminal_goto_origin()
{
    wprintf(GOTO_ORIGIN);
}

void terminal_goto(size_t x, size_t y)
{
    wprintf(L"\033[%lu;%luH", y, x);
}

void terminal_clear()
{
    wprintf(CLEAR_TERMINAL);
}

void terminal_enable_alternate_buffer()
{
    wprintf(ENABLE_ALT_BUFFER);
}
void terminal_disable_alternate_buffer()
{
    wprintf(DISABLE_ALT_BUFFER);
}

void terminal_hide_cursor()
{
    wprintf(HIDE_CURSOR);
}
void terminal_show_cursor()
{
    wprintf(SHOW_CURSOR);
}


void terminal_track_mouse()
{
    wprintf(ENABLE_MOUSE_TRACKING);
}
void terminal_untrack_mouse()
{
    wprintf(DISABLE_MOUSE_TRACKING);
}

void terminal_process_key_input()
{
    if (terminal.settings & TERMINAL_REGISTER_KEY_EVENTS)
    {
        int key_pressed;
        enum mouse_event_id id;

        pthread_mutex_lock(&terminal.key_mutex);
        key_pressed = terminal.key_event.is_key_pressed;
        id = terminal.key_event.id;
        terminal.key_event.is_key_pressed = 0;
        pthread_mutex_unlock(&terminal.key_mutex);

        if (key_pressed && terminal.on_key_event) terminal.on_key_event(id);
    }
}
void terminal_process_mouse_input()
{
    if (terminal.settings & TERMINAL_REGISTER_MOUSE_EVENTS)
    {
        enum mouse_event_id id;
        size_t x, y;

        pthread_mutex_lock(&terminal.mouse_mutex);
        id = terminal.mouse_event.id;
        x = terminal.mouse_event.x;
        y = terminal.mouse_event.y;
        pthread_mutex_unlock(&terminal.mouse_mutex);

        // wprintf(L"%d - %d %d\n", id, x, y);
    
        if (terminal.on_mouse_event) terminal.on_mouse_event(id, x, y);
    }
}
void terminal_process_inputs()
{
    terminal_process_key_input();
    terminal_process_mouse_input();
}

size_t terminal_width()
{
    return terminal.size.width;
}

size_t terminal_height()
{
    return terminal.size.height;
}

size_t terminal_frame_count()
{
    return terminal.frame_count;
}

void terminal_increment_frame()
{
    ++terminal.frame_count;
}