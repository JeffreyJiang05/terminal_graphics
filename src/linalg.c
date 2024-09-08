#include "math/linalg.h"

#include <math.h>
#include <stddef.h>

#include "simd/simd.h"
#include "math/functions.h"

struct fvec4 fvec4_load4f(float x, float y, float z, float w)
{
    struct fvec4 ret;
    ret.components.x = x;
    ret.components.y = y;
    ret.components.z = z;
    ret.components.w = w;
    return ret;
}

struct fvec4 fvec4_add(struct fvec4 a, struct fvec4 b)
{
    struct fvec4 ret;
#ifdef __AVX__
    m128f v0 = f32x4_load(a.buffer);
    m128f v1 = f32x4_load(b.buffer);
    v0 = f32x4_add(v0, v1);
    f32x4_store(ret.buffer, v0);
#else
    for (int i = 0; i < 4; ++i)
    {
        ret.buffer[i] = a.buffer[i] + b.buffer[i];
    }
#endif
    return ret;
}

struct fvec4 fvec4_sub(struct fvec4 a, struct fvec4 b)
{
    struct fvec4 ret;
#ifdef __AVX__
    m128f v0 = f32x4_load(a.buffer);
    m128f v1 = f32x4_load(b.buffer);
    v0 = f32x4_sub(v0, v1);
    f32x4_store(ret.buffer, v0);
#else
    for (int i = 0; i < 4; ++i)
    {
        ret.buffer[i] = a.buffer[i] - b.buffer[i];
    }
#endif
    return ret;
}

struct fvec4 fvec4_mult(struct fvec4 a, struct fvec4 b)
{
    struct fvec4 ret;
#ifdef __AVX__
    m128f v0 = f32x4_load(a.buffer);
    m128f v1 = f32x4_load(b.buffer);
    v0 = f32x4_mul(v0, v1);
    f32x4_store(ret.buffer, v0);
#else
    for (int i = 0; i < 4; ++i)
    {
        ret.buffer[i] = a.buffer[i] * b.buffer[i];
    }
#endif
    return ret;
}

struct fvec4 fvec4_div(struct fvec4 a, struct fvec4 b)
{
    struct fvec4 ret;
#ifdef __AVX__
    m128f v0 = f32x4_load(a.buffer);
    m128f v1 = f32x4_load(b.buffer);
    v0 = f32x4_div(v0, v1);
    f32x4_store(ret.buffer, v0);
#else
    for (int i = 0; i < 4; ++i)
    {
        ret.buffer[i] = a.buffer[i] / b.buffer[i];
    }
#endif
    return ret;
}

struct fvec4 fvec4_scale(struct fvec4 a, float scale)
{
    struct fvec4 ret;
    ret.components.x = a.components.x * scale;
    ret.components.y = a.components.y * scale;
    ret.components.z = a.components.z * scale;
    ret.components.w = a.components.w * scale;
    return ret;
}

float fvec4_magnitude(struct fvec4 a)
{
    float ret[4];
#ifdef __AVX__
    m128f v0 = f32x4_load(a.buffer);
    v0 = f32x4_mul(v0, v0);
    v0 = f32x4_hadd(v0, v0); v0 = f32x4_hadd(v0, v0);
    v0 = f32x4_sqrt(v0);
    f32x4_store(ret, v0);
#else
    ret[0] = 0;
    for (int i = 0; i < 4; ++i)
    {
        ret[0] += a.buffer[i] * a.buffer[i];  
    }
    ret[0] = sqrtf(ret[0]);
#endif
    return ret[0];
}

struct fvec4 fvec4_normalize(struct fvec4 a)
{
    struct fvec4 ret;
#ifdef __AVX__
    m128f v0, v1;
    v0 = f32x4_load(a.buffer);
    v1 = f32x4_mul(v0, v0);
    v1 = f32x4_hadd(v1, v1); v1 = f32x4_hadd(v1, v1);
    v1 = f32x4_sqrt(v1);
    v0 = f32x4_div(v0, v1);
    f32x4_store(ret.buffer, v0);
#else
    ret = fvec4_scale(a, 1 / fvec4_magnitude(a));
#endif
    return ret;
}

