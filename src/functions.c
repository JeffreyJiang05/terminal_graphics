#include "math/functions.h"

void float_linear_interpolate(void *a, void *b, float t, void *ret)
{
    float *fa = (float*) a, *fb = (float*) b, *fret = (float*) ret;
    *fret = *fa * (1 - t) + *fb * t;
}

void double_linear_interpolate(void *a, void *b, float t, void *ret)
{
    double *da = (double*) a, *db = (double*) b, *dret = (double*) ret;
    *dret = *da * (1 - t) + *db * t;
}

void int_linear_interpolate(void *a, void *b, float t, void *ret)
{
    int *ia = (int*) a, *ib = (int*) b, *iret = (int*) ret;
    *iret = (int) (*ia * (1 - t)) + (*ib * t);
}

void float_ternary_linear_interpolate(void *a, void *b, void *c, struct fvec4 t, void *ret)
{
    float *fa = (float*) a, *fb = (float*) b, *fc = (float*) c, *fret = (float*) ret;
    *fret = (float) (*fa * t.components.x + *fb * t.components.y + *fc * t.components.z);
}

void double_ternary_linear_interpolate(void *a, void *b, void *c, struct fvec4 t, void *ret)
{
    double *da = (double*) a, *db = (double*) b, *dc = (double*) c, *dret = (double*) ret;
    *dret = (double) (*da * t.components.x + *db * t.components.y + *dc * t.components.z);
}

void int_ternary_linear_interpolate(void *a, void *b, void *c, struct fvec4 t, void *ret)
{
    int *ia = (int*) a, *ib = (int*) b, *ic = (int*) c, *iret = (int*) ret;
    *iret = (int) (*ia * t.components.x + *ib * t.components.y + *ic * t.components.z);
}