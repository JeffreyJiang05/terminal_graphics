#ifndef GFX_DRAW_H
#define GFX_DRAW_H

#include <stdlib.h>

#include "gfx/image.h"
#include "gfx/vertex_buffer.h"

typedef void*(*vertex_shader_t)(struct fvec4 pos, void *data);
typedef struct pixel(*fragment_shader_t)(struct fvec4 pos, void *data);

// interpolation function on return type of vertex_shader
typedef void(*binary_interpolation_func_t)(void *d0, void *d1, float t, void *d_ret);
typedef void(*ternary_interpolation_func_t)(void *d0, void *d1, void *d2, struct fvec4 t, void *d_ret);

typedef int(*cull_func_t)(struct fvec4 v0, void *d0, struct fvec4 v1, void *d1, struct fvec4 v2, void *d2, void *context);

void draw_line_solid(struct image *img, struct fvec4 p0, struct fvec4 p1, struct pixel pix);

/**
 * not using depth buffer iff zbuffer == NULL, then get_depth == NULL
 * if depth buffer, depth is specified in the type of p0 and p1 and is retrieved via get_depth function
*/
void draw_line(struct image *img, depth_buffer_t zbuffer, struct fvec4 p0, struct fvec4 p1, void *d0, void *d1, 
               size_t data_size, binary_interpolation_func_t interpolate, fragment_shader_t shade);

// -------------------------------------------------------------------------------------------- //


void draw_triangle_wire_frame(struct image *img, depth_buffer_t zbuffer, struct fvec4 p0, void *d0, struct fvec4 p1, void *d1,
                              struct fvec4 p2, void *d2, size_t input_data_size, binary_interpolation_func_t interpolate, fragment_shader_t shade);

void draw_wire_frame_mesh(struct image *img, depth_buffer_t zbuffer, struct vertex_buffer *vertices, struct index_buffer *indices, 
                          void *data, size_t data_size, binary_interpolation_func_t interpolate, fragment_shader_t shade, cull_func_t cull, void *cull_context);

void draw_triangle_filled(struct image* img, depth_buffer_t zbuffer, struct fvec4 p0, void *d0, struct fvec4 p1, void *d1,
                          struct fvec4 p2, void *d2, size_t input_data_size, ternary_interpolation_func_t interpolate, fragment_shader_t shade);

void draw_filled_mesh(struct image *img, depth_buffer_t zbuffer, struct vertex_buffer *vertices, struct index_buffer *indices,
                      void *data, size_t data_size, ternary_interpolation_func_t interpolate, fragment_shader_t shade, cull_func_t cull, void *cull_context);


// vertex buffer = ..., index buffer  = ..., data = ...
// draw_mesh(img, zbuffer, vertex buffer, index buffer, data, vertex_shader, ternary_interpolation_func, fragment_shader)

// vertex_shader(fvec4, void*) -> void*
// fragment_shader(void*) -> pixel

// draw_wireframe(img, zbuffer, vertex buffer, index buffer, data, vertex_shader, binary_interpolation_func, fragment_shader)


#endif