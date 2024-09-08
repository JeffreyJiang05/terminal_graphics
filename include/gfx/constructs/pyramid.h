#ifndef GFX_CONSTRUCTS_PYRAMID_H
#define GFX_CONSTRUCTS_PYRAMID_H

#include "gfx/vertex_buffer.h"
#include "gfx/constructs/construct_data.h"

void construct_smooth_pyramid(struct vertex_buffer *vertices, struct index_buffer *indices, struct construct_data *data, float base_radius, float height, size_t sides, size_t stack);

void construct_flat_pyramid(struct vertex_buffer *vertices, struct index_buffer *indices, struct construct_data *data, float base_radius, float height, size_t sides, size_t stack);

#endif