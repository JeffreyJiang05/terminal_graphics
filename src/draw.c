#include "gfx/draw.h"
#include "gfx/utility.h"
#include "math/functions.h"

static void _draw_line_octant_I_pix(struct image *img, long x0, long y0, long x1, long y1, long A, long B, struct pixel pix)
{
    long dist = (A << 1) + B;
    while (x0 <= x1)
    {
        image_draw(img, x0, y0, pix);
        if (dist > 0)
        {
            ++y0;
            dist += (B << 1);
        }
        ++x0;
        dist += (A << 1);
    }
}

static void _draw_line_octant_II_pix(struct image *img, long x0, long y0, long x1, long y1, long A, long B, struct pixel pix)
{
    long dist = (B << 1) + A;
    while (y0 <= y1)
    {
        image_draw(img, x0, y0, pix);
        if (dist < 0)
        {
            ++x0;
            dist += (A << 1);
        }
        ++y0;
        dist += (B << 1);
    }
}

static void _draw_line_octant_VII_pix(struct image *img, long x0, long y0, long x1, long y1, long A, long B, struct pixel pix)
{
    long dist = A - (B << 1);
    while (y0 >= y1)
    {
        image_draw(img, x0, y0, pix);
        if (dist > 0)
        {
            ++x0;
            dist += (A << 1);
        }
        --y0;
        dist -= (B << 1);
    }
}

static void _draw_line_octant_VIII_pix(struct image *img, long x0, long y0, long x1, long y1, long A, long B, struct pixel pix)
{
    long dist = (A << 1) - B;
    while (x0 <= x1)
    {
        image_draw(img, x0, y0, pix);
        if (dist < 0)
        {
            --y0;
            dist -= (B << 1);
        }
        ++x0;
        dist += (A << 1);
    }
}

static void frame_coordinate(struct image *img, struct fvec4 *p)
{
    p->components.x = (img->width >> 1) + p->components.x;
    p->components.y = (img->height >> 1) - p->components.y;
}

void draw_line_solid(struct image *img, struct fvec4 p0, struct fvec4 p1, struct pixel pix)
{
    frame_coordinate(img, &p0);
    frame_coordinate(img, &p1);

    struct fvec4 tmp;
    if (p0.components.x < p1.components.x)
    {
        tmp = p0; p0 = p1; p1 = tmp;
    }
    long A = p1.components.y - p0.components.y;
    long B = p0.components.x - p1.components.x;
    if (A >= 0)
    {
        if (A <= -B) _draw_line_octant_I_pix(img, (long) p0.components.x, (long) p0.components.y, (long) p1.components.x, (long) p1.components.y, A, B, pix);
        else         _draw_line_octant_II_pix(img, (long) p0.components.x, (long) p0.components.y, (long) p1.components.x, (long) p1.components.y, A, B, pix);
    }
    else
    {
        if (A >= B)  _draw_line_octant_VIII_pix(img, (long) p0.components.x, (long) p0.components.y, (long) p1.components.x, (long) p1.components.y, A, B, pix);
        else         _draw_line_octant_VII_pix(img, (long) p0.components.x, (long) p0.components.y, (long) p1.components.x, (long) p1.components.y, A, B, pix);
    }
}

static depth_t interpolate_depth(depth_t a, depth_t b, float t)
{
    return (depth_t) (a * (1 - t)) + (depth_t) (b * t);
}

static depth_t ternary_interpolate_depth(depth_t a, depth_t b, depth_t c, struct fvec4 t)
{
    return (depth_t) (a * t.components.x + b * t.components.y + c * t.components.z);
}

static void _general_draw_line_octant_I(struct image *img, depth_buffer_t zbuffer, long x0, long y0, depth_t z0, long x1, long y1, depth_t z1, long A, long B, 
    void *p0, void *p1, binary_interpolation_func_t interpolate, void *interpolated_data, fragment_shader_t shade)
{
    long dist = (A << 1) + B;
    long begin = x0;
    float t;
    while (x0 <= x1)
    {
        t = (float) (x0 - begin) / (x1 - begin);
        interpolate(p0, p1, t, interpolated_data);
        if (zbuffer) image_depth_draw(img, zbuffer, x0, y0, interpolate_depth(z0, z1, t), shade((struct fvec4) {.buffer={ x0, y0, interpolate_depth(z0, z1, t), 1 }}, interpolated_data));
        else image_draw(img, x0, y0, shade((struct fvec4) {.buffer={ x0, y0, 0, 1 }}, interpolated_data));
        
        if (dist > 0)
        {
            ++y0;
            dist += (B << 1);
        }
        ++x0;
        dist += (A << 1);
    }
}

