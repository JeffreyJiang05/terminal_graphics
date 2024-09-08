#include "gfx/constructs/pyramid.h"

#include "gfx/constructs/cylindrical.h"

void construct_smooth_pyramid(struct vertex_buffer *vertices, struct index_buffer *indices, struct construct_data *data, float base_radius, float height, size_t sides, size_t stack)
{
    construct_smooth_cylindrical(vertices, indices, data, base_radius, 0, height, sides, stack);
}

void construct_flat_pyramid(struct vertex_buffer *vertices, struct index_buffer *indices, struct construct_data *data, float base_radius, float height, size_t sides, size_t stack)
{
    construct_flat_cylindrical(vertices, indices, data, base_radius, 0, height, sides, stack);
}