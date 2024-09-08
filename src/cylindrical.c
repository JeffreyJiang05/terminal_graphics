#include "gfx/constructs/cylindrical.h"

#include <math.h>

static float *gen_unit_circle(size_t sector)
{
    float *data = (float*) malloc((sector + 1) * 2 * sizeof(float));

    float sector_step = 2 * M_PI / sector;
    float sector_angle;
    for (size_t i = 0; i <= sector; ++i)
    {
        sector_angle = i * sector_step;
        data[2 * i + 0] = cosf(sector_angle);
        data[2 * i + 1] = sinf(sector_angle);
    }
    return data;    
}

static float *gen_smooth_side_normals(size_t sector, float base, float top, float height)
{
    float *data = (float*) malloc((sector + 1) * 3 * sizeof(float));

    float sector_step = 2 * M_PI / sector;
    float sector_angle;

    float z_angle = atan2(base - top, height);
    float x0 = cos(z_angle), y0 = 0, z0 = sin(z_angle);

    for (size_t i = 0; i <= sector; ++i)
    {
        sector_angle = i * sector_step;
        data[3 * i + 0] = cosf(sector_angle) * x0 - sinf(sector_angle) * y0;
        data[3 * i + 1] = sinf(sector_angle) * x0 + cosf(sector_angle) * y0;
        data[3 * i + 2] = z0;
    }
    return data;
}

