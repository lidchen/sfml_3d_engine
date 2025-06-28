#ifndef BASIC_TYPE
#define BASIC_TYPE
#include "my_la.h"
#include <fstream>
#include <sstream>
#include <iostream>

struct Triangle {
    Vec3 p[3];
    float light_strength;

    Triangle(const Vec3& a, const Vec3& b, const Vec3& c);
    Triangle();
    Vec3 cal_norm_vec();
    float get_middle_z() const;
    void set_color(float light_strength);
    void print();
    std::string to_string() const;
    friend std::ostream& operator<<(std::ostream& os, const Triangle& tri);
};

struct Mesh {
    std::vector<Triangle> tris_to_raster;
    Mesh();
    Mesh(const std::vector<Triangle> &t);
    bool load_from_obj(const std::string& file_location);
};

// Plane can be expressed by <N, P> + D = 0
// struct Plane {
//     Vec3 N, P;
//     float D;
// };

void rotate_vec2(float rad, Vec2& vec);

void draw_vec2(const Vec2& vec);

void draw_vec3(const Vec3& vec);

// void draw_tri(const Triangle t); 

Mat4 rotation_matrix(Vec3 u, float theta);

void input_handle();

#endif