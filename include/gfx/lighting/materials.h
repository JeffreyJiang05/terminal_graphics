#ifndef GFX_MATERIALS_H
#define GFX_MATERIALS_H

#include "math/linalg.h"

struct material
{
    struct fvec4 ambient;
    struct fvec4 diffuse;
    struct fvec4 specular;
    float shininess;
};

struct material get_material();

void set_material(struct material material);

#endif