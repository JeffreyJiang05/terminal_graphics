#ifndef UI_FRAME_QUEUE_H
#define UI_FRAME_QUEUE_H

#ifndef _POSIX_C_SOURCE
    #define _POSIX_C_SOURCE 202405L
#endif 

#include <pthread.h>
#include <signal.h>

#include "gfx/image.h"

// failed cause the main bottleneck is printing

void frame_queue_push(struct image *image);

int frame_queue_try_pop(struct image *image);

size_t frame_queue_size();

int frame_queue_empty();

void frame_queue_clear();

typedef void(*frame_context_func_t)(void*);

struct drawframe_callbacks_t
{
    frame_context_func_t callback;
    void *input;
};

void set_drawframe_callbacks(struct drawframe_callbacks_t callback);

void launch_drawframe_thread();

void terminate_drawframe_thread();

#endif 