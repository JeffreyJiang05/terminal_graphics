#ifndef SIMD_UTIL_H
#define SIMD_UTIL_H

#include <x86intrin.h>
 
#ifdef OPTIMIZE

#define SIMD_INLINE __always_inline

#else

#define SIMD_INLINE inline

#endif

#endif