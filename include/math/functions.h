#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H

#include "math/linalg.h"

void float_linear_interpolate(void *a, void *b, float t, void *ret);

void double_linear_interpolate(void *a, void *b, float t, void *ret);

void int_linear_interpolate(void *a, void *b, float t, void *ret);

void float_ternary_linear_interpolate(void *a, void *b, void *c, struct fvec4 t, void *ret);

void double_ternary_linear_interpolate(void *a, void *b, void *c, struct fvec4 t, void *ret);

void int_ternary_linear_interpolate(void *a, void *b, void *c, struct fvec4 t, void *ret);

#endif