static void _general_draw_line_octant_II(struct image *img, depth_buffer_t zbuffer, long x0, long y0, depth_t z0, long x1, long y1, depth_t z1, long A, long B, 
    void *p0, void *p1, binary_interpolation_func_t interpolate, void *interpolated_data, fragment_shader_t shade)
{
    long dist = (B << 1) + A;
    long begin = y0;
    float t;
    while (y0 <= y1)
    {
        t = (float) (y0 - begin) / (y1 - begin);
        interpolate(p0, p1, t, interpolated_data);
        if (zbuffer) image_depth_draw(img, zbuffer, x0, y0, interpolate_depth(z0, z1, t), shade((struct fvec4) {.buffer={ x0, y0, interpolate_depth(z0, z1, t), 1 }}, interpolated_data));
        else image_draw(img, x0, y0, shade((struct fvec4) {.buffer={ x0, y0, 0, 1 }}, interpolated_data));
        
        if (dist < 0)
        {
            ++x0;
            dist += (A << 1);
        }
        ++y0;
        dist += (B << 1);
    }
}

static void _general_draw_line_octant_VII(struct image *img, depth_buffer_t zbuffer, long x0, long y0, depth_t z0, long x1, long y1, depth_t z1, long A, long B, 
    void *p0, void *p1, binary_interpolation_func_t interpolate, void *interpolated_data, fragment_shader_t shade)
{
    long dist = A - (B << 1);
    long begin = y0;
    float t;
    while (y0 >= y1)
    {
        t = (float) (y0 - begin) / (y1 - begin);
        interpolate(p0, p1, t, interpolated_data);
        if (zbuffer) image_depth_draw(img, zbuffer, x0, y0, interpolate_depth(z0, z1, t), shade((struct fvec4) {.buffer={ x0, y0, interpolate_depth(z0, z1, t), 1 }}, interpolated_data));
        else image_draw(img, x0, y0, shade((struct fvec4) {.buffer={ x0, y0, 0, 1 }}, interpolated_data));
        
        if (dist > 0)
        {
            ++x0;
            dist += (A << 1);
        }
        --y0;
        dist -= (B << 1);
    }
}

static void _general_draw_line_octant_VIII(struct image *img, depth_buffer_t zbuffer, long x0, long y0, depth_t z0, long x1, long y1, depth_t z1, long A, long B, 
    void *p0, void *p1, binary_interpolation_func_t interpolate, void *interpolated_data, fragment_shader_t shade)
{
    long dist = (A << 1) - B;
    long begin = x0;
    float t;
    while (x0 <= x1)
    {
        t = (float) (x0 - begin) / (x1 - begin);
        interpolate(p0, p1, t, interpolated_data);
        if (zbuffer) image_depth_draw(img, zbuffer, x0, y0, interpolate_depth(z0, z1, t), shade((struct fvec4) {.buffer={ x0, y0, interpolate_depth(z0, z1, t), 1 }}, interpolated_data));
        else image_draw(img, x0, y0, shade((struct fvec4) {.buffer={ x0, y0, interpolate_depth(z0, z1, t), 1 }}, interpolated_data));
        
        if (dist < 0)
        {
            --y0;
            dist -= (B << 1);
        }
        ++x0;
        dist += (A << 1);
    }
}

void draw_line(struct image *img, depth_buffer_t zbuffer, struct fvec4 p0, struct fvec4 p1, void *d0, void *d1, 
               size_t data_size, binary_interpolation_func_t interpolate, fragment_shader_t shade)
{
    frame_coordinate(img, &p0);
    frame_coordinate(img, &p1);

    struct fvec4 tmp; void* tmp_d;
    if (p1.components.x < p0.components.x)
    {
        tmp = p0; p0 = p1; p1 = tmp;
        tmp_d = d0; d0 = d1; d1 = tmp_d;
    }
    long A = p1.components.y - p0.components.y;
    long B = p0.components.x - p1.components.x;
    depth_t depth0 = zbuffer ? compute_depth(p0.components.z) : 0;
    depth_t depth1 = zbuffer ? compute_depth(p1.components.z) : 0;

    void *interpolated_data = malloc(data_size);
    if (A >= 0)
    {
        if (A <= -B) _general_draw_line_octant_I(img, zbuffer, (long) p0.components.x, (long) p0.components.y, depth0, (long) p1.components.x, (long) p1.components.y, depth1, A, B, d0, d1, interpolate, interpolated_data, shade);
        else         _general_draw_line_octant_II(img, zbuffer, (long) p0.components.x, (long) p0.components.y, depth0, (long) p1.components.x, (long) p1.components.y, depth1, A, B, d0, d1, interpolate, interpolated_data, shade);
    }
    else
    {
        if (A >= B)  _general_draw_line_octant_VIII(img, zbuffer, (long) p0.components.x, (long) p0.components.y, depth0, (long) p1.components.x, (long) p1.components.y, depth1, A, B, d0, d1, interpolate, interpolated_data, shade);
        else         _general_draw_line_octant_VII(img, zbuffer, (long) p0.components.x, (long) p0.components.y, depth0, (long) p1.components.x, (long) p1.components.y, depth1, A, B, d0, d1, interpolate, interpolated_data, shade);
    }
    free(interpolated_data);
}

