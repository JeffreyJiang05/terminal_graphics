#include "gfx/utility.h"

#include <stdint.h>

struct fvec4 convertToNDC(struct image *img, struct fvec4 vec)
{
    vec.components.x = img->width * vec.components.x * 0.5;
    vec.components.y = img->height * vec.components.y * 0.5;
    return vec;
}

depth_t compute_depth(float z)
{
    return (depth_t) ((z - 1) * SIZE_MAX) / 2;
}

struct fvec4 perspective_divide(struct fvec4 vec)
{
    // wprintf(L"w - %f\n", vec.components.w);
    // wprintf(L"z = %f\n", vec.components.z / vec.components.w);
    vec.components.x /= vec.components.w;
    vec.components.y /= vec.components.w;
    vec.components.z /= vec.components.w;
    return vec;
}

/* struct fvec4 process_transformed_vertex(struct image *img, struct fvec4 vec)
{
    vec.components.x = img->width * (vec.components.x) * 0.5;
    vec.components.y = img->height * (vec.components.y) * 0.5;
    vec.components.w = compute_depth(vec);

    return vec;
} */