struct fvec4 fvec4_cross(struct fvec4 a, struct fvec4 b)
{
    struct fvec4 ret;
#ifdef __AVX__
    m128f v0, v1, v2, v3;
    v0 = f32x4_load(a.buffer);
    v1 = f32x4_load(b.buffer);
    v2 = _mm_permute_ps(v0, 0xC9);
    v3 = _mm_permute_ps(v1, 0xC9);
    v2 = f32x4_mul(v2, v1);
    v3 = f32x4_mul(v3, v0);
    v0 = f32x4_sub(v3, v2);
    v0 = _mm_permute_ps(v0, 0xC9);
    f32x4_store(ret.buffer, v0);
    // ret.components.w = 0;
#else
    ret.components.x = a.components.y * b.components.z - a.components.z * b.components.y;
    ret.components.y = a.components.z * b.components.x - a.components.x * b.components.z;
    ret.components.z = a.components.x * b.components.y - a.components.y * b.components.x;
    ret.components.w = 0;
#endif
    return ret;
}

float fvec4_dot(struct fvec4 a, struct fvec4 b)
{
    float ret[4];
#ifdef __AVX__
    m128f v0 = f32x4_load(a.buffer);
    m128f v1 = f32x4_load(b.buffer);
    v0 = f32x4_mul(v0, v1);
    v0 = f32x4_hadd(v0, v0); v0 = f32x4_hadd(v0, v0);
    f32x4_store(ret, v0);
#else
    ret[0] = 0;
    for (int i = 0; i < 4; ++i)
    {
        ret[0] += a.buffer[i] * b.buffer[i];  
    }
#endif
    return ret[0];
}

void fvec4_linear_interpolate(void *a, void *b, float t, void *ret)
{
    struct fvec4 *fv_a = (struct fvec4*) a, *fv_b = (struct fvec4*) b, *fv_ret = (struct fvec4*) ret;
    float_linear_interpolate(&fv_a->components.x, &fv_b->components.x, t, &fv_ret->components.x);
    float_linear_interpolate(&fv_a->components.y, &fv_b->components.y, t, &fv_ret->components.y);
    float_linear_interpolate(&fv_a->components.z, &fv_b->components.z, t, &fv_ret->components.z);
    float_linear_interpolate(&fv_a->components.w, &fv_b->components.w, t, &fv_ret->components.w);
}

void fvec4_ternary_linear_interpolate(void *a, void *b, void *c, struct fvec4 t, void *ret)
{
    struct fvec4 *fv_a = (struct fvec4*) a, *fv_b = (struct fvec4*) b, *fv_c = (struct fvec4*) c, *fv_ret = (struct fvec4*) ret;
    float_ternary_linear_interpolate(&fv_a->components.x, &fv_b->components.x, &fv_c->components.x, t, &fv_ret->components.x);
    float_ternary_linear_interpolate(&fv_a->components.y, &fv_b->components.y, &fv_c->components.y, t, &fv_ret->components.y);
    float_ternary_linear_interpolate(&fv_a->components.z, &fv_b->components.z, &fv_c->components.z, t, &fv_ret->components.z);
    float_ternary_linear_interpolate(&fv_a->components.w, &fv_b->components.w, &fv_c->components.w, t, &fv_ret->components.w);
}

/**
 * Matrix ops
*/

struct fmat4x4 fmat4x4_load4v(struct fvec4 v0, struct fvec4 v1, struct fvec4 v2, struct fvec4 v3)
{
    struct fmat4x4 ret;
    ret.cols[0] = v0;
    ret.cols[1] = v1;
    ret.cols[2] = v2;
    ret.cols[3] = v3;
    return ret;
}

struct fmat4x4 fmat4x4_identity()
{
    return fmat4x4_load4v(
        (struct fvec4) { .buffer={ 1, 0, 0, 0 } },
        (struct fvec4) { .buffer={ 0, 1, 0, 0 } },
        (struct fvec4) { .buffer={ 0, 0, 1, 0 } },
        (struct fvec4) { .buffer={ 0, 0, 0, 1 } }
    );
}

struct fmat4x4 fmat4x4_add(struct fmat4x4 a, struct fmat4x4 b)
{
    struct fmat4x4 ret;
#ifdef __AVX__
    m128f v0,v1,v2,v3,v4,v5,v6,v7;
    v0 = f32x4_load(&a.buffer[0]);
    v4 = f32x4_load(&b.buffer[0]);
    v1 = f32x4_load(&a.buffer[4]);
    v5 = f32x4_load(&b.buffer[4]);
    v2 = f32x4_load(&a.buffer[8]);
    v6 = f32x4_load(&b.buffer[8]);
    v3 = f32x4_load(&a.buffer[12]);
    v7 = f32x4_load(&b.buffer[12]);