void draw_triangle_wire_frame(struct image *img, depth_buffer_t zbuffer, struct fvec4 p0, void *d0, struct fvec4 p1, void *d1,
                              struct fvec4 p2, void *d2, size_t input_data_size, binary_interpolation_func_t interpolate, fragment_shader_t shade)
{
    p0 = convertToNDC(img, p0);
    p1 = convertToNDC(img, p1);
    p2 = convertToNDC(img, p2);

    draw_line(img, zbuffer, p0, p1, d0, d1, input_data_size, interpolate, shade);
    draw_line(img, zbuffer, p1, p2, d1, d2, input_data_size, interpolate, shade);
    draw_line(img, zbuffer, p2, p0, d2, d0, input_data_size, interpolate, shade);
}

static int is_not_in_frustum(struct fvec4 v0)
{
    return v0.components.x < -1 || v0.components.x > 1
        || v0.components.y < -1 || v0.components.y > 1
        || v0.components.z < 1 || v0.components.z > 2;
}

void draw_wire_frame_mesh(struct image *img, depth_buffer_t zbuffer, struct vertex_buffer *vertices, struct index_buffer *indices, 
    void *data, size_t data_size, binary_interpolation_func_t interpolate, fragment_shader_t shade, cull_func_t cull, void *cull_context)
{
    struct triangle_vertex_indices *begin = index_buffer_begin(indices), *end = index_buffer_end(indices);
    struct fvec4 v0, v1, v2;
    unsigned char *d0, *d1, *d2;
    while (begin != end)
    {
        v0 = vertex_buffer_get(vertices, begin->idx0);
        v1 = vertex_buffer_get(vertices, begin->idx1);
        v2 = vertex_buffer_get(vertices, begin->idx2);

        d0 = (unsigned char*) data + data_size * begin->idx0;
        d1 = (unsigned char*) data + data_size * begin->idx1;
        d2 = (unsigned char*) data + data_size * begin->idx2;

        if (is_not_in_frustum(v0) && is_not_in_frustum(v1) && is_not_in_frustum(v2)) { ++begin; continue; }
        if (cull && cull(v0, d0, v1, d1, v2, d2, cull_context)) { ++begin; continue; }

        draw_triangle_wire_frame(img, zbuffer, v0, d0, v1, d1, v2, d2, data_size, interpolate, shade);
        ++begin;
    }
}

void draw_triangle_filled(struct image* img, depth_buffer_t zbuffer, struct fvec4 p0, void *d0, struct fvec4 p1, void *d1,
                          struct fvec4 p2, void *d2, size_t input_data_size, ternary_interpolation_func_t interpolate, fragment_shader_t shade)
{
    frame_coordinate(img, &p0);
    frame_coordinate(img, &p1);
    frame_coordinate(img, &p2);

    struct fvec4 p_tmp;
    void *d_tmp;
    if (p1.components.y < p0.components.y) { p_tmp = p1; p1 = p0; p0 = p_tmp; d_tmp = d1; d1 = d0; d0 = d_tmp; };
    if (p2.components.y < p0.components.y) { p_tmp = p2; p2 = p0; p0 = p_tmp; d_tmp = d2; d2 = d0; d0 = d_tmp; };
    if (p2.components.y < p1.components.y) { p_tmp = p2; p2 = p1; p1 = p_tmp; d_tmp = d2; d2 = d1; d1 = d_tmp; };

