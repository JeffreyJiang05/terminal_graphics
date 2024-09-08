#define _POSIX_C_SOURCE 202405L

#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <signal.h>
#include <time.h>

#include "math/linalg.h"
#include "math/transformations.h"
#include "math/functions.h"
#include "gfx/gfx.h"
#include "ui/term_im_conv.h"
#include "gfx/constructs/constructs.h"
#include "ui/terminal.h"
#include "gfx/cameras/flycamera.h"
#include "gfx/lighting/lighting.h"

struct pixel HSL2RGB(double h, double sl, double l)
{
    double v;
    double r,g,b;
    r = l;   // default to gray
    g = l;
    b = l;
    v = (l <= 0.5) ? (l * (1.0 + sl)) : (l + sl - l * sl);
    if (v > 0)
    {
        double m;
        double sv;
        int sextant;
        double fract, vsf, mid1, mid2;
        m = l + l - v;
        sv = (v - m ) / v;
        h *= 6.0;
        sextant = (int)h;
        fract = h - sextant;
        vsf = v * sv * fract;
        mid1 = m + vsf;
        mid2 = v - vsf;
        switch (sextant)
        {
            case 0:
                r = v;
                g = mid1;
                b = m;
                break;
            case 1:
                r = mid2;
                g = v;
                b = m;
                break;
            case 2:
                r = m;
                g = v;
                b = mid1;
                break;
            case 3:
                r = m;
                g = mid2;
                b = v;
                break;
            case 4:
                r = mid1;
                g = m;
                b = v;
                break;
            case 5:
                r = v;
                g = m;
                b = mid2;
                break;
        }
    }

    return (struct pixel)
    {
        .red = (unsigned char) (r * 255),
        .green = (unsigned char) (g * 255),
        .blue = (unsigned char) (b * 255),
        .flags = 0
    };
}

struct pixel value_frag(struct fvec4 pos, void *d0)
{
    static struct pixel rgb;
    size_t t = terminal_frame_count();
#define INV_RAINBOW_SPEED 1
    if (t % INV_RAINBOW_SPEED == 0)
    {
        rgb = HSL2RGB(t / INV_RAINBOW_SPEED % 100 / 100.0, 0.85, 0.75);
    }
    return rgb;
}

void pixel_interpolate(void *d0, void *d1, void *d2, struct fvec4 t, void *d_ret)
{
    struct pixel *p0 = (struct pixel*) d0, *p1 = (struct pixel*) d1, *p2 = (struct pixel*) d2, *pret = (struct pixel*) d_ret;
    
    pret->red = (unsigned char) (p0->red * t.components.x + p1->red * t.components.y + p2->red * t.components.z);
    pret->green = (unsigned char) (p0->green * t.components.x + p1->green * t.components.y + p2->green * t.components.z);
    pret->blue = (unsigned char) (p0->blue * t.components.x + p1->blue * t.components.y + p2->blue * t.components.z);
}

struct pixel color(struct fvec4 pos, void *data)
{
    struct pixel *pix = (struct pixel*) data;
    return *pix;
}

struct pixel light_frag(struct fvec4 pos, void *data)
{
    return (struct pixel) { 255, 255, 255, 0 };
}

struct pixel normal_color(struct fvec4 pos, void *data)
{
    struct construct_datum *d0 = (struct construct_datum*) data;

    struct fvec4 normal = fvec4_normalize(d0->normal);
    unsigned char red = (unsigned char) (normal.components.x * 255);
    unsigned char green = (unsigned char) (normal.components.y * 255);
    unsigned char blue = (unsigned char) (normal.components.z * 255);

    // wprintf(L"%d %d %d\n", red, green, blue);
    return (struct pixel) { red, green, blue, 0 };
}