    v0 = f32x4_add(v0, v4);
    v1 = f32x4_add(v1, v5);
    v2 = f32x4_add(v2, v6);
    v3 = f32x4_add(v3, v7);

    f32x4_store(&ret.buffer[0], v0);
    f32x4_store(&ret.buffer[4], v1);
    f32x4_store(&ret.buffer[8], v2);
    f32x4_store(&ret.buffer[12], v3);
#else
    for (int i = 0; i < 4; ++i)
    {
        ret.cols[i] = fvec4_add(a.cols[i], b.cols[i]);
    }
#endif
    return ret;
}

struct fmat4x4 fmat4x4_sub(struct fmat4x4 a, struct fmat4x4 b)
{
    struct fmat4x4 ret;
#ifdef __AVX__
    m128f v0,v1,v2,v3,v4,v5,v6,v7;
    v0 = f32x4_load(&a.buffer[0]);
    v4 = f32x4_load(&b.buffer[0]);
    v1 = f32x4_load(&a.buffer[4]);
    v5 = f32x4_load(&b.buffer[4]);
    v2 = f32x4_load(&a.buffer[8]);
    v6 = f32x4_load(&b.buffer[8]);
    v3 = f32x4_load(&a.buffer[12]);
    v7 = f32x4_load(&b.buffer[12]);

    v0 = f32x4_sub(v0, v4);
    v1 = f32x4_sub(v1, v5);
    v2 = f32x4_sub(v2, v6);
    v3 = f32x4_sub(v3, v7);

    f32x4_store(&ret.buffer[0], v0);
    f32x4_store(&ret.buffer[4], v1);
    f32x4_store(&ret.buffer[8], v2);
    f32x4_store(&ret.buffer[12], v3);
#else
    for (int i = 0; i < 4; ++i)
    {
        ret.cols[i] = fvec4_sub(a.cols[i], b.cols[i]);
    }
#endif
    return ret;
}

struct fmat4x4 fmat4x4_mult(struct fmat4x4 a, struct fmat4x4 b)
{
    struct fmat4x4 ret;
#ifdef __AVX__
    m128f v0,v1,v2,v3,v4,v5,v6,v7,m0,m1,m2,m3;
    v0 = f32x4_load(&a.buffer[0]);
    v1 = f32x4_load(&a.buffer[4]);
    v2 = f32x4_load(&a.buffer[8]);
    v3 = f32x4_load(&a.buffer[12]);

    m0 = f32x4_load(&b.buffer[0]);
    m1 = f32x4_load(&b.buffer[4]);
    m2 = f32x4_load(&b.buffer[8]);
    m3 = f32x4_load(&b.buffer[12]);

    v4 = _mm_unpacklo_ps(v0, v1);
    v5 = _mm_unpackhi_ps(v0, v1);
    v6 = _mm_unpacklo_ps(v2, v3);
    v7 = _mm_unpackhi_ps(v2, v3);

    v0 = _mm_shuffle_ps(v4, v6, 0x44);
    v1 = _mm_shuffle_ps(v4, v6, 0xEE);
    v2 = _mm_shuffle_ps(v5, v7, 0x44);
    v3 = _mm_shuffle_ps(v5, v7, 0xEE);

    // work with m0

    v4 = f32x4_mul(v0, m0); v5 = f32x4_mul(v1, m0); v6 = f32x4_mul(v2, m0); v7 = f32x4_mul(v3, m0);
    v4 = f32x4_hadd(v4, v4); v5 = f32x4_hadd(v5, v5); v6 = f32x4_hadd(v6, v6); v7 = f32x4_hadd(v7, v7);
    v4 = f32x4_hadd(v4, v4); v5 = f32x4_hadd(v5, v5); v6 = f32x4_hadd(v6, v6); v7 = f32x4_hadd(v7, v7);
    v4 = _mm_unpacklo_ps(v4, v5);
    v6 = _mm_unpacklo_ps(v6, v7);
    m0 = _mm_blend_ps(v4, v6, 0xC);

