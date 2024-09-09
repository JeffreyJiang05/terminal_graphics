# Terminal Graphics

Created by Jeffrey Jiang

## Description

This is a rudimentary graphics engine for modern terminals in Linux. 

This engine takes inspiration from the OpenGL pipeline and imitates some of the most crucial stages in the pipeline such as Vertex Array Object (`struct vertex_buffer`), Buffer Array Object (`struct index_buffer`), Z buffer (`depth_buffer_t`), vertex shaders, and fragment shaders. 

The images are outputted directly to the terminal. This allows for 
functionality such as keyboard and mouse inputs. This library also
provides a camera which can be moved throughout the scene.

The engine provides many basic 3D shapes such as a sphere, torus,
etc. 

Additionally, this engine implements some basic shaders including
one that implements the Phong lighting model. 

## The API

### The Core API

- Image type (`struct image`) - Stores the pixels of the image.
  - Initializes image (`image_init`) - Initializes the image to provided dimensions.
  - Frees the image (`image_free`) - Frees the memory allocated for the image.
- Vertex Buffer Object (`struct vertex_buffer`) - Stores the vertices of a 3D shape.
  - Initializes VBO (`vertex_buffer_init`)
  - Apply linear transformation (`vertex_buffer_apply_transformation`) - Applies a linear transformation and stores in a new VBO
  - Add a vertex to buffer (`vertex_buffer_add`) - Adds a vertex to the VBO
  - Frees VBO (`vertex_buffer_free`) - Frees memory allocated for the VBO
- Index Buffer Object (`struct index_buffer`) - Stores the indexes for the triangles that make up the surface of the 3D shape in its corresponding `struct vertex_buffer`.
  - Initializes IBO (`index_buffer_init`)
  - Adds a index of a triangle (`index_buffer_add`)
  - Frees IBO (`index_buffer_free`) - Frees memory allocated for the IBO
- Construct Data Object (`struct construct_data`) - Stores information such as the texture mapping and the normals of the 3D shape.
  - Initializes the construct data (`construct_data_init`)
  - Frees the construct data (`construct_data_free`) - Frees memory allocated for the construct data.
- Depth Buffer (`depth_buffer_t`) - Stores depth value for each pixel
  - Initialize a new depth_buffer (`depth_buffer_new`)
  - Frees the depth buffer (`depth_buffer_free`) - Frees memory allocated for the depth buffer
- Perform perspective divide (`perspective_divide`) - Crucial if a perspective matrix is used.
- Draw a wireframe (`draw_wire_frame_mesh`)
- Draw a filled mesh (`draw_filled_mesh`)
- Terminal Image type (`struct terminal_image`) - Stores the characters for terminal output.
  - Convert image to terminal image (`conv_to_terminal_image`) - Converts an image to a terminal image.
  - Print the terminal image (`print_terminal_image`) - Outputs the frame to the terminal.
  - Free terminal image (`free_terminal_image`) - Frees the memory for the terminal image.
- Set the view position (`set_view_pos`) - Important for backface culling and more.

The drawing functions take arguments which include an optional Z buffer, an interpolation function for the data, a fragment shader function, and
a function to filter the triangles being drawn (this allows for backface culling). 

### Vectors, Matrices, Linear Transformations

- Floating point vector with four components (`fvec4`) 
  - Construct (`fvec4_load4f`)
  - Add two vectors (`fvec4_add`)
  - Subtract two vectors (`fvec4_sub`)
  - Componentwise multiply two vectors (`fvec4_mult`)
  - Componentwise divide two vectors (`fvec4_div`)
  - Scale a vector (`fvec4_scale`)
  - Calculate magnitude (`fvec4_magnitude`)
  - Normalize a vector (`fvec4_normalize`)
  - Cross product of two vectors (`fvec4_cross`)
  - Dot product of two vectors (`fvec4_dot`)
  - LERP between two vectors (`fvec4_linear_interpolate`)
  - Ternary interpolate between three vectors (`fvec4_ternary_linear_interpolate`)
