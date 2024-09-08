#ifndef SIM_MATERIAL_LIST_H
#define SIM_MATERIAL_LIST_H

#include <stddef.h>

#include "gfx/lighting/materials.h"

struct material_list
{
    struct material *materials;
    size_t size;
    size_t capacity;
};

void material_list_init(struct material_list *list);

void material_list_free(struct material_list *list);

void material_list_add(struct material_list *list, struct material *material);

struct material *material_list_get(struct material_list *list, size_t n);

#endif