    v4 = f32x4_mul(v0, m1); v5 = f32x4_mul(v1, m1); v6 = f32x4_mul(v2, m1); v7 = f32x4_mul(v3, m1);
    v4 = f32x4_hadd(v4, v4); v5 = f32x4_hadd(v5, v5); v6 = f32x4_hadd(v6, v6); v7 = f32x4_hadd(v7, v7);
    v4 = f32x4_hadd(v4, v4); v5 = f32x4_hadd(v5, v5); v6 = f32x4_hadd(v6, v6); v7 = f32x4_hadd(v7, v7);
    v4 = _mm_unpacklo_ps(v4, v5);
    v6 = _mm_unpacklo_ps(v6, v7);
    m1 = _mm_blend_ps(v4, v6, 0xC);

    v4 = f32x4_mul(v0, m2); v5 = f32x4_mul(v1, m2); v6 = f32x4_mul(v2, m2); v7 = f32x4_mul(v3, m2);
    v4 = f32x4_hadd(v4, v4); v5 = f32x4_hadd(v5, v5); v6 = f32x4_hadd(v6, v6); v7 = f32x4_hadd(v7, v7);
    v4 = f32x4_hadd(v4, v4); v5 = f32x4_hadd(v5, v5); v6 = f32x4_hadd(v6, v6); v7 = f32x4_hadd(v7, v7);
    v4 = _mm_unpacklo_ps(v4, v5);
    v6 = _mm_unpacklo_ps(v6, v7);
    m2 = _mm_blend_ps(v4, v6, 0xC);

    v4 = f32x4_mul(v0, m3); v5 = f32x4_mul(v1, m3); v6 = f32x4_mul(v2, m3); v7 = f32x4_mul(v3, m3);
    v4 = f32x4_hadd(v4, v4); v5 = f32x4_hadd(v5, v5); v6 = f32x4_hadd(v6, v6); v7 = f32x4_hadd(v7, v7);
    v4 = f32x4_hadd(v4, v4); v5 = f32x4_hadd(v5, v5); v6 = f32x4_hadd(v6, v6); v7 = f32x4_hadd(v7, v7);
    v4 = _mm_unpacklo_ps(v4, v5);
    v6 = _mm_unpacklo_ps(v6, v7);
    m3 = _mm_blend_ps(v4, v6, 0xC);

    f32x4_store(&ret.buffer[0], m0);
    f32x4_store(&ret.buffer[4], m1);
    f32x4_store(&ret.buffer[8], m2);
    f32x4_store(&ret.buffer[12], m3);
#else
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            ret.buffer[4 * j + i] = 0;
            for (int k = 0; k < 4; ++k)
            {
                ret.buffer[4 * j + i] += a.buffer[4 * k + i] * b.buffer[4 * j + k];
            }
        }
    }
#endif
    return ret;
}

struct fvec4 fmat4x4_matvec_mult(struct fmat4x4 a, struct fvec4 b)
{
    struct fvec4 ret;
#ifdef __AVX__
    m128f v0,v1,v2,v3,v4,v5,v6,v7,m0;
    v0 = f32x4_load(&a.buffer[0]);
    v1 = f32x4_load(&a.buffer[4]);
    v2 = f32x4_load(&a.buffer[8]);
    v3 = f32x4_load(&a.buffer[12]);

    m0 = f32x4_load(&b.buffer[0]);

    v4 = _mm_unpacklo_ps(v0, v1);
    v5 = _mm_unpackhi_ps(v0, v1);
    v6 = _mm_unpacklo_ps(v2, v3);
    v7 = _mm_unpackhi_ps(v2, v3);

    v0 = _mm_shuffle_ps(v4, v6, 0x44);
    v1 = _mm_shuffle_ps(v4, v6, 0xEE);
    v2 = _mm_shuffle_ps(v5, v7, 0x44);
    v3 = _mm_shuffle_ps(v5, v7, 0xEE);

    v4 = f32x4_mul(v0, m0); v5 = f32x4_mul(v1, m0); v6 = f32x4_mul(v2, m0); v7 = f32x4_mul(v3, m0);
    v4 = f32x4_hadd(v4, v4); v5 = f32x4_hadd(v5, v5); v6 = f32x4_hadd(v6, v6); v7 = f32x4_hadd(v7, v7);
    v4 = f32x4_hadd(v4, v4); v5 = f32x4_hadd(v5, v5); v6 = f32x4_hadd(v6, v6); v7 = f32x4_hadd(v7, v7);
    v4 = _mm_unpacklo_ps(v4, v5);
    v6 = _mm_unpacklo_ps(v6, v7);
    m0 = _mm_blend_ps(v4, v6, 0xC);

