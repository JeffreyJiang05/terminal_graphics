#include "gfx/lighting/phong.h"

#include <math.h>

#include "gfx/lighting/lights.h"
#include "gfx/lighting/materials.h"
#include "math/functions.h"

void phong_vertex_shader(struct fvec4 pos, struct camera_data_t camera, struct construct_datum *in_data, struct phong_vshader_data_t *out_data)
{
    struct fmat4x4 m = fmat4x4_transpose(fmat4x4_inv(camera.model));
    out_data->norm = fmat4x4_matvec_mult(m, pos);
    out_data->norm.buffer[3] = 1;

    out_data->tex = in_data->texture;
    out_data->frag_pos = fmat4x4_matvec_mult(camera.model, pos);
}

void phong_vshader_data_binary_interpolate(void *d0, void *d1, float t, void *d_ret)
{
    struct phong_vshader_data_t *v0 = (struct phong_vshader_data_t*) d0;
    struct phong_vshader_data_t *v1 = (struct phong_vshader_data_t*) d1;
    struct phong_vshader_data_t *v_ret = (struct phong_vshader_data_t*) d_ret;

    fvec4_linear_interpolate(&v0->norm, &v1->norm, t, &v_ret->norm);
    fvec4_linear_interpolate(&v0->tex, &v1->tex, t, &v_ret->tex);
    fvec4_linear_interpolate(&v0->frag_pos, &v1->frag_pos, t, &v_ret->frag_pos);
}

void phong_vshader_data_ternary_interpolate(void *d0, void *d1, void *d2, struct fvec4 t, void *d_ret)
{
    struct phong_vshader_data_t *v0 = (struct phong_vshader_data_t*) d0;
    struct phong_vshader_data_t *v1 = (struct phong_vshader_data_t*) d1;
    struct phong_vshader_data_t *v2 = (struct phong_vshader_data_t*) d2;
    struct phong_vshader_data_t *v_ret = (struct phong_vshader_data_t*) d_ret;

    fvec4_ternary_linear_interpolate(&v0->norm, &v1->norm, &v2->norm, t, &v_ret->norm);
    fvec4_ternary_linear_interpolate(&v0->tex, &v1->tex, &v2->tex, t, &v_ret->tex);
    fvec4_ternary_linear_interpolate(&v0->frag_pos, &v1->frag_pos, &v2->frag_pos, t, &v_ret->frag_pos);
}

static struct fvec4 calculate_point_light(struct point_light_t light, struct fvec4 normal, struct fvec4 frag_pos, struct fvec4 view_dir)
{
    struct fvec4 light_dir = fvec4_normalize(fvec4_sub(light.position, frag_pos));
    light_dir.components.w = 0;

    float diff = fmaxf(fvec4_dot(normal, light_dir), 0.0);
    struct fvec4 point_light = fvec4_mult(light.ambient, get_material().ambient);
    point_light = fvec4_add(point_light, fvec4_scale(fvec4_mult(light.diffuse, get_material().diffuse), diff));

    if (diff > 0)
    {
        struct fvec4 reflect_dir = fvec4_sub(light_dir, fvec4_scale(normal, 2 * fvec4_dot(normal, light_dir)));
        float spec = powf(fmaxf(fvec4_dot(view_dir, reflect_dir), 0.0), get_material().shininess);
        point_light = fvec4_add(point_light, fvec4_scale(fvec4_mult(light.specular, get_material().specular), spec));
    }

    // float distance = fvec4_magnitude(light_dir);
    // float attenuation = 1 / (light.a_constant + light.a_linear * distance + light.a_quadratic * distance * distance);
    // point_light = fvec4_scale(point_light, attenuation);

    return point_light;
}

struct pixel phong_frag_shader(struct fvec4 pos, void *data)
{
    struct phong_vshader_data_t *d = (struct phong_vshader_data_t*) data;

    struct fvec4 norm = fvec4_normalize(d->norm);
    norm.components.w = 0;
    struct fvec4 view_dir = fvec4_normalize(fvec4_sub(d->frag_pos, get_view_pos()));
    view_dir.components.w = 0;

    struct fvec4 result = { .buffer = { 0, 0, 0, 0 } };

    struct point_light_t point_light;
    for (size_t i = 0; i < get_point_light_count(); ++i)
    {
        get_point_light(i, &point_light);
        result = fvec4_add(result, calculate_point_light(point_light, norm, d->frag_pos, view_dir));
    }

    if (result.buffer[0] > 1.0) result.buffer[0] = 1.0;
    if (result.buffer[1] > 1.0) result.buffer[1] = 1.0;
    if (result.buffer[2] > 1.0) result.buffer[2] = 1.0;

    result = fvec4_scale(result, 255.0);

    return (struct pixel) {
        .red = (unsigned char) result.buffer[0],
        .green = (unsigned char) result.buffer[1],
        .blue = (unsigned char) result.buffer[2],
        .flags = 0
    };
}