int main(int argc, char const *argv[])
{
    terminal_init(TERMINAL_MAX_SCREEN_SIZE | TERMINAL_REGISTER_KEY_EVENTS | TERMINAL_REGISTER_MOUSE_EVENTS, 
                  NULL, fly_camera_process_keyboard_input, fly_camera_process_mouse_input);

    fly_camera_init(FVEC4(0, 0, 3, 0), FVEC4(0, 1, 0, 0), 90.0f, 0.0f);

    struct image img;
    depth_buffer_t zbuffer;

    struct vertex_buffer vb;
    struct index_buffer ib;
    struct construct_data data;

    vertex_buffer_init(&vb);
    index_buffer_init(&ib);
    construct_data_init(&data);

    construct_sphere(&vb, &ib, &data, 16, 16);
    // construct_cube(&vb, &ib, &data);
    // construct_flat_cylindrical(&vb, &ib, &data, 0.70710678118, 0.70710678118, 1.0, 4, 1);
    // construct_flat_cylindrical(&vb, &ib, &data, 1, 0.5, 1, 10, 1);
    // construct_smooth_pyramid(&vb, &ib, &data, 1, 1, 12, 1);
    // construct_flat_torus(&vb, &ib, &data, 1, 0.5, 10, 10);
    // construct_smooth_torus(&vb, &ib, &data, 1, 0.5, 10, 10);

    struct vertex_buffer light_vb;
    struct index_buffer light_ib;
    struct construct_data light_data;

    vertex_buffer_init(&light_vb);
    index_buffer_init(&light_ib);
    construct_data_init(&light_data);
    construct_cube(&light_vb, &light_ib, &light_data);

    struct fmat4x4 perspec; 
    struct fmat4x4 lookAt;
    struct fmat4x4 model;  

    struct fmat4x4 light_model = translate_3D(1.2f, -2.0f, 3.0f);
    struct vertex_buffer translated_light_buffer;

    struct vertex_buffer transformed;

    // struct timespec delay = { .tv_sec = 0, .tv_nsec = 1000000 };

    /* set_point_lights(1, (struct point_light_t) {
        .position = { .components = { 0, 5, 5, 1 } },
        .ambient = { .components = { 0.05f, 0.05f, 0.05f, 0.0f } },
        .diffuse = { .components = { 0.8f, 0.8f, 0.8f, 0.0f } },
        .specular = { .components = { 1.0f, 1.0f, 1.0f, 0.0f } },
        .a_constant = 1.0f,
        .a_linear = 0.09f,
        .a_quadratic = 0.032f
    }); */

    set_point_lights(1, (struct point_light_t) {
        .position = FVEC4(1.2f, -2.0f, 3.0f, 0.0f),
        .ambient = FVEC4(0.1f, 0.1f, 0.1f, 0.0f),
        .diffuse = FVEC4(1.0f, 1.0f, 1.0f, 0.0f),
        .specular = FVEC4(1.0f, 1.0f, 1.0f, 0.0f),
        .a_constant = 1.0f,
        .a_linear = 1.0f,
        .a_quadratic = 1.0f
    });

    set_material((struct material) {
        .ambient = FVEC4(1.0f, 0.5f, 0.31f, 0.0f),
        .diffuse = FVEC4(1.0f, 0.5f, 0.31f, 0.0f),
        .specular = FVEC4(0.5f, 0.5f, 0.5f, 0.0f),
        .shininess = 32.0f// 0.21794872f * 128
    });

    struct phong_vshader_data_t *shader_data = malloc(vb.size * sizeof(struct phong_vshader_data_t));

    struct timespec t0, t1;
    float total_fps = 0.0f;

    terminal_clear();
    terminal_hide_cursor();
    while (1)
    {
        clock_gettime(CLOCK_MONOTONIC_RAW, &t0);

        terminal_goto_origin();

        img = image_init(terminal_width(), 2 * terminal_height() - 2);
        zbuffer = depth_buffer_new(&img);

        vertex_buffer_init(&transformed);
        vertex_buffer_init(&translated_light_buffer);

        perspec = fly_camera_get_projection_matrix(&img); 
        // model = fmat4x4_identity();
        model = rotate_3D(1, 1, 0.5, terminal_frame_count() * M_PI / 120);
        lookAt = fly_camera_get_view_matrix();

        set_view_pos(fly_camera_get_position());

        struct fmat4x4 res = fmat4x4_mult(perspec, fmat4x4_mult(lookAt, model));
        // vertex_buffer_apply_transformation(&vb, res, &transformed);

        struct fmat4x4 light_res = fmat4x4_mult(perspec, fmat4x4_mult(lookAt, light_model));
        vertex_buffer_apply_transformation(&light_vb, light_res, &translated_light_buffer);

        for (struct fvec4 *begin = vertex_buffer_begin(&translated_light_buffer), *end = vertex_buffer_end(&translated_light_buffer); begin < end; ++begin)
        {
            *begin = perspective_divide(*begin);
        }

        struct camera_data_t camera_data = { model, lookAt, perspec };
        struct construct_datum *input_begin = construct_data_begin(&data); 
        struct phong_vshader_data_t *shader_begin = shader_data;
        for (struct fvec4 *begin = vertex_buffer_begin(&vb), *end = vertex_buffer_end(&vb); begin < end; ++begin, ++input_begin, ++shader_begin)
        {
            phong_vertex_shader(*begin, camera_data, input_begin, shader_begin);
            struct fvec4 transformed_pos = perspective_divide(fmat4x4_matvec_mult(res, *begin));
            vertex_buffer_add(&transformed, transformed_pos);
        }

        /* for (struct fvec4 *begin = vertex_buffer_begin(&transformed), *end = vertex_buffer_end(&transformed); begin < end; ++begin)
        {
            *begin = perspective_divide(*begin);
        } */

        // draw_wire_frame_mesh(&img, zbuffer, &transformed, &ib, construct_data_begin(&data), sizeof(struct construct_datum), bl_interpolate_construct_datum, value_frag, backface_cull_construct_datum, NULL);
        // draw_wire_frame_mesh(&img, zbuffer, &transformed, &ib, construct_data_begin(&data), sizeof(struct construct_datum), bl_interpolate_construct_datum, value_frag, NULL, NULL);
        // draw_filled_mesh(&img, zbuffer, &transformed, &ib, construct_data_begin(&data), sizeof(struct construct_datum), tl_interpolate_construct_datum, normal_color, backface_cull_construct_datum, NULL);
        draw_filled_mesh(&img, zbuffer, &transformed, &ib, shader_data, sizeof(struct phong_vshader_data_t), 
            phong_vshader_data_ternary_interpolate, phong_frag_shader, backface_cull_construct_datum, NULL);
        // draw_filled_mesh(&img, zbuffer, &transformed, &ib, construct_data_begin(&data), sizeof(struct construct_datum), tl_interpolate_construct_datum, normal_color, NULL, NULL);
        draw_filled_mesh(&img, zbuffer, &translated_light_buffer, &light_ib, construct_data_begin(&light_data), sizeof(struct construct_datum), tl_interpolate_construct_datum, light_frag, backface_cull_construct_datum, NULL);

        struct terminal_image term_img = conv_to_terminal_image(&img);
        print_terminal_image(&term_img);
        free_terminal_image(&term_img);

        vertex_buffer_free(&transformed);
        vertex_buffer_free(&translated_light_buffer);

        depth_buffer_free(zbuffer);
        image_free(&img);

        terminal_process_inputs();
        terminal_increment_frame();

        // sleep(1);
        // nanosleep(&delay, NULL);
        // nanosleep(&terminal.frame_delay, NULL);

        clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
        unsigned long delta_t = (t1.tv_sec - t0.tv_sec) * 1000000000 + (t1.tv_nsec - t0.tv_nsec);
        float fps = 1000000000.0 / delta_t;
        total_fps += fps;
        terminal_goto(1, terminal_height());
        wprintf(L"fps %f - %lu nanoseconds between frames | average fps = %f | screen size %lu x %lu", 
            fps, delta_t, total_fps / terminal_frame_count(), terminal_width(), terminal_height());
    }   
    
    vertex_buffer_free(&vb);
    index_buffer_free(&ib);
    construct_data_free(&data);

    terminal_show_cursor();

    return 0;
}