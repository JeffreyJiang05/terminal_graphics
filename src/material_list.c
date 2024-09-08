#include "sim/material_list.h"

#include <stdlib.h>

#define DEFAULT_MATERIAL_LIST_CAPACITY 8

void material_list_init(struct material_list *list)
{
    list->capacity = DEFAULT_MATERIAL_LIST_CAPACITY;
    list->size = 0;
    list->materials = malloc(list->capacity * sizeof(struct material));
}

void material_list_free(struct material_list *list)
{
    list->capacity = list->size = 0;
    free(list->materials);
}

static void material_list_resize(struct material_list *list)
{
    list->capacity = 2 * list->capacity;
    list->materials = realloc(list->materials, list->capacity * sizeof(struct material));
}

void material_list_add(struct material_list *list, struct material *material)
{
    if (list->size == list->capacity) material_list_resize(list);
    list->materials[list->size++] = *material;
}

struct material *material_list_get(struct material_list *list, size_t n)
{
    if (n >= list->size) return NULL;
    return &list->materials[n];
}