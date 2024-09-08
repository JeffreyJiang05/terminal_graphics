#include "math/transformations.h"

#include "math.h"

struct fmat4x4 scale_2D(float x, float y)
{
    return fmat4x4_load4v(
        (struct fvec4) { .buffer = { x, 0, 0, 0 } },
        (struct fvec4) { .buffer = { 0, y, 0, 0 } },
        (struct fvec4) { .buffer = { 0, 0, 1, 0 } },
        (struct fvec4) { .buffer = { 0, 0, 0, 1 } }
    );
}

struct fmat4x4 translate_2D(float x, float y)
{
    return fmat4x4_load4v(
        (struct fvec4) { .buffer = { 1, 0, 0, 0 } },
        (struct fvec4) { .buffer = { 0, 1, 0, 0 } },
        (struct fvec4) { .buffer = { 0, 0, 1, 0 } },
        (struct fvec4) { .buffer = { x, y, 0, 1 } }
    );
}

struct fmat4x4 rotate_2D(float angle)
{
    return fmat4x4_load4v(
        (struct fvec4) { .buffer = { cos(angle), sin(angle), 0, 0 } },
        (struct fvec4) { .buffer = { -sin(angle), cos(angle), 0, 0 } },
        (struct fvec4) { .buffer = { 0, 0, 1, 0 } },
        (struct fvec4) { .buffer = { 0, 0, 0, 1 } }
    );
}

struct fmat4x4 shear_2D(float x, float y)
{
    return fmat4x4_load4v(
        (struct fvec4) { .buffer = { 1, x, 0, 0 } },
        (struct fvec4) { .buffer = { y, 1, 0, 0 } },
        (struct fvec4) { .buffer = { 0, 0, 1, 0 } },
        (struct fvec4) { .buffer = { 0, 0, 0, 1 } }
    );
}

struct fmat4x4 scale_3D(float x, float y, float z)
{
    return fmat4x4_load4v(
        (struct fvec4) { .buffer = { x, 0, 0, 0 } },
        (struct fvec4) { .buffer = { 0, y, 0, 0 } },
        (struct fvec4) { .buffer = { 0, 0, z, 0 } },
        (struct fvec4) { .buffer = { 0, 0, 0, 1 } }
    );
}

struct fmat4x4 translate_3D(float x, float y, float z)
{
    return fmat4x4_load4v(
        (struct fvec4) { .buffer = { 1, 0, 0, 0 } },
        (struct fvec4) { .buffer = { 0, 1, 0, 0 } },
        (struct fvec4) { .buffer = { 0, 0, 1, 0 } },
        (struct fvec4) { .buffer = { x, y, z, 1 } }
    );
}

struct fmat4x4 rotate_3D(float axis_x, float axis_y, float axis_z, float angle)
{

    struct fvec4 axis = fvec4_normalize((struct fvec4) { .buffer = { axis_x, axis_y, axis_z, 0 } });
    float s = sin(angle);
    float c = cos(angle);
    struct fvec4 tmp = fvec4_scale(axis, 1 - c);
    return fmat4x4_load4v(
        (struct fvec4) { .buffer = { 
            tmp.buffer[0] * axis.components.x + c, 
            tmp.buffer[0] * axis.components.y + s * axis.components.z, 
            tmp.buffer[0] * axis.components.z - s * axis.components.y, 
            0 
        } },
        (struct fvec4) { .buffer = { 
            tmp.buffer[1] * axis.components.x - s * axis.components.z, 
            tmp.buffer[1] * axis.components.y + c, 
            tmp.buffer[1] * axis.components.z + s * axis.components.x, 
            0 
        } },
        (struct fvec4) { .buffer = { 
            tmp.buffer[2] * axis.components.x + s * axis.components.y, 
            tmp.buffer[2] * axis.components.y - s * axis.components.x,
            tmp.buffer[2] * axis.components.z + c, 
            0 
        } },
        (struct fvec4) { .buffer = { 0, 0, 0, 1 } }
    );
}

struct fmat4x4 perspective(float angle, float aspect_ratio, float near, float far)
{
    float fov = 1 / tan(angle / 2);
    return fmat4x4_load4v(
        (struct fvec4) { .buffer = { fov * (1 / aspect_ratio), 0, 0, 0 } },
        (struct fvec4) { .buffer = { 0, fov, 0, 0 } },
        (struct fvec4) { .buffer = { 0, 0, (far + near) / (near - far), -1 } },
        (struct fvec4) { .buffer = { 0, 0, 2 * far * near / (near - far), 0 } }
    );
}

struct fmat4x4 lookat(float cam_x, float cam_y, float cam_z,
                      float target_x, float target_y, float target_z,
                      float up_x, float up_y, float up_z)
{
    struct fvec4 camera_pos = fvec4_load4f(cam_x, cam_y, cam_z, 0);
    struct fvec4 camera_direction = fvec4_normalize(fvec4_sub(
        fvec4_load4f(target_x, target_y, target_z, 0), camera_pos
    ));
    struct fvec4 camera_right = fvec4_normalize(fvec4_cross(
        camera_direction, fvec4_load4f(up_x, up_y, up_z, 0)
    ));
    struct fvec4 camera_up = fvec4_cross(
        camera_right, camera_direction
    );

    return fmat4x4_load4v(
        (struct fvec4) { .buffer = { 
            camera_right.components.x, 
            camera_up.components.x, 
            -camera_direction.components.x, 
            0 
        } },
        (struct fvec4) { .buffer = { 
            camera_right.components.y, 
            camera_up.components.y, 
            -camera_direction.components.y, 
            0 
        } },
        (struct fvec4) { .buffer = {
            camera_right.components.z, 
            camera_up.components.z, 
            -camera_direction.components.z, 
            0 
        } },
        (struct fvec4) { .buffer = { 
            -fvec4_dot(camera_pos, camera_right),
            -fvec4_dot(camera_pos, camera_up),
            fvec4_dot(camera_pos, camera_direction),
            1
        } }
    ); 
}