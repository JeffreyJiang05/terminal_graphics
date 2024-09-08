#include "gfx/constructs/construct_data.h"

#include "stdarg.h"

void bl_interpolate_construct_datum(void *a, void *b, float t, void *ret)
{
    struct construct_datum *da = (struct construct_datum*) a, *db = (struct construct_datum*) b;
    struct construct_datum *d_ret = (struct construct_datum*) ret;

    fvec4_linear_interpolate(&da->normal, &db->normal, t, &d_ret->normal);
    fvec4_linear_interpolate(&da->texture, &db->texture, t, &d_ret->texture);
}

void tl_interpolate_construct_datum(void *a, void *b, void *c, struct fvec4 t, void *ret)
{
    struct construct_datum *da = (struct construct_datum*) a, *db = (struct construct_datum*) b, *dc = (struct construct_datum*) c;
    struct construct_datum *d_ret = (struct construct_datum*) ret;

    fvec4_ternary_linear_interpolate(&da->normal, &db->normal, &dc->normal, t, &d_ret->normal);
    fvec4_ternary_linear_interpolate(&da->texture, &db->texture, &dc->texture, t, &d_ret->texture);
}

int backface_cull_construct_datum(struct fvec4 v0, void *d0, struct fvec4 v1, void *d1, struct fvec4 v2, void *d2, void *context)
{
    float d = (v1.components.y - v0.components.y) * (v2.components.x - v1.components.x) - (v2.components.y - v1.components.y) * (v1.components.x - v0.components.x);
    if (d >= 0) return 0;
    return 1;

    // struct fvec4* camera_view = (struct fvec4*) context;
    // struct fvec4 diff = fvec4_sub(v0, *camera_view);
    // struct fvec4 normal = fvec4_cross(fvec4_sub(v2, v0), fvec4_sub(v1, v0));
    // if (fvec4_dot(diff, normal) > 0) return 0;
    // return 1;
}

void construct_data_init(struct construct_data *data)
{
    data->buffer = (struct construct_datum*) malloc(DEFAULT_CONSTRUCT_DATA_SIZE * sizeof(struct construct_datum));
    data->capacity = DEFAULT_CONSTRUCT_DATA_SIZE;
    data->size = 0;
}

void construct_data_init_capacity(struct construct_data *data, size_t capacity)
{
    data->buffer = (struct construct_datum*) malloc(capacity * sizeof(struct construct_datum));
    data->capacity = capacity;
    data->size = 0;
}

void construct_data_init_values(struct construct_data *data, size_t n, ...)
{
    data->buffer = (struct construct_datum*) malloc(n * sizeof(struct construct_datum));
    data->capacity = n;
    data->size = n;

    va_list va;
    va_start(va, n);
    for (size_t i = 0; i < n; ++i)
    {
        data->buffer[i] = va_arg(va, struct construct_datum);
    }
    va_end(va);
}

void construct_data_free(struct construct_data *data)
{
    free(data->buffer);
    data->buffer = NULL;
    data->capacity = data->size = 0;
}

static void construct_data_resize(struct construct_data *data)
{
    data->capacity = data->capacity * 2;
    data->buffer = realloc(data->buffer, data->capacity * sizeof(struct construct_datum));
}

void construct_data_add(struct construct_data *data, struct construct_datum datum)
{
    if (data->size == data->capacity) construct_data_resize(data);
    data->buffer[data->size++] = datum;
}

void construct_data_add_many(struct construct_data *data, size_t n, ...)
{
    va_list va;
    va_start(va, n);
    for (size_t i = 0; i < n; ++i)
    {
        construct_data_add(data, va_arg(va, struct construct_datum));
    }
    va_end(va);
}

struct construct_datum *construct_data_get(struct construct_data *data, size_t n)
{
    if (n >= data->size) return NULL;
    return data->buffer + n;
}

struct construct_datum construct_data_get_value(struct construct_data *data, size_t n)
{
    return data->buffer[n];
}

struct construct_datum *construct_data_begin(struct construct_data *data)
{
    return data->buffer;
}

struct construct_datum *construct_data_end(struct construct_data *data)
{
    return data->buffer + data->size;
}

void construct_data_shrink_to_fit(struct construct_data *data)
{
    data->buffer = realloc(data->buffer, data->size * sizeof(struct construct_datum));
    data->capacity = data->size;
}

void construct_data_reserve(struct construct_data *data, size_t n)
{
    if (n > data->size) return;
    data->buffer = realloc(data->buffer, n * sizeof(struct construct_datum));
    data->capacity = n;
}