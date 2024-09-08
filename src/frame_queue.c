#include "ui/frame_queue.h"

#include <stdatomic.h>
#include <sched.h>

#include "ui/terminal.h"
#include "ui/term_im_conv.h"

struct image_node
{
    struct image image;
    struct image_node *prev;
    struct image_node *next;
};

struct frame_queue_t
{
    struct image_node *front;
    struct image_node *end;
    size_t size;
    pthread_mutex_t mutex;
};

static struct frame_queue_t g_frame_queue = { NULL, NULL, 0, PTHREAD_MUTEX_INITIALIZER };

void frame_queue_push(struct image *image)
{
    struct image_node *new_front = malloc(sizeof(struct image_node));
    new_front->image = *image;
    new_front->prev = NULL;

    pthread_mutex_lock(&g_frame_queue.mutex);

    new_front->next = g_frame_queue.front;
    if (g_frame_queue.front) g_frame_queue.front->prev = new_front;

    g_frame_queue.front = new_front;

    if (!g_frame_queue.end) g_frame_queue.end = new_front;

    ++g_frame_queue.size;
    pthread_mutex_unlock(&g_frame_queue.mutex);
}

int frame_queue_try_pop(struct image *image)
{
    pthread_mutex_lock(&g_frame_queue.mutex);
    if (g_frame_queue.end)
    {
        struct image_node *frame = g_frame_queue.end;
        g_frame_queue.end = frame->prev;

        if (g_frame_queue.end) g_frame_queue.end->next = NULL;

        if (g_frame_queue.front == frame) g_frame_queue.front = NULL;
        --g_frame_queue.size;

        *image = frame->image;
        free(frame);
        pthread_mutex_unlock(&g_frame_queue.mutex);
        return 1;
    }

    pthread_mutex_unlock(&g_frame_queue.mutex);
    return 0;
}

size_t frame_queue_size()
{
    size_t res;
    pthread_mutex_lock(&g_frame_queue.mutex);
    res = g_frame_queue.size;
    pthread_mutex_unlock(&g_frame_queue.mutex);
    return res;
}

int frame_queue_empty()
{
    pthread_mutex_lock(&g_frame_queue.mutex);
    if (g_frame_queue.front)
    {
        pthread_mutex_unlock(&g_frame_queue.mutex);
        return 1;
    }
    pthread_mutex_unlock(&g_frame_queue.mutex);
    return 0;
}

void frame_queue_clear()
{
    struct image_node *iter, *frame;
    pthread_mutex_lock(&g_frame_queue.mutex);
    iter = frame = g_frame_queue.front;
    while (iter)
    {   
        frame = iter;
        iter = iter->next;
        free(frame);
    }
    g_frame_queue.front = g_frame_queue.end = NULL;
    pthread_mutex_unlock(&g_frame_queue.mutex);
}

static struct drawframe_callbacks_t g_dataframe_callback; 
static pthread_t g_drawframe_thread;
static atomic_bool g_run_drawframe_thread = 0;

static void* drawframe(void* data)
{
    while (!atomic_load(&g_run_drawframe_thread)) sched_yield();

    struct image img;
    struct terminal_image term_img;
    while (atomic_load(&g_run_drawframe_thread))
    {
        if (frame_queue_try_pop(&img))
        {
            wprintf(L"SUCCESS - %ld\n", frame_queue_size());
            terminal_goto_origin();

            term_img = conv_to_terminal_image(&img);
            print_terminal_image(&term_img);
            free_terminal_image(&term_img);
            image_free(&img);

            if (g_dataframe_callback.callback) g_dataframe_callback.callback(g_dataframe_callback.input);
        }
    }

    return NULL;
}

void set_drawframe_callbacks(struct drawframe_callbacks_t callback)
{
    g_dataframe_callback = callback;
}

void launch_drawframe_thread()
{
    atomic_store(&g_run_drawframe_thread, 1);
    pthread_create(&g_drawframe_thread, NULL, drawframe, NULL);
}

void terminate_drawframe_thread()
{
    atomic_store(&g_run_drawframe_thread, 0);
    if (!atomic_load(&g_run_drawframe_thread)) pthread_join(g_drawframe_thread, NULL);
}