- 4x4 floating point matrix
  - Construct from vectors (`fmat4x4_load4v`)
  - Multiplicative identity (`fmat4x4_identity`)
  - Add two matrices (`fmat4x4_add`)
  - Subtract two matrices (`fmat4x4_sub`)
  - Multiply two matrices (`fmat4x4_mult`)
  - Multiply a matrix with vector (`fmat4x4_matvec_mult`)
  - Scale a matrix (`fmat4x4_scale`)
  - Transpose a matrix (`fmat4x4_transpose`)
  - Invert a matrix (`fmat4x4_inv`)
- Linear Transformations
  - Scale in the x,y dimension (`scale_2D`)
  - Translate in the x,y dimension (`translate_2D`)
  - Rotate in the x,y dimension (`rotate_2D`)
  - Shear in the x,y dimension (`shear_2D`)
  - Scale in the x,y,z dimension (`scale_3D`)
  - Translate in the x,y,z dimension (`translate_3D`)
  - Rotate in the x,y,z dimension (`rotate_3D`)
  - Create perspective matrix (`perspective`)
  - Create look at matrix (`lookat`)

### Terminal

- Type representing settings for the terminal (`struct terminal`)
- Initialize terminal with appropriate settings (`terminal_init`)
  - Settings include `TERMINAL_DEFAULT`, `TERMINAL_MAX_SCREEN_SIZE`, `TERMINAL_REGISTER_KEY_EVENTS`, `TERMINAL_REGISTER_MOUSE_EVENTS`
- Returns width of terminal `terminal_width`
- Returns height of terminal `terminal_height`
- Returns the frame count since startup `terminal_frame_count`
- Increment the frame count `terminal_increment_frame`
- Moves cursor to origin `terminal_goto_origin`
- Moves cursor to position `terminal_goto`
- Clears terminal `terminal_clear`
- Hides cursor in terminal `terminal_hide_cursor`
- Shows cursor in terminal `terminal_show_cursor`
- Enables mouse tracking `terminal_track_mouse`
- Process keyboard inputs `terminal_process_key_input`
- Process mouse inputs `terminal_process_mouse_input`
- Process keyboard and mouse inputs `terminal_process_inputs`

### Constructs

- Construct sphere vertices and indices `construct_sphere`
- Construct cube vertices and indices `construct_cube`
- Construct cylindrical vertices and indices `construct_cylindrical`
- Construct prism vertices and indices `construct_prism`
- Construct pyramid vertices and indices `construct_pyramid`
- Construct torus vertices and indices `construct_torus`

### Fly Camera

- Fly camera type (`struct fly_camera_t`)
- Initializes the fly camera (`fly_camera_init`) - Accepts parameters 
- Default initializes the fly camera (`fly_camera_default_init`)
- Gets look at matrix (`fly_camera_get_view_matrix`)
- Get projection matrix (`fly_camera_get_projection_matrix`)
- Process keyboard inputs for camera (`fly_camera_process_keyboard_input`)
- Process mouse inputs for camera (`fly_camera_process_mouse_input`)
- Gets position of camera (`fly_camera_get_position`)
- Set the camera sensitivity (`fly_camera_set_sensitivity`)
- Get the camera sensitivity (`fly_camera_get_sensitivity`)
- Set the camera speed (`fly_camera_set_speed`)
- Get the camera speed (`fly_camera_get_speed`)

### Lighting System

- Get number of point light sources `get_point_light_count`
- Set the point light sources `set_point_lights`
- Get the point light at index `get_point_light`
- Get number of directional light sources `get_directional_light_count`
- Set the directional light sources `set_directional_lights`
- Get the directional light at index `get_directional_light`
- Get the bound material `get_material`
- Bind material `set_material`
- Phong vertex shader `phong_vertex_shader`
- Phong fragment shader `phong_frag_shader`
- Binary interpolate vertex shader data `phong_vshader_data_binary_interpolate`
- Ternary interpolate vertex shader data `phong_vshader_data_ternary_interpolate`

## Limitations

The biggest limitation to this graphics engine is its framerate. Unfortunately,
the terminal renders the characters too slowly. The larger the image, then the
the number of characters which needs to be rendered increases as well which can
tank performance.

## How to Build

## Features to be Implemented 

In order of importance:

1. Texture Mapping (The groundwork is already set up.)
2. Engine Optimizations
3. Mesh Loading
4. World Box