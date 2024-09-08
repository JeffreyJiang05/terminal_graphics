#include "sim/construct_list.h"

#include <stdlib.h>

#define DEFAULT_CONSTRUCT_LIST_CAPACITY 8

void construct_list_init(struct construct_list *list)
{
    list->capacity = DEFAULT_CONSTRUCT_LIST_CAPACITY;
    list->size = 0;
    list->constructs = malloc(list->capacity * sizeof(struct construct));
}

void construct_list_free(struct construct_list *list)
{
    list->capacity = list->size = 0;
    free(list->constructs);
}

static void resize_construct_list(struct construct_list *list)
{
    list->capacity = 2 * list->capacity;
    list->constructs = realloc(list->constructs, list->capacity * sizeof(struct construct));
}

void construct_list_add(struct construct_list *list, struct construct *construct)
{
    if (list->size == list->capacity) resize_construct_list(list);
    list->constructs[list->size++] = *construct;
}

int construct_list_remove(struct construct_list *list, size_t n)
{
    if (n >= list->size) return 0;
    for (size_t i = n; i < list->size - 1; ++i) list->constructs[i] = list->constructs[i + 1];
    --list->size;
    return 1;
}

struct construct *construct_list_get(struct construct_list *list, size_t n)
{
    if (n >= list->size) return NULL;
    return &list->constructs[n];
}