#ifndef UTIL3D
#define UTIL3D
#include "basic_type.h"

Mat4 rotation_matrix(Vec3 u, float theta);
Mat4 pointat_matrix(const Vec3& pos, const Vec3& target, const Vec3& up_origin);
Mat4 quick_inverse_matrix(const Mat4& m);
Vec3 intersection_plane_line(const Vec3& A, const Vec3& B, const Vec3& plane_n, const Vec3& plane_p);

void rotate_vec2(float rad, Vec2& vec);
void cal_circle_info(const std::vector<Triangle>& obj, Vec3& out_center, float& out_radius);
void move_triangle(Triangle& tri, Vec3 dir, float length);
void bomb_affect(std::vector<Triangle>& tris);
float signed_distance_plane_point(const Vec3& plane_n, const Vec3& plane_p, const Vec3& point);

#endif