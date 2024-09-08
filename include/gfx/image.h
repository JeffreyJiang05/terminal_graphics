#ifndef GFX_IMAGE_H
#define GFX_IMAGE_H

#include <stdlib.h>
#include <assert.h>

struct pixel
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char flags;
};

static_assert(sizeof(struct pixel) == 4, "Incorrect size of pixel");

struct pixel rgb_pix(unsigned char red, unsigned char green, unsigned char blue);

struct image
{
    size_t width;
    size_t height;
    struct pixel *pixels;
};

typedef size_t depth_t;
typedef depth_t* depth_buffer_t;

struct image image_init(size_t width, size_t height);

void image_free(struct image* img);

void image_clear(struct image* img);

depth_buffer_t depth_buffer_new(struct image* img);

void depth_buffer_free(depth_buffer_t zbuffer);

void depth_buffer_clear(struct image* img, depth_buffer_t zbuffer);

void image_draw(struct image *img, size_t x, size_t y, struct pixel rgb);

void image_depth_draw(struct image* img, depth_buffer_t zbuffer, size_t x, size_t y, size_t depth, struct pixel rgb);

struct pixel image_get_pixel(struct image* img, size_t x, size_t y);

depth_t image_get_depth(struct image* img, depth_buffer_t zbuffer, size_t x, size_t y);

#endif