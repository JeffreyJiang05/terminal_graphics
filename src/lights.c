#include "gfx/lighting/lights.h"

#include <stdlib.h>
#include <stdarg.h>

struct light_sources_t
{
    size_t point_light_count;
    struct point_light_t *point_lights;
    size_t directional_light_count;
    struct directional_light_t *directional_lights;
};

static struct fvec4 g_view_pos = { .buffer = { 0, 0, 0, 0 } };
static struct light_sources_t g_light_sources = { 0, NULL, 0, NULL };

struct fvec4 get_view_pos()
{
    return g_view_pos;
}

void set_view_pos(struct fvec4 view_pos)
{
    g_view_pos = view_pos;
}

size_t get_point_light_count()
{
    return g_light_sources.point_light_count;
}

void get_point_light(size_t n, struct point_light_t *point_light)
{
    *point_light = g_light_sources.point_lights[n];
}

void set_point_lights(size_t count, ...)
{
    free(g_light_sources.point_lights);
    g_light_sources.point_light_count = count;
    g_light_sources.point_lights = malloc(count * sizeof(struct point_light_t));

    va_list va;
    va_start(va, count);
    for (size_t i = 0; i < count; ++i) g_light_sources.point_lights[i] = va_arg(va, struct point_light_t);
    va_end(va);
}

size_t get_directional_light_count()
{
    return g_light_sources.directional_light_count;
}

void get_directional_light(size_t n, struct directional_light_t *directional_light)
{
    *directional_light = g_light_sources.directional_lights[n];
}

void set_directional_lights(size_t count, ...)
{
    free(g_light_sources.directional_lights);
    g_light_sources.directional_light_count = count;
    g_light_sources.directional_lights = malloc(count * sizeof(struct directional_light_t));

    va_list va;
    va_start(va, count);
    for (size_t i = 0; i < count; ++i) g_light_sources.directional_lights[i] = va_arg(va, struct directional_light_t);
    va_end(va);
}