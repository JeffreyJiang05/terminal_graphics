#ifndef TERMINAL_IMAGE_CONVERSION
#define TERMINAL_IMAGE_CONVERSION

#include <stdlib.h>
#include <wchar.h>

#include "gfx/image.h"

struct terminal_image
{
    size_t height;
    wchar_t **lines;
};

struct terminal_image conv_to_terminal_image(struct image *img);

void free_terminal_image(struct terminal_image *img);

void print_terminal_image(struct terminal_image* img);

void scanline_image(struct image* img, size_t term_line, wchar_t* buf);

struct terminal_image_string
{
    size_t length;
    wchar_t *buffer;
};

struct terminal_image_string image2wstr_conv(struct image *img);

struct terminal_image_line
{
    size_t x; // indexed at 1
    size_t y; // indexed at 1
    wchar_t *line;
};

struct lazy_terminal_image
{
    struct terminal_image_line *lines;
    size_t capacity;
    size_t size;
};

void conv_to_lazy_terminal_image(struct lazy_terminal_image* term_img, struct image *img, struct pixel bg);

void free_lazy_terminal_image(struct lazy_terminal_image* term_img);

void print_lazy_terminal_image(struct lazy_terminal_image* term_img, size_t x_offset, size_t y_offset);

#endif