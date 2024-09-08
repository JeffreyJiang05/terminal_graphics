#ifndef MATH_LINEAR_ALGEBRA_STRUCTURES_H
#define MATH_LINEAR_ALGEBRA_STRUCTURES_H

#include <assert.h>

struct fvec4
{
    union 
    {
        float buffer[4];
        struct {
            float x;
            float y;
            float z;
            float w;
        } components;
    };
};

static_assert(sizeof(struct fvec4) == sizeof(float) * 4, "Incorrect Size of fvec4");

#ifndef FVEC4
    #define FVEC4(x,y,z,w) (struct fvec4) { .buffer = { x, y, z, w } }
#endif

struct fvec4 fvec4_load4f(float x, float y, float z, float w);
struct fvec4 fvec4_add(struct fvec4 a, struct fvec4 b);
struct fvec4 fvec4_sub(struct fvec4 a, struct fvec4 b);
struct fvec4 fvec4_mult(struct fvec4 a, struct fvec4 b);
struct fvec4 fvec4_div(struct fvec4 a, struct fvec4 b);
struct fvec4 fvec4_scale(struct fvec4 a, float scale);
float fvec4_magnitude(struct fvec4 a);
struct fvec4 fvec4_normalize(struct fvec4 a);
struct fvec4 fvec4_cross(struct fvec4 a, struct fvec4 b);
float fvec4_dot(struct fvec4 a, struct fvec4 b);

void fvec4_linear_interpolate(void *a, void *b, float t, void *ret);
void fvec4_ternary_linear_interpolate(void *a, void *b, void *c, struct fvec4 t, void *ret);

struct fmat4x4
{
    union
    {
        float buffer[4 * 4];
        struct fvec4 cols[4];
    };
};

static_assert(sizeof(struct fmat4x4) == sizeof(float) * 4 * 4, "Incorrect size of fmat4x4");

#ifndef FMAT4X4
    #define FMAT4X4(c0,c1,c2,c3) (struct fmat4x4) { .cols = { c0, c1, c2, c3 } }
#endif

struct fmat4x4 fmat4x4_load4v(struct fvec4 v0, struct fvec4 v1, struct fvec4 v2, struct fvec4 v3);
struct fmat4x4 fmat4x4_identity();
struct fmat4x4 fmat4x4_add(struct fmat4x4 a, struct fmat4x4 b);
struct fmat4x4 fmat4x4_sub(struct fmat4x4 a, struct fmat4x4 b);
struct fmat4x4 fmat4x4_mult(struct fmat4x4 a, struct fmat4x4 b);
struct fvec4 fmat4x4_matvec_mult(struct fmat4x4 a, struct fvec4 b);
struct fmat4x4 fmat4x4_scale(struct fmat4x4 a, float scale);
struct fmat4x4 fmat4x4_transpose(struct fmat4x4 a);
struct fmat4x4 fmat4x4_inv(struct fmat4x4 m);
struct fmat4x4 fmat4x4_conv3x3(struct fmat4x4 m);

#endif