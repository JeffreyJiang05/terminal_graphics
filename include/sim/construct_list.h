#ifndef SIM_CONSTRUCT_LIST_H
#define SIM_CONSTRUCT_LIST_H

#include "gfx/constructs/constructs.h"
#include "gfx/lighting/materials.h"

struct model_data_t
{
    float pos_x;
    float pos_y;
    float pos_z;

    float scale;

    float rot_x;
    float rot_y;
    float rot_z;
    float rot_speed;
};

typedef size_t material_id_t;

struct construct
{
    struct vertex_buffer vertices;
    struct index_buffer indices;
    struct construct_data data;
    struct model_data_t model_func;
    material_id_t material_id;
};

struct construct_list
{
    struct construct *constructs;
    size_t size;
    size_t capacity;
};

void construct_list_init(struct construct_list *list);

void construct_list_free(struct construct_list *list);

void construct_list_add(struct construct_list *list, struct construct *construct);

int construct_list_remove(struct construct_list *list, size_t n);

struct construct *construct_list_get(struct construct_list *list, size_t n);

#endif 