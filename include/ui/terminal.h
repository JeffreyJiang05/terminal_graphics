#ifndef UI_TERMINAL_H
#define UI_TERMINAL_H

#ifndef _POSIX_C_SOURCE
    #define _POSIX_C_SOURCE 202405L
#endif 

#include <termios.h>
#include <signal.h>
#include <pthread.h>
#include <signal.h>
#include <sys/ioctl.h>

#include "ui/inputs.h"

typedef unsigned char terminal_setting_t;

extern const terminal_setting_t TERMINAL_DEFAULT;
extern const terminal_setting_t TERMINAL_MAX_SCREEN_SIZE;
extern const terminal_setting_t TERMINAL_REGISTER_KEY_EVENTS;
extern const terminal_setting_t TERMINAL_REGISTER_MOUSE_EVENTS;

typedef void(*on_key_event_function_t)(enum key_event_id key_event);
typedef void(*on_mouse_event_function_t)(enum mouse_event_id mouse_event, size_t x, size_t y);
typedef void(*on_resize_event_function_t)();

struct terminal
{
    terminal_setting_t settings;
    struct { size_t width; size_t height; } size;
    on_resize_event_function_t on_resize_event;

    struct key_event key_event;
    pthread_mutex_t key_mutex;
    on_key_event_function_t on_key_event;
    struct mouse_event mouse_event;
    pthread_mutex_t mouse_mutex;
    on_mouse_event_function_t on_mouse_event;
    pthread_t input_thread;

    struct termios startup_stdin_config;
    struct termios current_stdin_config;

    struct sigaction sigint_action;
    struct sigaction sigwinch_action;

    size_t frame_count;
};

void terminal_init(terminal_setting_t settings, ...);

void terminal_goto_origin();
void terminal_goto(size_t x, size_t y);

void terminal_clear();

void terminal_enable_alternate_buffer();
void terminal_disable_alternate_buffer();

void terminal_hide_cursor();
void terminal_show_cursor();

void terminal_track_mouse();
void terminal_untrack_mouse();

void terminal_process_key_input();
void terminal_process_mouse_input();
void terminal_process_inputs();

size_t terminal_width();
size_t terminal_height();
size_t terminal_frame_count();

void terminal_increment_frame();

#endif 