    f32x4_store(&ret.buffer[0], m0);
#else
    struct fmat4x4 t_a = fmat4x4_transpose(a);
    for (int i = 0; i < 4; ++i)
    {
        ret.buffer[i] = fvec4_dot(t_a.cols[i], b);
    }
#endif
    return ret;
}

struct fmat4x4 fmat4x4_scale(struct fmat4x4 a, float scale)
{
    struct fmat4x4 ret;
    for (int i = 0; i < 4; ++i)
    {
        ret.cols[i] = fvec4_scale(a.cols[i], scale);
    }
    return ret;
}

struct fmat4x4 fmat4x4_transpose(struct fmat4x4 a)
{
    struct fmat4x4 ret;
#ifdef __AVX__
    m128f v0,v1,v2,v3,v4,v5,v6,v7;
    v0 = f32x4_load(&a.buffer[0]);
    v1 = f32x4_load(&a.buffer[4]);
    v2 = f32x4_load(&a.buffer[8]);
    v3 = f32x4_load(&a.buffer[12]);

    v4 = _mm_unpacklo_ps(v0, v1);
    v5 = _mm_unpackhi_ps(v0, v1);
    v6 = _mm_unpacklo_ps(v2, v3);
    v7 = _mm_unpackhi_ps(v2, v3);

    v0 = _mm_shuffle_ps(v4, v6, 0x44);
    v1 = _mm_shuffle_ps(v4, v6, 0xEE);
    v2 = _mm_shuffle_ps(v5, v7, 0x44);
    v3 = _mm_shuffle_ps(v5, v7, 0xEE);

    f32x4_store(&ret.buffer[0], v0);
    f32x4_store(&ret.buffer[4], v1);
    f32x4_store(&ret.buffer[8], v2);
    f32x4_store(&ret.buffer[12], v3);
#else
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            ret.buffer[4 * i + j] = a.buffer[4 * j + i];
        }
    }
#endif
    return ret;
}

static float fm_get(struct fmat4x4 *mat, size_t col, size_t row)
{
    return mat->cols[col].buffer[row];
}

struct fmat4x4 fmat4x4_inv(struct fmat4x4 m)
{
    float cf_00, cf_01, cf_02, cf_03;
    float cf_10, cf_11, cf_12, cf_13;
    float cf_20, cf_21, cf_22, cf_23;
    float cf_30, cf_31, cf_32, cf_33;
    float det;

    cf_00 = fm_get(&m, 1, 1) * (fm_get(&m, 2, 2) * fm_get(&m, 3, 3) - fm_get(&m, 3, 2) * fm_get(&m, 2, 3)) 
          + fm_get(&m, 1, 2) * (fm_get(&m, 3, 1) * fm_get(&m, 2, 3) - fm_get(&m, 2, 1) * fm_get(&m, 3, 3)) 
          + fm_get(&m, 1, 3) * (fm_get(&m, 2, 1) * fm_get(&m, 3, 2) - fm_get(&m, 3, 1) * fm_get(&m, 2, 2)); 
    cf_10 = fm_get(&m, 0, 1) * (fm_get(&m, 3, 2) * fm_get(&m, 2, 3) - fm_get(&m, 2, 2) * fm_get(&m, 3, 3))
          + fm_get(&m, 0, 2) * (fm_get(&m, 2, 1) * fm_get(&m, 3, 3) - fm_get(&m, 3, 1) * fm_get(&m, 2, 3))
          + fm_get(&m, 0, 3) * (fm_get(&m, 3, 1) * fm_get(&m, 2, 2) - fm_get(&m, 2, 1) * fm_get(&m, 3, 2));
    cf_20 = fm_get(&m, 3, 1) * (fm_get(&m, 0, 2) * fm_get(&m, 1, 3) - fm_get(&m, 1, 2) * fm_get(&m, 0, 3))
          + fm_get(&m, 3, 2) * (fm_get(&m, 1, 1) * fm_get(&m, 0, 3) - fm_get(&m, 0, 1) * fm_get(&m, 1, 3))
          + fm_get(&m, 3, 3) * (fm_get(&m, 0, 1) * fm_get(&m, 1, 2) - fm_get(&m, 1, 1) * fm_get(&m, 0, 2));
    cf_30 = fm_get(&m, 2, 1) * (fm_get(&m, 1, 2) * fm_get(&m, 0, 3) - fm_get(&m, 0, 2) * fm_get(&m, 1, 3))
          + fm_get(&m, 2, 2) * (fm_get(&m, 0, 1) * fm_get(&m, 1, 3) - fm_get(&m, 1, 1) * fm_get(&m, 0, 3))
          + fm_get(&m, 2, 3) * (fm_get(&m, 1, 1) * fm_get(&m, 0, 2) - fm_get(&m, 0, 1) * fm_get(&m, 1, 2));

