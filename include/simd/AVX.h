#ifndef SIMD_AVX_H
#define SIMD_AVX_H

#include <x86intrin.h>

#include "util.h"

#define INLINE __always_inline

typedef __m128 m128f;
typedef __m128_u m128f_u;

SIMD_INLINE m128f f32x4_load(float* buffer)
{
    return _mm_loadu_ps(buffer);
}

SIMD_INLINE void f32x4_store(float* buffer, m128f v0)
{
    _mm_storeu_ps(buffer, v0);
}

SIMD_INLINE m128f f32x4_add(m128f v0, m128f v1)
{
    return _mm_add_ps(v0, v1);
}

SIMD_INLINE m128f f32x4_hadd(m128f v0, m128f v1)
{
    return _mm_hadd_ps(v0, v1);
}

SIMD_INLINE m128f f32x4_sub(m128f v0, m128f v1)
{
    return _mm_sub_ps(v0, v1);
}

SIMD_INLINE m128f f32x4_hsub(m128f v0, m128f v1)
{
    return _mm_sub_ps(v0, v1);
}

SIMD_INLINE m128f f32x4_mul(m128f v0, m128f v1)
{
    return _mm_mul_ps(v0, v1);
}

SIMD_INLINE m128f f32x4_div(m128f v0, m128f v1)
{
    return _mm_div_ps(v0, v1);
}

SIMD_INLINE m128f f32x4_sqrt(m128f v0)
{
    return _mm_sqrt_ps(v0);
}

#ifdef __FMA__

// v0 * v1 + v2
SIMD_INLINE m128f f32x4_fma(m128f v0, m128f v1, m128f v2)
{
    return _mm_fmadd_ps(v0, v1, v2);
}

// v0 * v1 - v2
SIMD_INLINE m128f f32x4_fms(m128f v0, m128f v1, m128f v2)
{
    return _mm_fmsub_ps(v0, v1, v2);
}

#else

SIMD_INLINE m128f f32x4_fma(m128f v0, m128f v1, m128f v2)
{
    v0 = f32x4_mul(v0, v1);
    return f32x4_add(v0, v2);
}

SIMD_INLINE m128f f32x4_fms(m128f v0, m128f v1, m128f v2)
{
    v0 = f32x4_mul(v0, v1);
    return f32x4_sub(v0, v2);
}

#endif

#endif