#include "gfx/constructs/torus.h"

#include <math.h>

void construct_smooth_torus(struct vertex_buffer *vertices, struct index_buffer *indices, struct construct_data *data, float major_radius, float minor_radius, size_t sectors, size_t sides)
{
    float x, y, z, xy;
    float nx, ny, nz;
    float len_inv = 1 / minor_radius;
    float s, t;

    float sector_step = 2 * M_PI / sectors;
    float side_step = 2 * M_PI / sides;
    float sector_angle, side_angle;

    for (size_t i = 0; i <= sides; ++i)
    {
        side_angle = M_PI - i * side_step;
        xy = minor_radius * cosf(side_angle);
        z = minor_radius * sinf(side_angle);

        for (size_t j = 0; j <= sectors; ++j)
        {
            sector_angle = j * sector_step;

            x = xy * cosf(sector_angle);
            y = xy * sinf(sector_angle);

            nx = x * len_inv;
            ny = y * len_inv;
            nz = z * len_inv;

            x += major_radius * cosf(sector_angle);
            y += major_radius * sinf(sector_angle);

            s = (float) j / sectors;
            t = (float) i / sides;

            vertex_buffer_add(vertices, (struct fvec4) { .buffer = { x, y, z, 1 } });
            construct_data_add(data, (struct construct_datum) { 
                .normal = { .buffer = { nx, ny, nz, 1 } },
                .texture = { .buffer = { s, t, 0, 0 } }
            });
        }
    }

    size_t k1, k2;
    for (size_t i = 0; i < sides; ++i)
    {
        k1 = i * (sectors + 1);
        k2 = k1 + sectors + 1;

        for (size_t j = 0; j < sectors; ++j, ++k1, ++k2)
        {
            index_buffer_add(indices, (struct triangle_vertex_indices) { k1, k2, k1 + 1 });
            index_buffer_add(indices, (struct triangle_vertex_indices) { k1 + 1, k2, k2 + 1 });
        }
    }
}

static void compute_normal_for_face(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float *dx, float *dy, float *dz)
{
    float nx, ny, nz;

    float ex0 = x1 - x0, ex1 = x2 - x0;
    float ey0 = y1 - y0, ey1 = y2 - y0;
    float ez0 = z1 - z0, ez1 = z2 - z0;

    nx = ey0 * ez1 - ez0 * ey1;
    ny = ez0 * ex1 - ex0 * ez1;
    nz = ex0 * ey1 - ey0 * ex1;

    float inv_len = 1 / sqrtf(nx * nx + ny * ny + nz * nz);
    *dx = nx * inv_len;
    *dy = ny * inv_len;
    *dz = nz * inv_len;
}

void construct_flat_torus(struct vertex_buffer *vertices, struct index_buffer *indices, struct construct_data *data, float major_radius, float minor_radius, size_t sectors, size_t sides)
{
    size_t i, j;
    float x, y, xy, z;
    float s, t;

    float sector_step = 2 * M_PI / sectors;
    float side_step = 2 * M_PI / sides;
    float sector_angle, side_angle;

    typedef struct construct_data temporary_buffer;
    temporary_buffer tmp_buf;
    construct_data_init(&tmp_buf);

    for (i = 0; i <= sides; ++i)
    {
        side_angle = M_PI - i * side_step;
        xy = major_radius + minor_radius * cosf(side_angle);
        z = minor_radius * sinf(side_angle);

        for (j = 0; j <= sectors; ++j)
        {
            sector_angle = j * sector_step;

            x = xy * cosf(sector_angle);
            y = xy * sinf(sector_angle);
            
            s = (float) j / sectors;
            t = (float) i / sides;

            construct_data_add(data, (struct construct_datum) {
                .normal = { .buffer = { x, y, z, 1 } },
                .texture = { .buffer = { s, t, 0, 0 } }
            });
        }
    }

    struct construct_datum v1, v2, v3, v4;
    size_t vi1, vi2;
    size_t index = 0;
    float nx, ny, nz;

    for (i = 0; i < sides; ++i)
    {
        vi1 = i * (sectors + 1);
        vi2 = (i + 1) * (sectors + 1);

        for (j = 0; j < sectors; ++j, ++vi1, ++vi2)
        {
            v1 = construct_data_get_value(data, vi1);
            v2 = construct_data_get_value(data, vi2);
            v3 = construct_data_get_value(data, vi1 + 1);
            v4 = construct_data_get_value(data, vi2 + 1);

            vertex_buffer_add(vertices, (struct fvec4) { 
                .buffer = { v1.normal.components.x, v1.normal.components.y, v1.normal.components.z, 1 } 
            });
            vertex_buffer_add(vertices, (struct fvec4) { 
                .buffer = { v2.normal.components.x, v2.normal.components.y, v2.normal.components.z, 1 } 
            });
            vertex_buffer_add(vertices, (struct fvec4) { 
                .buffer = { v3.normal.components.x, v3.normal.components.y, v3.normal.components.z, 1 } 
            });
            vertex_buffer_add(vertices, (struct fvec4) { 
                .buffer = { v4.normal.components.x, v4.normal.components.y, v4.normal.components.z, 1 } 
            });

            compute_normal_for_face(
                v1.normal.components.x, v1.normal.components.y, v1.normal.components.z,
                v2.normal.components.x, v2.normal.components.y, v2.normal.components.z,
                v3.normal.components.x, v3.normal.components.y, v3.normal.components.z,
                &nx, &ny, &nz
            );

            construct_data_add_many(data, 4,
                (struct construct_datum) {
                    .normal = { .buffer = { nx, ny, nz, 1 } },
                    .texture = v1.texture
                },
                (struct construct_datum) {
                    .normal = { .buffer = { nx, ny, nz, 1 } },
                    .texture = v2.texture
                },
                (struct construct_datum) {
                    .normal = { .buffer = { nx, ny, nz, 1 } },
                    .texture = v3.texture
                },
                (struct construct_datum) {
                    .normal = { .buffer = { nx, ny, nz, 1 } },
                    .texture = v4.texture
                }
            );

            index_buffer_add(indices, (struct triangle_vertex_indices) { index, index + 1, index + 2 });
            index_buffer_add(indices, (struct triangle_vertex_indices) { index + 2, index + 1, index + 3 });

            index += 4;
        }
    }

    construct_data_free(&tmp_buf);
}
