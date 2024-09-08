#include "sim/lighting_list.h"

#include <stdlib.h>

#define DEFAULT_LIGHT_LIST_CAPACITY 8

void light_list_init(struct light_list *list)
{
    list->capacity = DEFAULT_LIGHT_LIST_CAPACITY;
    list->size = 0;
    list->lights = malloc(list->capacity * sizeof(struct light));
}

void light_list_free(struct light_list *list)
{
    list->capacity = list->size = 0;
    free(list->lights);
}

static void resize_light_list(struct light_list *list)
{
    list->capacity = 2 * list->capacity;
    list->lights = realloc(list->lights, list->capacity * sizeof(struct construct));
}

void light_list_add(struct light_list *list, struct light *light)
{
    if (list->size == list->capacity) resize_light_list(list);
    list->lights[list->size++] = *light;
}

int light_list_remove(struct light_list *list, size_t n)
{
    if (n >= list->size) return 0;
    for (size_t i = n; i < list->size - 1; ++i) list->lights[i] = list->lights[i + 1];
    --list->size;
    return 1;
}

struct light *light_list_get(struct light_list *list, size_t n)
{
    if (n >= list->size) return NULL;
    return &list->lights[n];
}