#ifndef VARIABLE
#define VARIABLE

#include "basic_type.h"
#include <SFML/Graphics.hpp>

#define screenX 800
#define screenY 600
extern bool over;
extern bool enable_debug_line;
extern bool enable_rotation;
extern sf::RenderWindow window;
extern sf::Font Alkhemikal;
extern sf::Text debug_text;

extern const float k_move_speed;
extern const float k_rotation_speed;
extern const std::string k_model_path;

extern float fov;
extern float w;
extern float h;
extern float f;
extern float n;
extern float displacement;
extern float aspect_ratio;
extern float fov_radius;
extern float t;
extern float r;
extern float offset;


extern Mat4 projection_mat;
extern Mat4 camera_mat;
extern Mat4 view_mat;
extern Vec3 light_dir;
extern Vec3 camera_vec;
extern Vec3 camera_dir;
extern Mesh mesh_cube;
extern Mesh tri_for_test;

#endif