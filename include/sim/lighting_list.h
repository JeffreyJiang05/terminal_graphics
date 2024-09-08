#ifndef SIM_LIGHTING_LIST_H
#define SIM_LIGHTING_LIST_H

#include "gfx/constructs/constructs.h"
#include "sim/construct_list.h"

struct light
{
    struct vertex_buffer vertices;
    struct index_buffer indices;
    struct construct_data data;
    struct model_data_t model_func;
};

struct light_list
{
    struct light *lights;
    size_t size;
    size_t capacity;
};

void light_list_init(struct light_list *list);

void light_list_free(struct light_list *list);

void light_list_add(struct light_list *list, struct light *light);

int light_list_remove(struct light_list *list, size_t n);

struct light *light_list_get(struct light_list *list, size_t n);

#endif 