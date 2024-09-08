#ifndef GFX_CONSTRUCTS_SPHERE_H
#define GFX_CONSTRUCTS_SPHERE_H 

#include "gfx/vertex_buffer.h"
#include "gfx/constructs/construct_data.h"

void construct_sphere(struct vertex_buffer *vertices, struct index_buffer *indices, struct construct_data *data, size_t stack_num, size_t sector_num);

#endif