#include "ui/term_im_conv.h"

#include <stdio.h>

// "\033[38;2;000;000;000m\033[48;2;000;000;000m" = 38 + 1 null terminating
// "\033[0m" = 4 + 1 null terminating

static void conv_pixels_to_term_char(struct pixel top, struct pixel bottom, wchar_t *str)
{
    swprintf(str, 40, L"\033[48;2;%03d;%03d;%03dm\033[38;2;%03d;%03d;%03dm▄", top.red, top.green, top.blue, bottom.red, bottom.green, bottom.blue);
}

// line = terminal_img (includes line 2n and 2n+1)
static wchar_t *conv_line_to_wstr(size_t width, struct pixel *line)
{
    wchar_t *wstr = (wchar_t*) malloc((width * 39 + 4) * sizeof(wchar_t));
    wchar_t *tmp = wstr;
    for (size_t i = 0; i < width; ++i, wstr += 39)
    {
        conv_pixels_to_term_char(line[2 * i], line[2 * i + 1], wstr);
    }
    wmemcpy(tmp + width * 39, L"\033[m", 4);

    return tmp;
}

struct terminal_image conv_to_terminal_image(struct image *img)
{
    struct terminal_image res;
    res.height = img->height / 2;
    res.lines = (wchar_t**) malloc(res.height * sizeof(void*));
    for (size_t i = 0; i < res.height; ++i)
    {
        res.lines[i] = conv_line_to_wstr(img->width, img->pixels + img->width * 2 * i);
    }
    return res;
}

void free_terminal_image(struct terminal_image *img)
{
    for (size_t i = 0; i < img->height; ++i)
    {
        free(img->lines[i]);
    }
    free(img->lines);
    img->height = 0;
}

void print_terminal_image(struct terminal_image* img)
{
    for (size_t i = 0; i < img->height; ++i)
    {
        // fputws(img->lines[i], stdout);
        wprintf(img->lines[i]);
        wprintf(L"\n");
    }
}

void scanline_image(struct image* img, size_t term_line, wchar_t* buf)
{
    struct pixel *line = img->pixels + 2 * term_line * img->width;
    for (size_t i = 0; i < img->width; ++i, buf += 39)
    {
        conv_pixels_to_term_char(line[2 * i], line[2 * i + 1], buf);
    }
    wmemcpy(buf, L"\033[0m", 5);
}

struct terminal_image_string image2wstr_conv(struct image *img)
{
    struct terminal_image_string string;
    size_t num_of_lines = img->height / 2;
    size_t line_str_length = img->width * 39 + 4;
    
    string.length = num_of_lines * line_str_length - 1;
    string.buffer = malloc(num_of_lines * line_str_length * sizeof(wchar_t));

    wchar_t *begin = string.buffer;
    for (size_t line = 0; line < num_of_lines; ++line, begin += 4)
    {
        struct pixel *line_pixels = img->pixels + img->width * 2 * line;
        for (size_t i = 0; i < img->width; ++i, begin += 39)
        {
            conv_pixels_to_term_char(line_pixels[2 * i], line_pixels[2 * i + 1], begin);
        }
        wmemcpy(begin, L"\033[m\n", 4);
    }
    string.buffer[string.length] = 0;

    return string;
}


static int test_pixel_eq(struct pixel p0, struct pixel p1)
{
    return p0.red == p1.red && p0.green == p1.green && p0.blue == p1.blue && p0.flags == p1.flags;
}

static struct terminal_image_line make_terminal_image_line(struct image *img, size_t line, struct pixel bg)
{
    struct pixel *img_line = img->pixels + line * 2 * img->width;
    long begin = 0, end = img->width - 1;
    while (test_pixel_eq(img_line[2 * begin], bg) && test_pixel_eq(img_line[2 * begin + 1], bg)) ++begin;
    while (test_pixel_eq(img_line[2 * end], bg) && test_pixel_eq(img_line[2 * end + 1], bg)) --end;

    if (end < begin) return (struct terminal_image_line) { .line = NULL };

    size_t diff = (long) (end - begin + 1);
    struct terminal_image_line ret = { 
        .x = begin + 1,
        .y = line + 1,
        .line = (wchar_t*) malloc((diff * 39 + 5) * sizeof(wchar_t))
    };
    // wprintf(L"(%lu,%lu)\n", ret.x, ret.y);
    wchar_t* buf = ret.line;
    for (long i = begin; i <= end; ++i, buf += 39)
    {
        conv_pixels_to_term_char(img_line[2 * i], img_line[2 * i + 1], buf);
    }
    wmemcpy(ret.line + diff * 39, L"\033[0m", 5);

    return ret;
}

void conv_to_lazy_terminal_image(struct lazy_terminal_image* term_img, struct image *img, struct pixel bg)
{
    term_img->capacity = img->height / 2;
    term_img->size = 0;
    term_img->lines = malloc(term_img->capacity * sizeof(struct terminal_image_line));

    struct terminal_image_line image_line;
    for (size_t i = 0; i < term_img->capacity; ++i)
    {
        image_line = make_terminal_image_line(img, i, bg);
        if (image_line.line) term_img->lines[term_img->size++] = image_line;
    }
}

void free_lazy_terminal_image(struct lazy_terminal_image* term_img)
{
    for (size_t i = 0; i < term_img->size; ++i)
    {
        free(term_img->lines[i].line);
    }
    free(term_img->lines);
    term_img->capacity = 0;
    term_img->size = 0;
}

void print_lazy_terminal_image(struct lazy_terminal_image* term_img, size_t x_offset, size_t y_offset)
{
    for (size_t i = 0; i < term_img->size; ++i)
    {
        wprintf(L"\033[%lu;%luH", x_offset + term_img->lines[i].y, y_offset + term_img->lines[i].x);
        fputws(term_img->lines[i].line, stdout);
    }
}