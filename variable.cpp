#include "variable.h"
bool over = false;
bool debug_view_on = true;
bool enable_debug_line = false;
bool enable_rotation = false;
bool enable_mouse_release = true;
sf::RenderWindow window(sf::VideoMode(screenX, screenY), "3d_engine");
sf::Font Alkhemikal;
sf::Text debug_text;

const float k_move_speed = 5.f;
const float k_rotation_speed = 3.f;
const std::string k_model_path = "../res/obj/mountains.obj";
const float k_mouse_sensitivity = 0.003f;

int tri_num;

float fov = 55.f;
float w = screenX;
float h = screenY;
float f = 1000.f;
float n = 0.1f;
float displacement = 0.0f;

float aspect_ratio = w / h;
float fov_radius = 1.0f / tanf(fov * 0.5f / 180.f * M_PI);
float t = tanf(fov_radius) * n;
float r = t * aspect_ratio;
float offset = 0.1f;
float fps;
float dt;

// Vec3 light_dir(0.f, 0.f, -1.f);
Vec3 light_dir(0.f, 1.f, 0.f);

// Transform to NDC(Normailzed Device Coordinates)
Mat4 projection_mat(
    n/r, 0.f, 0.f, 0.f,
    0, n/t, 0, 0,
    0, 0, -(f+n)/(f-n), -2*f*n/(f-n),
    0, 0, 1, 0
);

Mat4 camera_mat;
Mat4 view_mat;
// Mat4 camera_mat (
//     1.f, 0, 0, 0,
//     0, 1.f, 0, 0,
//     0, 0, 1.f, 3.f,
//     0, 0, 0, 1.f
// );

Vec3 world_up(0, 1, 0);
Vec3 camera_dir(0.f, 0.f, -1.f); // Along z-axis
Vec3 camera_vec(-10.f, 5.f, 30.f); 
Vec3 camera_right = (rotation_matrix(Vec3(0.f, -1.f, 0.f), M_PI / 2) * camera_dir.to_vec4()).to_vec3().normalize();
Vec3 camera_up = -camera_dir.cross_product(camera_right).normalize();

Mesh mesh_cube;
// Mesh tri_for_test(
//     std::vector<Triangle>{Triangle(Vec3(0, 0, 0), Vec3(5.f, 5.f, 5.f), Vec3(10.f, 0, 0))}
// );
Mesh tri_for_test(
    // std::vector<Triangle>{Triangle(Vec3(0, 5.f, 5.f), Vec3(10.f, 5.f, 5.f), Vec3(5.f, 0.f, 0.f))}
    std::vector<Triangle>{Triangle(Vec3(0.f, 0.f, 0.f), Vec3(-10.f, 5.f, 5.f), Vec3(5.f, 5.f, 5.f))}
    // std::vector<Triangle>{
    //     Triangle(
    //         Vec3(0.f, 0.f, 0.f),
    //         Vec3(5.f, 5.f, 5.f),
    //         // Vec3(-5.89593f, 2.94796f, 2.94796f)
    //         Vec3(-5.40919, 2.7046, 2.7046)
    //     ),
    //     Triangle(
    //         Vec3(5.f, 5.f, 5.f),
    //         // Vec3(-6.94035f, 5.f, 5.f),
    //         Vec3(-5.68783f, 5.f, 5.f),
    //         // Vec3(-5.89593f, 2.94796f, 2.94796f)
    //         Vec3(-5.40919, 2.7046, 2.7046)
    //     )
    // }

    // std::vector<Triangle>{
    //     Triangle(Vec3(-10.f, 5.f, 5.f), Vec3(5.f, 5.f, 5.f), Vec3(-1.32458f, 0.662288f, 0.662288f)),
    //     Triangle(Vec3(5.f, 5.f, 5.f), Vec3(0.662288f, 0.662288f, 0.662288f), Vec3(-1.32458f, 0.662288f, 0.662288f))
    // }
);

// Mesh mesh_cube {
//     std::vector<Triangle> {
//         // South face
//         {{0, 0, 0}, {0, 1, 0}, {1, 1, 0}},
//         {{0, 0, 0}, {1, 1, 0}, {1, 0, 0}},
//         // East face
//         {{1, 0, 0}, {1, 1, 0}, {1, 1, 1}},
//         {{1, 0, 0}, {1, 1, 1}, {1, 0, 1}},
//         // North face
//         {{1, 0, 1}, {1, 1, 1}, {0, 1, 1}},
//         {{1, 0, 1}, {0, 1, 1}, {0, 0, 1}},
//         // West face
//         {{0, 0, 1}, {0, 1, 1}, {0, 1, 0}},
//         {{0, 0, 1}, {0, 1, 0}, {0, 0, 0}},
//         // Top face
//         {{0, 1, 0}, {0, 1, 1}, {1, 1, 1}},
//         {{0, 1, 0}, {1, 1, 1}, {1, 1, 0}},
//         // Bottom face
//         {{1, 0, 1}, {0, 0, 1}, {0, 0, 0}},
//         {{1, 0, 1}, {0, 0, 0}, {1, 0, 0}}
//     }
// };