    cf_01 = fm_get(&m, 1, 0) * (fm_get(&m, 3, 2) * fm_get(&m, 2, 3) - fm_get(&m, 2, 2) * fm_get(&m, 3, 3))
          + fm_get(&m, 1, 2) * (fm_get(&m, 2, 0) * fm_get(&m, 3, 3) - fm_get(&m, 3, 0) * fm_get(&m, 2, 3))
          + fm_get(&m, 1, 3) * (fm_get(&m, 3, 0) * fm_get(&m, 2, 2) - fm_get(&m, 2, 0) * fm_get(&m, 3, 2));
    cf_11 = fm_get(&m, 0, 0) * (fm_get(&m, 2, 2) * fm_get(&m, 3, 3) - fm_get(&m, 3, 2) * fm_get(&m, 2, 3))
          + fm_get(&m, 0, 2) * (fm_get(&m, 3, 0) * fm_get(&m, 2, 3) - fm_get(&m, 2, 0) * fm_get(&m, 3, 3))
          + fm_get(&m, 0, 3) * (fm_get(&m, 2, 0) * fm_get(&m, 3, 2) - fm_get(&m, 3, 0) * fm_get(&m, 2, 2));
    cf_21 = fm_get(&m, 3, 0) * (fm_get(&m, 1, 2) * fm_get(&m, 0, 3) - fm_get(&m, 0, 2) * fm_get(&m, 1, 3))
          + fm_get(&m, 3, 2) * (fm_get(&m, 0, 0) * fm_get(&m, 1, 3) - fm_get(&m, 1, 0) * fm_get(&m, 0, 3))
          + fm_get(&m, 3, 3) * (fm_get(&m, 1, 0) * fm_get(&m, 0, 2) - fm_get(&m, 0, 0) * fm_get(&m, 1, 2));
    cf_31 = fm_get(&m, 2, 0) * (fm_get(&m, 0, 2) * fm_get(&m, 1, 3) - fm_get(&m, 1, 2) * fm_get(&m, 0, 3))
          + fm_get(&m, 2, 2) * (fm_get(&m, 1, 0) * fm_get(&m, 0, 3) - fm_get(&m, 0, 0) * fm_get(&m, 1, 3))
          + fm_get(&m, 2, 3) * (fm_get(&m, 0, 0) * fm_get(&m, 1, 2) - fm_get(&m, 1, 0) * fm_get(&m, 0, 2));

    cf_02 = fm_get(&m, 1, 0) * (fm_get(&m, 2, 1) * fm_get(&m, 3, 3) - fm_get(&m, 3, 1) * fm_get(&m, 2, 3))
          + fm_get(&m, 1, 1) * (fm_get(&m, 3, 0) * fm_get(&m, 2, 3) - fm_get(&m, 2, 0) * fm_get(&m, 3, 3))
          + fm_get(&m, 1, 3) * (fm_get(&m, 2, 0) * fm_get(&m, 3, 1) - fm_get(&m, 3, 0) * fm_get(&m, 2, 1));
    cf_12 = fm_get(&m, 0, 0) * (fm_get(&m, 3, 1) * fm_get(&m, 2, 3) - fm_get(&m, 2, 1) * fm_get(&m, 3, 3)) 
          + fm_get(&m, 0, 1) * (fm_get(&m, 2, 0) * fm_get(&m, 3, 3) - fm_get(&m, 3, 0) * fm_get(&m, 2, 3)) 
          + fm_get(&m, 0, 3) * (fm_get(&m, 3, 0) * fm_get(&m, 2, 1) - fm_get(&m, 2, 0) * fm_get(&m, 3, 1));
    cf_22 = fm_get(&m, 3, 0) * (fm_get(&m, 0, 1) * fm_get(&m, 1, 3) - fm_get(&m, 1, 1) * fm_get(&m, 0, 3)) 
          + fm_get(&m, 3, 1) * (fm_get(&m, 1, 0) * fm_get(&m, 0, 3) - fm_get(&m, 0, 0) * fm_get(&m, 1, 3)) 
          + fm_get(&m, 3, 3) * (fm_get(&m, 0, 0) * fm_get(&m, 1, 1) - fm_get(&m, 1, 0) * fm_get(&m, 0, 1));
    cf_32 = fm_get(&m, 2, 0) * (fm_get(&m, 1, 1) * fm_get(&m, 0, 3) - fm_get(&m, 0, 1) * fm_get(&m, 1, 3)) 
          + fm_get(&m, 2, 1) * (fm_get(&m, 0, 0) * fm_get(&m, 1, 3) - fm_get(&m, 1, 0) * fm_get(&m, 0, 3)) 
          + fm_get(&m, 2, 3) * (fm_get(&m, 1, 0) * fm_get(&m, 0, 1) - fm_get(&m, 0, 0) * fm_get(&m, 1, 1));

