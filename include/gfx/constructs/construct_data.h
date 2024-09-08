#ifndef GFX_CONSTRUCT_DATA_H
#define GFX_CONSTRUCT_DATA_H

#include "math/linalg.h"
#include "gfx/image.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

struct construct_datum
{
    struct fvec4 normal;
    struct fvec4 texture;    
};

#ifndef CONSTRUCT_DATA
    #define CONSTRUCT_DATA(nx,ny,nz,u,v) (struct construct_datum) { .normal = { .buffer = { nx, ny, nz, 1 } }, .texture = { .buffer = { u, v, 0, 0 } } }
#endif

// implement binary_interpolate 
void bl_interpolate_construct_datum(void *a, void *b, float t, void *ret);

// implement ternary_interpolate
void tl_interpolate_construct_datum(void *a, void *b, void *c, struct fvec4 t, void *ret);

int backface_cull_construct_datum(struct fvec4 v0, void *d0, struct fvec4 v1, void *d1, struct fvec4 v2, void *d2, void *context);

struct construct_data
{
    size_t capacity;
    size_t size;
    struct construct_datum *buffer;
};

#define DEFAULT_CONSTRUCT_DATA_SIZE 8

void construct_data_init(struct construct_data *data);

void construct_data_init_capacity(struct construct_data *data, size_t capacity);

void construct_data_init_values(struct construct_data *data, size_t n, ...);

void construct_data_free(struct construct_data *data);

void construct_data_add(struct construct_data *data, struct construct_datum datum);

void construct_data_add_many(struct construct_data *data, size_t n, ...);

struct construct_datum *construct_data_get(struct construct_data *data, size_t n);

struct construct_datum construct_data_get_value(struct construct_data *data, size_t n);

struct construct_datum *construct_data_begin(struct construct_data *data);

struct construct_datum *construct_data_end(struct construct_data *data);

void construct_data_shrink_to_fit(struct construct_data *data);

void construct_data_reserve(struct construct_data *data, size_t n);

#endif