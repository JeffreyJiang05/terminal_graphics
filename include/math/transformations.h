#ifndef MATH_LINEAR_TRANSFORMATIONS_STRUCTURES_H
#define MATH_LINEAR_TRANSFORMATIONS_STRUCTURES_H

#include "math/linalg.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

struct fmat4x4 scale_2D(float x, float y);

struct fmat4x4 translate_2D(float x, float y);

struct fmat4x4 rotate_2D(float angle);

struct fmat4x4 shear_2D(float x, float y);

struct fmat4x4 scale_3D(float x, float y, float z);

struct fmat4x4 translate_3D(float x, float y, float z);

struct fmat4x4 rotate_3D(float axis_x, float axis_y, float axis_z, float angle);

struct fmat4x4 perspective(float angle, float aspect_ratio, float near, float far);

struct fmat4x4 lookat(float cam_x, float cam_y, float cam_z,
                      float target_x, float target_y, float target_z,
                      float up_x, float up_y, float up_z);

#endif