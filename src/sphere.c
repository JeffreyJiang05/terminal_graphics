#include "gfx/constructs/sphere.h"

#include <math.h>

void construct_sphere(struct vertex_buffer *vertices, struct index_buffer *indices, struct construct_data *data, size_t stack_num, size_t sector_num)
{
    size_t vbuf_size = ((stack_num + 1) * (sector_num + 1)) << 3;
    size_t ibuf_size = (stack_num * (sector_num - 2)) << 1;

    vertex_buffer_reserve(vertices, vbuf_size);
    construct_data_reserve(data, vbuf_size);
    index_buffer_reserve(indices, ibuf_size);

    float x, y, z, xy;
    float nx, ny, nz;
    float s, t;

    float d_sector = 2 * M_PI / sector_num;
    float d_stack = M_PI / stack_num;
    float sector_angle, stack_angle;

    for (size_t i = 0; i <= stack_num; ++i)
    {
        stack_angle = M_PI / 2 - i * d_stack;
        xy = cos(stack_angle);
        z = sin(stack_angle);

        for (size_t j = 0; j <= sector_num; ++j)
        {
            sector_angle = j * d_sector;
            x = xy * cos(sector_angle);
            y = xy * sin(sector_angle);

            nx = x;
            ny = y;
            nz = z;

            s = (float) j / sector_num;
            t = (float) i / stack_num;

            vertex_buffer_add(vertices, (struct fvec4) { .buffer = { x, y, z, 1 } });
            construct_data_add(data, (struct construct_datum) { 
                .normal = (struct fvec4) { .buffer = { nx, ny, nz, 1 } },
                .texture = (struct fvec4) { .buffer = { s, t, 0, 0 } } 
            });
        }
    }

    size_t k1, k2;

    for (size_t i = 0; i < stack_num; ++i)
    {
        k1 = i * (sector_num + 1);
        k2 = k1 + sector_num + 1;
        for (size_t j = 0; j < sector_num; ++j, ++k1, ++k2)
        {
            if (i != 0) index_buffer_add(indices, (struct triangle_vertex_indices) { k1, k2, k1 + 1 });
            if (i != stack_num - 1) index_buffer_add(indices, (struct triangle_vertex_indices) { k1 + 1, k2, k2 + 1 });
        }
    }
}