    cf_03 = fm_get(&m, 1, 0) * (fm_get(&m, 3, 1) * fm_get(&m, 2, 2) - fm_get(&m, 2, 1) * fm_get(&m, 3, 2)) 
          + fm_get(&m, 1, 1) * (fm_get(&m, 2, 0) * fm_get(&m, 3, 2) - fm_get(&m, 3, 0) * fm_get(&m, 2, 2)) 
          + fm_get(&m, 1, 2) * (fm_get(&m, 3, 0) * fm_get(&m, 2, 1) - fm_get(&m, 2, 0) * fm_get(&m, 3, 1));
    cf_13 = fm_get(&m, 0, 0) * (fm_get(&m, 2, 1) * fm_get(&m, 3, 2) - fm_get(&m, 3, 1) * fm_get(&m, 2, 2)) 
          + fm_get(&m, 0, 1) * (fm_get(&m, 3, 0) * fm_get(&m, 2, 2) - fm_get(&m, 2, 0) * fm_get(&m, 3, 2)) 
          + fm_get(&m, 0, 2) * (fm_get(&m, 2, 0) * fm_get(&m, 3, 1) - fm_get(&m, 3, 0) * fm_get(&m, 2, 1));
    cf_23 = fm_get(&m, 3, 0) * (fm_get(&m, 1, 1) * fm_get(&m, 0, 2) - fm_get(&m, 0, 1) * fm_get(&m, 1, 2)) 
          + fm_get(&m, 3, 1) * (fm_get(&m, 0, 0) * fm_get(&m, 1, 2) - fm_get(&m, 1, 0) * fm_get(&m, 0, 2)) 
          + fm_get(&m, 3, 2) * (fm_get(&m, 1, 0) * fm_get(&m, 0, 1) - fm_get(&m, 0, 0) * fm_get(&m, 1, 1));
    cf_33 = fm_get(&m, 2, 0) * (fm_get(&m, 0, 1) * fm_get(&m, 1, 2) - fm_get(&m, 1, 1) * fm_get(&m, 0, 2)) 
          + fm_get(&m, 2, 1) * (fm_get(&m, 1, 0) * fm_get(&m, 0, 2) - fm_get(&m, 0, 0) * fm_get(&m, 1, 2)) 
          + fm_get(&m, 2, 2) * (fm_get(&m, 0, 0) * fm_get(&m, 1, 1) - fm_get(&m, 1, 0) * fm_get(&m, 0, 1));

    det = fm_get(&m, 0, 0) * cf_00 + fm_get(&m, 0, 1) * cf_01 + fm_get(&m, 0, 2) * cf_02 + fm_get(&m, 0, 3) * cf_03;

    struct fmat4x4 ret = { .cols = {
        { .buffer = { cf_00, cf_10, cf_20, cf_30 } },
        { .buffer = { cf_01, cf_11, cf_21, cf_31 } },
        { .buffer = { cf_02, cf_12, cf_22, cf_32 } },
        { .buffer = { cf_03, cf_13, cf_23, cf_33 } }
    }};

    return fmat4x4_scale(ret, 1 / det);
}

struct fmat4x4 fmat4x4_conv3x3(struct fmat4x4 m)
{
    m.cols[0].buffer[3] = 0;
    m.cols[1].buffer[3] = 0;
    m.cols[2].buffer[3] = 0;
    m.cols[3] = (struct fvec4) { .buffer = { 0, 0, 0, 0 } };
    return m;
}