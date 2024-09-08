#ifndef GFX_CONSTRUCTS_PRISM_H
#define GFX_CONSTRUCTS_PRISM_H

#include "gfx/vertex_buffer.h"
#include "gfx/constructs/construct_data.h"

void construct_smooth_prism(struct vertex_buffer *vertices, struct index_buffer *indices, struct construct_data *data, float apothem, float height, size_t sides);

void construct_flat_prism(struct vertex_buffer *vertices, struct index_buffer *indices, struct construct_data *data, float apothem, float height, size_t sides);

#endif