#include "gfx/image.h"

#include <string.h>

struct pixel rgb_pix(unsigned char red, unsigned char green, unsigned char blue)
{
    return (struct pixel) { red, green, blue };
}

struct image image_init(size_t width, size_t height)
{
    struct image img;
    img.width = width;
    img.height = height;
    img.pixels = (struct pixel* ) calloc(width * height, sizeof(struct pixel));
    return img;
}

void image_free(struct image* img)
{
    img->width = 0;
    img->height = 0;
    free(img->pixels);
}

void image_clear(struct image* img)
{
    memset(img->pixels, 0, img->width * img->height * sizeof(struct pixel));
}   

depth_buffer_t depth_buffer_new(struct image* img)
{
    return (depth_buffer_t) calloc(img->width * img->height, sizeof(depth_t));
}

void depth_buffer_free(depth_buffer_t zbuffer)
{
    free(zbuffer);
}

void depth_buffer_clear(struct image *img, depth_buffer_t zbuffer)
{
    memset(zbuffer, 0, img->width * img->height * sizeof(depth_t));
}

static size_t conv2index(size_t x, size_t y, size_t width)
{
    return (y & -2) * width + (x << 1) + (y & 1);
}

void image_draw(struct image *img, size_t x, size_t y, struct pixel rgb)
{
    if (x < img->width && y < img->height) 
        // img->pixels[img->width * y + x] = rgb;
        img->pixels[conv2index(x, y, img->width)] = rgb;
}

void image_depth_draw(struct image* img, depth_buffer_t zbuffer, size_t x, size_t y, size_t depth, struct pixel rgb)
{
    if (x < img->width && y < img->height && depth >= zbuffer[img->width * y + x]) 
    {
        // img->pixels[img->width * y + x] = rgb;
        img->pixels[conv2index(x, y, img->width)] = rgb;
        zbuffer[img->width * y + x] = depth;
    }
}

struct pixel image_get_pixel(struct image* img, size_t x, size_t y)
{
    return img->pixels[conv2index(x, y, img->width)];
}

depth_t image_get_depth(struct image* img, depth_buffer_t zbuffer, size_t x, size_t y)
{
    return zbuffer[y * img->width + x];
}