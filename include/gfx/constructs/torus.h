#ifndef GFX_CONSTRUCTS_TORUS_H
#define GFX_CONSTRUCTS_TORUS_H

#include "gfx/vertex_buffer.h"
#include "gfx/constructs/construct_data.h"

void construct_smooth_torus(struct vertex_buffer *vertices, struct index_buffer *indices, struct construct_data *data, float major_radius, float minor_radius, size_t sectors, size_t sides);

void construct_flat_torus(struct vertex_buffer *vertices, struct index_buffer *indices, struct construct_data *data, float major_radius, float minor_radius, size_t sectors, size_t sides);

#endif 