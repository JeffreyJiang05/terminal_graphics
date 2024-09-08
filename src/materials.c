#include "gfx/lighting/materials.h"

static struct material g_material = {
    .ambient = { .buffer = { 0, 0, 0, 0 } },
    .diffuse = { .buffer = { 0, 0, 0, 0 } },
    .specular = { .buffer = { 0, 0, 0, 0 } },
    .shininess = 8.0
};

struct material get_material()
{
    return g_material;
}

void set_material(struct material material)
{
    g_material = material;
}