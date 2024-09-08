#include "gfx/cameras/flycamera.h"

#include <math.h>
#include <wchar.h>

#include "math/transformations.h"
#include "ui/terminal.h"

#define DEFAULT_YAW -90.0f
#define DEFAULT_PITCH 0.0f
#define DEFAULT_SPEED 0.05f
#define DEFAULT_SENSITIVITY 1.0f
#define DEFAULT_ZOOM 45.0f

static struct fly_camera_t fly_camera;

static float radians(float degrees)
{
    return degrees * M_PI * (1.0 / 180);
}

static void update_fly_camera_vectors()
{
    fly_camera.front = fvec4_normalize(
        (struct fvec4) { .buffer = {
            cosf(radians(fly_camera.yaw)) * cosf(radians(fly_camera.pitch)),
            sinf(radians(fly_camera.pitch)),
            sinf(radians(fly_camera.yaw)) * cosf(radians(fly_camera.pitch)),
            0
        }}
    );
    fly_camera.right = fvec4_normalize(fvec4_cross(fly_camera.front, fly_camera.world_up));
    fly_camera.up = fvec4_normalize(fvec4_cross(fly_camera.right, fly_camera.front));
}

void fly_camera_init(struct fvec4 position, struct fvec4 world_up, float yaw, float pitch)
{
    fly_camera.position = position;
    fly_camera.world_up = world_up;
    fly_camera.yaw = yaw;
    fly_camera.pitch = pitch;
    fly_camera.movement_speed = DEFAULT_SPEED;
    fly_camera.zoom = DEFAULT_ZOOM;
    fly_camera.sensitivity = DEFAULT_SENSITIVITY;

    update_fly_camera_vectors();
}

void fly_camera_default_init()
{
    fly_camera_init(
        (struct fvec4) { .buffer = { 0.0f, 0.0f, 0.0f, 0.0f } },
        (struct fvec4) { .buffer = { 0.0f, 1.0f, 0.0f, 0.0f } },
        DEFAULT_YAW,
        DEFAULT_PITCH
    );
}

struct fmat4x4 fly_camera_get_view_matrix()
{
    struct fvec4 target = fvec4_add(fly_camera.position, fly_camera.front);
    return lookat(
        fly_camera.position.components.x, fly_camera.position.components.y, fly_camera.position.components.z,
        target.components.x, target.components.y, target.components.z,
        fly_camera.up.components.x, fly_camera.up.components.y, fly_camera.up.components.z
    );
}

struct fmat4x4 fly_camera_get_projection_matrix(struct image *img)
{
    return perspective(radians(fly_camera.zoom), (float) img->width / img->height, 0.1f, 100.0f);
}

void fly_camera_process_keyboard_input(int input)
{
    switch (input)
    {
        case KEY_UP:
        case KEY_LOWERCASE_W:
        {
            fly_camera.position = fvec4_sub(fly_camera.position, fvec4_scale(fly_camera.front, fly_camera.movement_speed));
            break;
        }
        case KEY_DOWN:
        case KEY_LOWERCASE_S:
        {
            fly_camera.position = fvec4_add(fly_camera.position, fvec4_scale(fly_camera.front, fly_camera.movement_speed));
            break;
        }
        case KEY_LEFT:
        case KEY_LOWERCASE_A:
        {
            fly_camera.position = fvec4_add(fly_camera.position, fvec4_scale(fly_camera.right, fly_camera.movement_speed));
            break;
        }
        case KEY_RIGHT:
        case KEY_LOWERCASE_D:
        {
            fly_camera.position = fvec4_sub(fly_camera.position, fvec4_scale(fly_camera.right, fly_camera.movement_speed));
            break;
        }
    }
}

void fly_camera_process_mouse_input(enum mouse_event_id id, size_t x, size_t y)
{
    static int initialized = 0;
    static long int last_x, last_y;

    if (id == MOUSE_MOVE) {
        if (!initialized)
        {
            last_x = x; //  terminal.window_size.ws_row / 2;
            last_y = y; //  terminal.window_size.ws_col / 2;
            initialized = 1;
        }

        float x_offset = (float) ((long int) x - last_x);
        float y_offset = (float) (last_y - (long int) y);
        last_x = x;
        last_y = y;

        x_offset *= fly_camera.sensitivity;
        y_offset *= fly_camera.sensitivity;

        fly_camera.yaw -= x_offset;
        fly_camera.pitch -= y_offset;

        if (fly_camera.pitch > 89.0f) fly_camera.pitch = 89.0f;
        if (fly_camera.pitch < -89.0f) fly_camera.pitch = -89.0f;

        update_fly_camera_vectors();
    }
}

struct fvec4 fly_camera_get_position()
{
    return fly_camera.position;
}

struct fvec4 fly_camera_get_world_up()
{
    return fly_camera.world_up;
}

struct fvec4 fly_camera_get_front()
{
    return fly_camera.front;
}

struct fvec4 fly_camera_get_up()
{
    return fly_camera.up;
}

struct fvec4 fly_camera_get_right()
{
    return fly_camera.right;
}

float fly_camera_get_yaw()
{
    return fly_camera.yaw;
}

float fly_camera_get_pitch()
{
    return fly_camera.pitch;
}

float fly_camera_get_speed()
{
    return fly_camera.movement_speed;
}

float fly_camera_get_sensitivity()
{
    return fly_camera.sensitivity;
}

float fly_camera_get_zoom()
{
    return fly_camera.zoom;
}

void fly_camera_set_sensitivity(float sensitivity)
{
    fly_camera.sensitivity = sensitivity;
}

void fly_camera_set_speed(float movement_speed)
{
    fly_camera.movement_speed = movement_speed;
}