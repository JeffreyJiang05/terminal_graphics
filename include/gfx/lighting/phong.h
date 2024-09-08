#ifndef LIGHTING_PHONG_H
#define LIGHTING_PHONG_H

#include "gfx/image.h"
#include "math/linalg.h"
#include "gfx/constructs/construct_data.h"

struct phong_vshader_data_t
{
    struct fvec4 norm;
    struct fvec4 tex;
    struct fvec4 frag_pos;
};

struct camera_data_t
{
    struct fmat4x4 model;
    struct fmat4x4 view;
    struct fmat4x4 projection;
};

void phong_vertex_shader(struct fvec4 pos, struct camera_data_t camera, struct construct_datum *in_data, struct phong_vshader_data_t *out_data);

void phong_vshader_data_binary_interpolate(void *d0, void *d1, float t, void *d_ret);

void phong_vshader_data_ternary_interpolate(void *d0, void *d1, void *d2, struct fvec4 t, void *d_ret);

struct pixel phong_frag_shader(struct fvec4 pos, void *data);

#endif 