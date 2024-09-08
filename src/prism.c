#include "gfx/constructs/prism.h"

#include "gfx/constructs/cylindrical.h"

void construct_smooth_prism(struct vertex_buffer *vertices, struct index_buffer *indices, struct construct_data *data, float apothem, float height, size_t sides)
{
    construct_smooth_cylindrical(vertices, indices, data, apothem, apothem, height, sides, 1);
}

void construct_flat_prism(struct vertex_buffer *vertices, struct index_buffer *indices, struct construct_data *data, float apothem, float height, size_t sides)
{
    construct_flat_cylindrical(vertices, indices, data, apothem, apothem, height, sides, 1);
}
