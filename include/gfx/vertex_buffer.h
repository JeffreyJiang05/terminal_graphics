#ifndef GFX_VERTEX_BUFFER
#define GFX_VERTEX_BUFFER

#include <stdlib.h>

#include "math/linalg.h"

#define GFX_BUFFER_INITIAL_CAPACITY 8

struct vertex_buffer
{
    size_t capacity;
    size_t size;
    struct fvec4 *vertices;
};

void vertex_buffer_init(struct vertex_buffer *buffer);

void vertex_buffer_init_with_vecs(struct vertex_buffer *buffer, size_t n, ...);

void vertex_buffer_copy(struct vertex_buffer *buffer, size_t n, void *vertices);

void vertex_buffer_free(struct vertex_buffer *buffer);

struct fvec4 vertex_buffer_get(struct vertex_buffer *buffer, size_t n);

struct fvec4 *vertex_buffer_get_reference(struct vertex_buffer *buffer, size_t n);

void vertex_buffer_add(struct vertex_buffer *buffer, struct fvec4 vec);

void vertex_buffer_add_many(struct vertex_buffer *buffer, size_t n, ...);

struct fvec4 *vertex_buffer_begin(struct vertex_buffer *buffer);

struct fvec4 *vertex_buffer_end(struct vertex_buffer *buffer);

void vertex_buffer_shrink_to_fit(struct vertex_buffer *buffer);

void vertex_buffer_reserve(struct vertex_buffer *buffer, size_t n);

void vertex_buffer_apply_transformation(struct vertex_buffer *buffer, struct fmat4x4 matrix, struct vertex_buffer* target);

struct triangle_vertex_indices
{
    size_t idx0;
    size_t idx1;
    size_t idx2;
};

struct index_buffer
{
    size_t capacity;
    size_t size;
    struct triangle_vertex_indices *indices;
};

void index_buffer_init(struct index_buffer *buffer);

void index_buffer_init_with_indices(struct index_buffer *buffer, size_t triangle_count, ...);

void index_buffer_copy(struct index_buffer *buffer, size_t n, void* indices);

void index_buffer_free(struct index_buffer* buffer);

struct triangle_vertex_indices *index_buffer_get(struct index_buffer *buffer, size_t n);

void index_buffer_add(struct index_buffer* buffer, struct triangle_vertex_indices vertices);

void index_buffer_add_many(struct index_buffer* buffer, size_t n, ...);

struct triangle_vertex_indices *index_buffer_begin(struct index_buffer *buffer);

struct triangle_vertex_indices *index_buffer_end(struct index_buffer *buffer);

void index_buffer_shrink_to_fit(struct index_buffer *buffer);

void index_buffer_reserve(struct index_buffer *buffer, size_t n);

#endif