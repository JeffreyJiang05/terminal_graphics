#ifndef GFX_FLY_CAMERA_H
#define GFX_FLY_CAMERA_H

#define _POSIX_C_SOURCE 202405L

#include <stdlib.h>

#include "math/linalg.h"
#include "gfx/image.h"
#include "ui/terminal.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

struct fly_camera_t
{
    struct fvec4 position;
    struct fvec4 world_up;
    struct fvec4 front;
    struct fvec4 up;
    struct fvec4 right;
    float yaw;
    float pitch;
    float movement_speed;
    float sensitivity;
    float zoom;    
};

void fly_camera_init(struct fvec4 position, struct fvec4 world_up, float yaw, float pitch);

void fly_camera_default_init();

struct fmat4x4 fly_camera_get_view_matrix();

struct fmat4x4 fly_camera_get_projection_matrix(struct image* img);

void fly_camera_process_keyboard_input(int input);

void fly_camera_process_mouse_input(enum mouse_event_id id, size_t x, size_t y);

struct fvec4 fly_camera_get_position();

struct fvec4 fly_camera_get_world_up();

struct fvec4 fly_camera_get_front();

struct fvec4 fly_camera_get_up();

struct fvec4 fly_camera_get_right();

float fly_camera_get_yaw();

float fly_camera_get_pitch();

float fly_camera_get_speed();

float fly_camera_get_sensitivity();

float fly_camera_get_zoom();

void fly_camera_set_sensitivity(float sensitivity);

void fly_camera_set_speed(float movement_speed);

#endif