    float m0, m1, m2, xbegin, xend;
    m0 = (p2.components.x - p0.components.x) / ((int) p2.components.y - (int) p0.components.y);
    m1 = (p1.components.x - p0.components.x) / ((int) p1.components.y - (int) p0.components.y);
    m2 = (p2.components.x - p1.components.x) / ((int) p2.components.y - (int) p1.components.y);

    xbegin = xend = p0.components.x;

    float x21, y12, x02, y20, inv_det, xx2, yy2, bary0, bary1;
    x21 = p2.components.x - p1.components.x;
    y12 = p1.components.y - p2.components.y;
    x02 = p0.components.x - p2.components.x;
    y20 = p2.components.y - p0.components.y;
    inv_det = 1 / (y12 * x02 - x21 * y20);

    double da_x, db_x;
    da_x = (p1.components.y - p2.components.y) * inv_det;
    db_x = (p2.components.y - p0.components.y) * inv_det;

    depth_t depth_a = compute_depth(p0.components.z);
    depth_t depth_b = compute_depth(p1.components.z);
    depth_t depth_c = compute_depth(p2.components.z);

    float xmin, xmax;
    void *interpolated_data = malloc(input_data_size);
    struct fvec4 point, barycentric;
    depth_t depth;
    for (int y = (int) p0.components.y; y <= (int) p2.components.y; ++y, xbegin += m0, xend += m1)
    {
        if (y == (int) p1.components.y) { m1 = m2; xend = p1.components.x; }
        
        if (xbegin <= xend) { xmin = xbegin; xmax = xend; }
        else { xmin = xend; xmax = xbegin; }

        xx2 = xmin - p2.components.x;
        yy2 = y - p2.components.y;

        bary0 = (y12 * xx2 + x21 * yy2) * inv_det;
        bary1 = (y20 * xx2 + x02 * yy2) * inv_det;

        for (int x = (int) xmin; x < (int) xmax; ++x, bary0 += da_x, bary1 += db_x)
        {            
            float bary_x = bary0 < 0 ? 0 : bary0 > 1 ? 1 : bary0;
            float bary_y = bary1 < 0 ? 0 : bary1 > 1 ? 1 : bary1;

            barycentric = (struct fvec4) { .buffer = { bary_x, bary_y, 1 - bary_x - bary_y, 0 } };
            interpolate(d0, d1, d2, barycentric, interpolated_data);

            if (zbuffer)
            {
                depth = ternary_interpolate_depth(depth_a, depth_b, depth_c, barycentric);

                point = (struct fvec4) { .buffer = { x, y, depth, 1 } };
                image_depth_draw(img, zbuffer, x, y, depth, shade(point, interpolated_data));
            }
            else
            {
                point = (struct fvec4) { .buffer = { x, y, 0, 1 } };
                image_draw(img, x, y, shade(point, interpolated_data));
            }
        }
    }
    free(interpolated_data);
}

void draw_filled_mesh(struct image *img, depth_buffer_t zbuffer, struct vertex_buffer *vertices, struct index_buffer *indices,
                      void *data, size_t data_size, ternary_interpolation_func_t interpolate, fragment_shader_t shade, cull_func_t cull, void *cull_context)
{
    struct triangle_vertex_indices *begin = index_buffer_begin(indices), *end = index_buffer_end(indices);
    struct fvec4 v0, v1, v2;
    unsigned char *d0, *d1, *d2;
    while (begin != end)
    {
        v0 = vertex_buffer_get(vertices, begin->idx0);
        v1 = vertex_buffer_get(vertices, begin->idx1);
        v2 = vertex_buffer_get(vertices, begin->idx2);

        d0 = (unsigned char*) data + data_size * begin->idx0;
        d1 = (unsigned char*) data + data_size * begin->idx1;
        d2 = (unsigned char*) data + data_size * begin->idx2;

        if (is_not_in_frustum(v0) && is_not_in_frustum(v1) && is_not_in_frustum(v2)) { ++begin; continue; }
        if (cull && cull(v0, d0, v1, d1, v2, d2, cull_context)) { ++begin; continue; }

        v0 = convertToNDC(img, v0);
        v1 = convertToNDC(img, v1);
        v2 = convertToNDC(img, v2);

        /* wprintf(L"(%f %f) -- (%f %f) -- (%f %f)\n", 
            v0.components.x, v0.components.y,
            v1.components.x, v1.components.y,
            v2.components.x, v1.components.y
        ); */

        draw_triangle_filled(img, zbuffer, v0, d0, v1, d1, v2, d2, data_size, interpolate, shade);
        ++begin;
    }
}