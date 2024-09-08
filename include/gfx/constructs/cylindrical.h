#ifndef GFX_CONSTRUCTS_CYLINDRICAL_H
#define GFX_CONSTRUCTS_CYLINDRICAL_H

#include "gfx/vertex_buffer.h"
#include "gfx/constructs/construct_data.h"

void construct_smooth_cylindrical(struct vertex_buffer *vertices, struct index_buffer *indices, struct construct_data *data, 
                                  float base_radius, float top_radius, float height, size_t sector, size_t stack);

void construct_flat_cylindrical(struct vertex_buffer *vertices, struct index_buffer *indices, struct construct_data *data, 
                                float base_radius, float top_radius, float height, size_t sector, size_t stack);

#endif 