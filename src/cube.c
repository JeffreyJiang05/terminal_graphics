#include "gfx/constructs/cube.h"

void construct_cube(struct vertex_buffer *vertices, struct index_buffer *indices, struct construct_data *data)
{
    vertex_buffer_reserve(vertices, 6 * 6);
    construct_data_reserve(data, 6 * 6);
    index_buffer_reserve(indices, 12);

    index_buffer_add_many(indices, 12,
        (struct triangle_vertex_indices) {  0,  1,  2 },
        (struct triangle_vertex_indices) {  3,  4,  5 },
        (struct triangle_vertex_indices) {  6,  7,  8 },
        (struct triangle_vertex_indices) {  9, 10, 11 },
        (struct triangle_vertex_indices) { 12, 13, 14 },
        (struct triangle_vertex_indices) { 15, 16, 17 },
        (struct triangle_vertex_indices) { 18, 19, 20 },
        (struct triangle_vertex_indices) { 21, 22, 23 },
        (struct triangle_vertex_indices) { 24, 25, 26 },
        (struct triangle_vertex_indices) { 27, 28, 29 },
        (struct triangle_vertex_indices) { 30, 31, 32 },
        (struct triangle_vertex_indices) { 33, 34, 35 }
    );

    vertex_buffer_add_many(vertices, 36,
        FVEC4(  0.5f, -0.5f,  0.5f,  1.0f ),
        FVEC4(  0.5f,  0.5f,  0.5f,  1.0f ),
        FVEC4( -0.5f, -0.5f,  0.5f,  1.0f ),
        FVEC4( -0.5f, -0.5f,  0.5f,  1.0f ),
        FVEC4(  0.5f,  0.5f,  0.5f,  1.0f ),
        FVEC4( -0.5f,  0.5f,  0.5f,  1.0f ),

        FVEC4(  0.5f, -0.5f, -0.5f,  1.0f ),
        FVEC4(  0.5f,  0.5f, -0.5f,  1.0f ),
        FVEC4(  0.5f, -0.5f,  0.5f,  1.0f ),
        FVEC4(  0.5f, -0.5f,  0.5f,  1.0f ),
        FVEC4(  0.5f,  0.5f, -0.5f,  1.0f ),
        FVEC4(  0.5f,  0.5f,  0.5f,  1.0f ),

        FVEC4( -0.5f, -0.5f, -0.5f,  1.0f ),
        FVEC4( -0.5f,  0.5f, -0.5f,  1.0f ),
        FVEC4(  0.5f, -0.5f, -0.5f,  1.0f ),
        FVEC4(  0.5f, -0.5f, -0.5f,  1.0f ),
        FVEC4( -0.5f,  0.5f, -0.5f,  1.0f ),
        FVEC4(  0.5f,  0.5f, -0.5f,  1.0f ),

        FVEC4( -0.5f, -0.5f,  0.5f,  1.0f ),
        FVEC4( -0.5f,  0.5f,  0.5f,  1.0f ),
        FVEC4( -0.5f, -0.5f, -0.5f,  1.0f ),
        FVEC4( -0.5f, -0.5f, -0.5f,  1.0f ),
        FVEC4( -0.5f,  0.5f,  0.5f,  1.0f ),
        FVEC4( -0.5f,  0.5f, -0.5f,  1.0f ),

        FVEC4(  0.5f,  0.5f,  0.5f,  1.0f ),
        FVEC4(  0.5f,  0.5f, -0.5f,  1.0f ),
        FVEC4( -0.5f,  0.5f,  0.5f,  1.0f ),
        FVEC4( -0.5f,  0.5f,  0.5f,  1.0f ),
        FVEC4(  0.5f,  0.5f, -0.5f,  1.0f ),
        FVEC4( -0.5f,  0.5f, -0.5f,  1.0f ),

        FVEC4( -0.5f, -0.5f,  0.5f,  1.0f ),
        FVEC4( -0.5f, -0.5f, -0.5f,  1.0f ),
        FVEC4(  0.5f, -0.5f,  0.5f,  1.0f ),
        FVEC4(  0.5f, -0.5f,  0.5f,  1.0f ),
        FVEC4( -0.5f, -0.5f, -0.5f,  1.0f ),
        FVEC4(  0.5f, -0.5f, -0.5f,  1.0f )
    );

    construct_data_add_many(data, 36,
        CONSTRUCT_DATA(  0.0f,  0.0f,  1.0f,  1.0f,  1.0f ),
        CONSTRUCT_DATA(  0.0f,  0.0f,  1.0f,  1.0f,  0.0f ),
        CONSTRUCT_DATA(  0.0f,  0.0f,  1.0f,  0.0f,  1.0f ),
        CONSTRUCT_DATA(  0.0f,  0.0f,  1.0f,  0.0f,  1.0f ),
        CONSTRUCT_DATA(  0.0f,  0.0f,  1.0f,  1.0f,  0.0f ),
        CONSTRUCT_DATA(  0.0f,  0.0f,  1.0f,  0.0f,  0.0f ),

        CONSTRUCT_DATA(  1.0f,  0.0f,  0.0f,  1.0f,  1.0f ),
        CONSTRUCT_DATA(  1.0f,  0.0f,  0.0f,  1.0f,  0.0f ),
        CONSTRUCT_DATA(  1.0f,  0.0f,  0.0f,  0.0f,  1.0f ),
        CONSTRUCT_DATA(  1.0f,  0.0f,  0.0f,  0.0f,  1.0f ),
        CONSTRUCT_DATA(  1.0f,  0.0f,  0.0f,  1.0f,  0.0f ),
        CONSTRUCT_DATA(  1.0f,  0.0f,  0.0f,  0.0f,  0.0f ),

        CONSTRUCT_DATA(  0.0f,  0.0f, -1.0f,  1.0f,  1.0f ),
        CONSTRUCT_DATA(  0.0f,  0.0f, -1.0f,  1.0f,  0.0f ),
        CONSTRUCT_DATA(  0.0f,  0.0f, -1.0f,  0.0f,  1.0f ),
        CONSTRUCT_DATA(  0.0f,  0.0f, -1.0f,  0.0f,  1.0f ),
        CONSTRUCT_DATA(  0.0f,  0.0f, -1.0f,  1.0f,  0.0f ),
        CONSTRUCT_DATA(  0.0f,  0.0f, -1.0f,  0.0f,  0.0f ),

        CONSTRUCT_DATA( -1.0f,  0.0f,  0.0f,  1.0f,  1.0f ),
        CONSTRUCT_DATA( -1.0f,  0.0f,  0.0f,  1.0f,  0.0f ),
        CONSTRUCT_DATA( -1.0f,  0.0f,  0.0f,  0.0f,  1.0f ),
        CONSTRUCT_DATA( -1.0f,  0.0f,  0.0f,  0.0f,  1.0f ),
        CONSTRUCT_DATA( -1.0f,  0.0f,  0.0f,  1.0f,  0.0f ),
        CONSTRUCT_DATA( -1.0f,  0.0f,  0.0f,  0.0f,  0.0f ),

        CONSTRUCT_DATA(  0.0f,  1.0f,  0.0f,  1.0f,  1.0f ),
        CONSTRUCT_DATA(  0.0f,  1.0f,  0.0f,  1.0f,  0.0f ),
        CONSTRUCT_DATA(  0.0f,  1.0f,  0.0f,  0.0f,  1.0f ),
        CONSTRUCT_DATA(  0.0f,  1.0f,  0.0f,  0.0f,  1.0f ),
        CONSTRUCT_DATA(  0.0f,  1.0f,  0.0f,  1.0f,  0.0f ),
        CONSTRUCT_DATA(  0.0f,  1.0f,  0.0f,  0.0f,  0.0f ),

        CONSTRUCT_DATA(  0.0f, -1.0f, -1.0f,  1.0f,  1.0f ),
        CONSTRUCT_DATA(  0.0f, -1.0f, -1.0f,  1.0f,  0.0f ),
        CONSTRUCT_DATA(  0.0f, -1.0f, -1.0f,  0.0f,  1.0f ),
        CONSTRUCT_DATA(  0.0f, -1.0f, -1.0f,  0.0f,  1.0f ),
        CONSTRUCT_DATA(  0.0f, -1.0f, -1.0f,  1.0f,  0.0f ),
        CONSTRUCT_DATA(  0.0f, -1.0f, -1.0f,  0.0f,  0.0f )
    );

}
