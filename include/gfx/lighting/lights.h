#ifndef GFX_LIGHTS_H
#define GFX_LIGHTS_H

#include <stddef.h>

#include "math/linalg.h"

struct point_light_t
{
    struct fvec4 position;

    struct fvec4 ambient;
    struct fvec4 diffuse;
    struct fvec4 specular;

    float a_constant;
    float a_linear;
    float a_quadratic;
};

struct directional_light_t
{
    struct fvec4 direction;
    struct fvec4 ambient;
    struct fvec4 diffuse;
    struct fvec4 specular;
};

struct fvec4 get_view_pos();

void set_view_pos(struct fvec4 view_pos);

size_t get_point_light_count();

void get_point_light(size_t n, struct point_light_t *point_light);

void set_point_lights(size_t count, ...);

size_t get_directional_light_count();

void get_directional_light(size_t n, struct directional_light_t *directional_light);

void set_directional_lights(size_t count, ...);

#endif