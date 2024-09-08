#include "gfx/vertex_buffer.h"

#include <stdarg.h>
#include <string.h>

void vertex_buffer_init(struct vertex_buffer *buffer)
{
    buffer->vertices = malloc(GFX_BUFFER_INITIAL_CAPACITY * sizeof(struct fvec4));
    buffer->capacity = GFX_BUFFER_INITIAL_CAPACITY;
    buffer->size = 0;
}

void vertex_buffer_init_with_vecs(struct vertex_buffer *buffer, size_t n, ...)
{
    buffer->vertices = malloc(n * sizeof(struct fvec4));
    buffer->capacity = n;
    buffer->size = n;

    va_list va;   
    va_start(va, n);
    for (size_t i = 0; i < n; ++i)
    {
        buffer->vertices[i] = va_arg(va, struct fvec4);
    }
    va_end(va);
}

void vertex_buffer_copy(struct vertex_buffer *buffer, size_t n, void *vertices)
{
    buffer->vertices = malloc(n * sizeof(struct fvec4));
    buffer->capacity = n;
    buffer->size = n;

    memcpy(buffer->vertices, vertices, n * sizeof(struct fvec4));
}

void vertex_buffer_free(struct vertex_buffer *buffer)
{
    free(buffer->vertices);
    buffer->vertices = NULL;
    buffer->size = 0;
    buffer->capacity = 0;
}

struct fvec4 vertex_buffer_get(struct vertex_buffer *buffer, size_t n)
{
    return buffer->vertices[n];
}

struct fvec4 *vertex_buffer_get_reference(struct vertex_buffer *buffer, size_t n)
{
    if (n >= buffer->size) return NULL;
    return buffer->vertices + n;
}

static void _resize_vertex_buffer(struct vertex_buffer *buffer)
{
    buffer->vertices = realloc(buffer->vertices, (buffer->capacity * 2 + 1) * sizeof(struct fvec4));
    buffer->capacity = buffer->capacity * 2 + 1;
}

void vertex_buffer_add(struct vertex_buffer *buffer, struct fvec4 vec)
{
    if (buffer->size == buffer->capacity) _resize_vertex_buffer(buffer);
    buffer->vertices[buffer->size++] = vec;
}

void vertex_buffer_add_many(struct vertex_buffer *buffer, size_t n, ...)
{
    va_list va;
    va_start(va, n);
    for (size_t i = 0; i < n; ++i)
    {
        vertex_buffer_add(buffer, va_arg(va, struct fvec4));
    }
    va_end(va);
}

struct fvec4 *vertex_buffer_begin(struct vertex_buffer *buffer)
{
    return vertex_buffer_get_reference(buffer, 0);
}
 
struct fvec4 *vertex_buffer_end(struct vertex_buffer *buffer)
{
    return buffer->vertices + buffer->size;
}

void vertex_buffer_shrink_to_fit(struct vertex_buffer *buffer)
{
    buffer->vertices = realloc(buffer->vertices, buffer->size * sizeof(struct fvec4));
    buffer->capacity = buffer->size;
}

void vertex_buffer_reserve(struct vertex_buffer *buffer, size_t n)
{
    if (n < buffer->size) return;
    buffer->vertices = realloc(buffer->vertices, n * sizeof(struct fvec4));
    buffer->capacity = n;
}

void vertex_buffer_apply_transformation(struct vertex_buffer *buffer, struct fmat4x4 matrix, struct vertex_buffer* target)
{
    struct fvec4 *begin = vertex_buffer_begin(buffer), *end = vertex_buffer_end(buffer);
    while (begin != end)
    {
        vertex_buffer_add(target, fmat4x4_matvec_mult(matrix, *begin));
        ++begin;
    }
}


// INDEX BUFFER

void index_buffer_init(struct index_buffer *buffer)
{
    buffer->indices = malloc(GFX_BUFFER_INITIAL_CAPACITY * sizeof(struct triangle_vertex_indices));
    buffer->capacity = GFX_BUFFER_INITIAL_CAPACITY;
    buffer->size = 0;
}

void index_buffer_init_with_indices(struct index_buffer *buffer, size_t triangle_count, ...)
{
    buffer->indices = malloc(triangle_count * sizeof(struct triangle_vertex_indices));
    buffer->capacity = triangle_count;
    buffer->size = triangle_count;

    va_list va;
    va_start(va, triangle_count);
    for (size_t i = 0; i < triangle_count; ++i)
    {
        buffer->indices[i] = va_arg(va, struct triangle_vertex_indices);
    }
    va_end(va);
}

void index_buffer_copy(struct index_buffer *buffer, size_t n, void* indices)
{
    buffer->indices = malloc(n * sizeof(struct triangle_vertex_indices));
    buffer->size = n;
    buffer->capacity = n;

    memcpy(buffer->indices, indices, n * sizeof(struct triangle_vertex_indices));
}

void index_buffer_free(struct index_buffer* buffer)
{
    free(buffer->indices);
    buffer->capacity = 0;
    buffer->indices = 0;
    buffer->indices = NULL;
}

struct triangle_vertex_indices *index_buffer_get(struct index_buffer *buffer, size_t n)
{
    if (n >= buffer->size) return NULL;
    return buffer->indices + n;
}

static void _resize_index_buffer(struct index_buffer *buffer)
{
    buffer->indices = realloc(buffer->indices, (buffer->capacity * 2 + 1) * sizeof(struct triangle_vertex_indices));
    buffer->capacity = buffer->capacity * 2 + 1;
}

void index_buffer_add(struct index_buffer* buffer, struct triangle_vertex_indices vertices)
{
    if (buffer->size == buffer->capacity) _resize_index_buffer(buffer);
    buffer->indices[buffer->size++] = vertices;
}

void index_buffer_add_many(struct index_buffer* buffer, size_t n, ...)
{
    va_list va;
    va_start(va, n);
    for (size_t i = 0; i < n; ++i)
    {
        index_buffer_add(buffer, va_arg(va, struct triangle_vertex_indices));
    }
    va_end(va);
}

struct triangle_vertex_indices *index_buffer_begin(struct index_buffer *buffer)
{
    return index_buffer_get(buffer, 0);
}

struct triangle_vertex_indices *index_buffer_end(struct index_buffer *buffer)
{
    return buffer->indices + buffer->size;
}

void index_buffer_shrink_to_fit(struct index_buffer *buffer)
{
    buffer->indices = realloc(buffer->indices, buffer->size * sizeof(struct triangle_vertex_indices));
    buffer->capacity = buffer->size;
}

void index_buffer_reserve(struct index_buffer *buffer, size_t n)
{
    if (n < buffer->size) return;
    buffer->indices = realloc(buffer->indices, n * sizeof(struct triangle_vertex_indices));
    buffer->capacity = n;
}