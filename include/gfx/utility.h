#ifndef GFX_UTILITY_H
#define GFX_UTILITY_H

#include "gfx/image.h"
#include "math/linalg.h"

struct fvec4 convertToNDC(struct image *img, struct fvec4 vec);

depth_t compute_depth(float z);

struct fvec4 perspective_divide(struct fvec4 vec);

#endif