void construct_smooth_cylindrical(struct vertex_buffer *vertices, struct index_buffer *indices, struct construct_data *data, 
                                  float base_radius, float top_radius, float height, size_t sector, size_t stack)
{
    float x, y, z;
    float s, t;
    float radius;

    float *unit_circle = gen_unit_circle(sector); // 2 floats per vertex

    // sides
    float *side_normals = gen_smooth_side_normals(sector, base_radius, top_radius, height); // 3 float per vertex
    for (size_t i = 0; i <= stack; ++i)
    {
        z = height * -0.5f + (float) i / stack * height;
        radius = base_radius + (float) i / stack * (top_radius - base_radius);
        t = 1.0f - (float) i / stack;

        for (size_t j = 0; j <= sector; ++j)
        {
            x = unit_circle[2 * j + 0] * radius;
            y = unit_circle[2 * j + 1] * radius;
            vertex_buffer_add(vertices, (struct fvec4) { .buffer = { x, y, z, 1 } });

            s = (float) j / sector;
            construct_data_add(data, (struct construct_datum) { 
                .normal = { .buffer = { side_normals[3 * j + 0], side_normals[3 * j + 1], side_normals[3 * j + 2], 1 } },
                .texture = { .buffer = { s, t, 0, 0 } } 
            });
        }
    }

    // bottom base
    z = height * -0.5f;
    vertex_buffer_add(vertices, (struct fvec4) { .buffer = { 0, 0, z, 1 } });
    construct_data_add(data, (struct construct_datum) { .normal = { .buffer = { 0, 0, -1, 1 } }, .texture = { .buffer = { 0.5f, 0.5f, 0, 0 } } });
 
    for (size_t i = 0; i < sector; ++i)
    {
        x = unit_circle[2 * i + 0] * base_radius;
        y = unit_circle[2 * i + 1] * base_radius;
        vertex_buffer_add(vertices, (struct fvec4) { .buffer = { x, y, z, 1 } });

        s = unit_circle[2 * i + 0] * -0.5f + 0.5f;
        t = unit_circle[2 * i + 1] * -0.5f + 0.5f;
        construct_data_add(data, (struct construct_datum) {
            .normal = { .buffer = { 0, 0, -1, 0 } },
            .texture = { .buffer = { s, t, 0, 0 } }
        });
    } 

    // top base
    z = height * 0.5f;
    vertex_buffer_add(vertices, (struct fvec4) { .buffer = { 0, 0, z, 1 } });
    construct_data_add(data, (struct construct_datum) { .normal = { .buffer = { 0, 0, 1, 1 } }, .texture = { .buffer = { 0.5f, 0.5f, 0, 0 } } });

    for (size_t i = 0; i < sector; ++i)
    {
        x = unit_circle[2 * i + 0] * top_radius;
        y = unit_circle[2 * i + 1] * top_radius;
        vertex_buffer_add(vertices, (struct fvec4) { .buffer = { x, y, z, 1 } });

        s = unit_circle[2 * i + 0] * 0.5f + 0.5f;
        t = unit_circle[2 * i + 1] * -0.5f + 0.5f;
        construct_data_add(data, (struct construct_datum) {
            .normal = { .buffer = { 0, 0, -1, 0 } },
            .texture = { .buffer = { s, t, 0, 0 } }
        });
    } 

    size_t base_vertex_index = (stack + 1) * (sector + 1);
    size_t top_vertex_index = (stack + 2) * (sector + 1);

    size_t k1, k2;
    for (size_t i = 0; i < stack; ++i)
    {
        k1 = i * (sector + 1);
        k2 = k1 + sector + 1;

        for (size_t j = 0; j < sector; ++j, ++k1, ++k2)
        {
            index_buffer_add(indices, (struct triangle_vertex_indices) { k1, k1 + 1, k2 });
            index_buffer_add(indices, (struct triangle_vertex_indices) { k2, k1 + 1, k2 + 1 });
        }
    }

    for (size_t i = 0, k = base_vertex_index + 1; i < sector; ++i, ++k)
    {
        if (i < sector - 1) index_buffer_add(indices, (struct triangle_vertex_indices) { base_vertex_index, k + 1, k });
        else index_buffer_add(indices, (struct triangle_vertex_indices) { base_vertex_index, base_vertex_index + 1, k });
    }

    for (size_t i = 0, k = top_vertex_index + 1; i < sector; ++i, ++k)
    {
        if (i < sector - 1) index_buffer_add(indices, (struct triangle_vertex_indices) { top_vertex_index, k, k + 1 });
        else index_buffer_add(indices, (struct triangle_vertex_indices) { top_vertex_index, k, top_vertex_index + 1 });
    }
 
    free(unit_circle);
    free(side_normals);
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

void construct_flat_cylindrical(struct vertex_buffer *vertices, struct index_buffer *indices, struct construct_data *data, 
                                float base_radius, float top_radius, float height, size_t sector, size_t stack)
{
    float *unit_circle = gen_unit_circle(sector);

    size_t i, j, k;
    float x, y, z, s, t, radius;

    typedef struct construct_data temporary_buffer;
    temporary_buffer tmp_buf;
    construct_data_init(&tmp_buf);

    for (i = 0; i <= stack; ++i)
    {
        z = height * -0.5f + (float) i / stack * height;
        radius = base_radius + (float) i / stack * (top_radius - base_radius);
        t = 1.0f - (float) i / stack;

        for (j = 0; j <= sector; ++j)
        {
            x = unit_circle[2 * j + 0] * radius;
            y = unit_circle[2 * j + 1] * radius;
            s = (float) j / sector;

            construct_data_add(&tmp_buf, (struct construct_datum) { 
                .normal = { .buffer = { x, y, z, 1 } },
                .texture = { .buffer = { s, t, 0, 0 } } 
            });
        }
    }

    size_t vi1, vi2;
    float nx, ny, nz;
    struct construct_datum v0, v1, v2, v3;
    struct fvec4 norm;

    // sides
    for (i = 0; i < stack; ++i)
    {
        vi1 = i * (sector + 1);
        vi2 = (i + 1) * (sector + 1);

        for (j = 0; j < sector; ++j, ++vi1, ++vi2)
        {
            v0 = construct_data_get_value(&tmp_buf, vi1);
            v1 = construct_data_get_value(&tmp_buf, vi2);
            v2 = construct_data_get_value(&tmp_buf, vi1 + 1);
            v3 = construct_data_get_value(&tmp_buf, vi2 + 1);

            compute_normal_for_face(
                v0.normal.components.x, v0.normal.components.y, v0.normal.components.z,
                v2.normal.components.x, v2.normal.components.y, v2.normal.components.z,
                v1.normal.components.x, v1.normal.components.y, v1.normal.components.z,
                &nx, &ny, &nz    
            );
            norm = (struct fvec4) { .buffer = { nx, ny, nz, 1 } };

            vertex_buffer_add(vertices, v0.normal);
            vertex_buffer_add(vertices, v1.normal);
            vertex_buffer_add(vertices, v2.normal);
            vertex_buffer_add(vertices, v3.normal);

            construct_data_add(data, (struct construct_datum) { .normal = norm, .texture = v0.texture });
            construct_data_add(data, (struct construct_datum) { .normal = norm, .texture = v1.texture });
            construct_data_add(data, (struct construct_datum) { .normal = norm, .texture = v2.texture });
            construct_data_add(data, (struct construct_datum) { .normal = norm, .texture = v3.texture });
        }
    }

    // bottom
    z = height * -0.5f;
    vertex_buffer_add(vertices, (struct fvec4) { .buffer = { 0, 0, z, 1 } });
    construct_data_add(data, (struct construct_datum) { .normal = { .buffer = { 0, 0, -1, 1 } }, .texture = { .buffer = { 0.5f, 0.5f, 0, 0 } } });

    for (i = 0; i < sector; ++i)
    {
        x = unit_circle[2 * i + 0] * base_radius;
        y = unit_circle[2 * i + 1] * base_radius;
        s = x * -0.5f + 0.5f;
        t = y * -0.5f + 0.5f;

        vertex_buffer_add(vertices, (struct fvec4) { .buffer = { x, y, z, 1 } });
        construct_data_add(data, (struct construct_datum) { .normal = { .buffer = { 0, 0, -1, 1 } }, .texture = { .buffer = { s, t, 0, 0 } } });
    }

    // top
    z = height * 0.5f;
    vertex_buffer_add(vertices, (struct fvec4) { .buffer = { 0, 0, z, 1 } });
    construct_data_add(data, (struct construct_datum) { .normal = { .buffer = { 0, 0, 1, 1 } }, .texture = { .buffer = { 0.5f, 0.5f, 0, 0 } } });

    for (int i = 0; i < sector; ++i)
    {
        x = unit_circle[2 * i + 0] * top_radius;
        y = unit_circle[2 * i + 1] * top_radius;
        s = x * 0.5f + 0.5f;
        t = y * -0.5f + 0.5f;

        vertex_buffer_add(vertices, (struct fvec4) { .buffer = { x, y, z, 1 } });
        construct_data_add(data, (struct construct_datum) { .normal = { .buffer = { 0, 0, -1, 1 } }, .texture = { .buffer = { s, t, 0, 0 } } });
    }

    // indices
    size_t index = 0;
    for (i = 0; i < stack; ++i)
    {
        for (j = 0; j < sector; ++j, index += 4)
        {
            index_buffer_add(indices, (struct triangle_vertex_indices) { index, index + 2, index + 1 });
            index_buffer_add(indices, (struct triangle_vertex_indices) { index + 1, index + 2, index + 3 });
        }
    }

    size_t base_vertex_index = 4 * stack * sector;
    for (i = 0, k = base_vertex_index + 1; i < sector; ++i, ++k)
    {
        if (i < sector - 1) index_buffer_add(indices, (struct triangle_vertex_indices) { base_vertex_index, k + 1, k });
        else index_buffer_add(indices, (struct triangle_vertex_indices) { base_vertex_index, base_vertex_index + 1, k });
    }

    size_t top_vertex_index = 4 * stack * sector + sector + 1;
    for (i = 0, k = top_vertex_index + 1; i < sector; ++i, ++k)
    {
        if (i < sector - 1) index_buffer_add(indices, (struct triangle_vertex_indices) { top_vertex_index, k, k + 1 });
        else index_buffer_add(indices, (struct triangle_vertex_indices) { top_vertex_index, k, top_vertex_index + 1 });
    }

    construct_data_free(&tmp_buf);
    free(unit_circle);
}