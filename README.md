# raylib4Vita samples
raylib samples for PlayStation Vita

For compiling each sample
```
cd [sample]
mkdir build
cd build
cmake ..
make
```
For udp logs you can use debug.sh script with the content
```
echo [HOST] debugnet listener up
echo [HOST] ready to have a lot of fun!!!
socat udp-recv:18194 stdout
```
## EXAMPLES LIST

### category: shapes

Examples using raylib shapes drawing functionality, provided by raylib.

| ## | example  | image  | developer  | new |
|----|----------|--------|:----------:|:---:|
| 25 | [shapes_basic_shapes](shapes/shapes_basic_shapes/main.c)                     | <img src="shapes/shapes_basic_shapes/shapes_basic_shapes.png" alt="shapes_basic_shapes" width="200">                     | ray                                        | ⭐️     |
| 26 | [shapes_logo_raylib](shapes/shapes_logo_raylib/main.c)                         | <img src="shapes/shapes_logo_raylib/shapes_logo_raylib.png" alt="shapes_logo_raylib" width="200">                         | ray                                        |        |
| 27 | [shapes_logo_raylib_anim](shapes/shapes_logo_raylib_anim/main.c)               | <img src="shapes/shapes_logo_raylib_anim/shapes_logo_raylib_anim.png" alt="shapes_logo_raylib_anim" width="200">               | ray                                        |        |
| 30 | [shapes_collision_area](shapes/shapes_collision_area/main.c)                   | <img src="shapes/shapes_collision_area/shapes_collision_area.png" alt="shapes_collision_area" width="200">                   | ray                                        | ⭐️     |


### category: textures

Examples using raylib textures functionality, including image/textures loading/generation and drawing, provided by raylib 

| ## | example  | image  | developer  | new |
|----|----------|--------|:----------:|:---:|
| 40 | [textures_logo_raylib](textures/textures_logo_raylib/main.c)                   | <img src="textures/textures_logo_raylib/textures_logo_raylib.png" alt="textures_logo_raylib" width="200">                   | ray                                              |        |
| 44 | [textures_image_generation](textures/textures_image_generation/main.c) | <img src="textures/textures_image_generation/textures_image_generation.png" alt="textures_image_generation" width="200"> | ray                                              | ⭐️     |

### category: models

Examples using raylib models functionality, including models loading/generation and drawing

| ## | example  | image  | developer  | new |
|----|----------|--------|:----------:|:---:|
| 71 | [models_box_collisions](models/models_box_collisions/main.c)                   | <img src="models/models_box_collisions/models_box_collisions.png" alt="models_box_collisions" width="200">                   | ray                                              |        |
| 72 | [models_cubicmap](models/models_cubicmap/main.c)                               | <img src="models/models_cubicmap/models_cubicmap.png" alt="models_cubicmap" width="200">                               | ray                                              |
| 76 | [models_mesh_generation](models/models_mesh_generation/main.c)                 | <img src="models/models_mesh_generation/models_mesh_generation.png" alt="models_mesh_generation" width="200">                 | ray                                              |        |

As always contributions are welcome, feel free to send new examples! Here it is an [examples template](examples_template.c) to start with!
