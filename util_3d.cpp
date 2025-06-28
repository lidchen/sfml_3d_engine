#include "util_3d.h"

void move_triangle(Triangle& tri, Vec3 dir, float length) {
    tri.p[0] = tri.p[0] + dir * length;
    tri.p[1] = tri.p[1] + dir * length;
    tri.p[2] = tri.p[2] + dir * length;
}

Mat4 rotation_matrix(Vec3 u, float theta) {
    float c = cos(theta);
    float s = sin(theta);
    float one_minus_c = 1.0f - c;
    
    float x = u.x, y = u.y, z = u.z;

    return Mat4(
        c + x*x*one_minus_c,     x*y*one_minus_c - z*s, x*z*one_minus_c + y*s, 0,
        y*x*one_minus_c + z*s,   c + y*y*one_minus_c,   y*z*one_minus_c - x*s, 0,
        z*x*one_minus_c - y*s,   z*y*one_minus_c + x*s, c + z*z*one_minus_c,   0,
        0, 0, 0, 1.0f
    );
}

// Returns a row-major "point at" matrix (for row-major math libraries)
Mat4 pointat_matrix(const Vec3& pos, const Vec3& target, const Vec3& up_origin) {
    Vec3 forward = (target - pos).normalize();

    // Project up_origin onto forward to get the orthogonal up vector
    Vec3 a = forward * up_origin.dot_product(forward);
    Vec3 up = (up_origin - a).normalize();

    Vec3 right = up.cross_product(forward);

    // Row-major: each row is a basis vector, last row is translation
    // For view matrix, translation should be in the last column, not last row
    // But for a "point at" matrix, this is a world-to-camera transform, so:
    return Mat4 (
        right.x,    up.x,    forward.x,    pos.x,
        right.y,    up.y,    forward.y,    pos.y,
        right.z,    up.z,    forward.z,    pos.z,
        0,          0,       0,            1
    );
}

// Row-major quick inverse for rotation/translation matrix
Mat4 quick_inverse_matrix(const Mat4& m) {
    // Extract rotation (upper 3x3)
    Mat4 rotT(
        m.val[0][0], m.val[1][0], m.val[2][0], 0,
        m.val[0][1], m.val[1][1], m.val[2][1], 0,
        m.val[0][2], m.val[1][2], m.val[2][2], 0,
        0,           0,           0,           1
    );
    // Extract translation
    Vec3 t(m.val[0][3], m.val[1][3], m.val[2][3]);
    // Inverse translation
    Vec3 invT = -(rotT * t.to_vec4()).to_vec3();

    Mat4 result = rotT;
    result.val[0][3] = invT.x;
    result.val[1][3] = invT.y;
    result.val[2][3] = invT.z;
    result.val[3][3] = 1.0f;
    return result;
}

// line could be expressed as P=A+t(B−A)
// Vec3 intersection_plane_line(const Vec3& A, const Vec3& B, const Vec3& plane_n, float plane_d) {
//     // t from 0 to 1, represent position on line AB
//     Vec3 AB = B - A;
//     float t = (-plane_d - plane_n.dot_product(A)) / (plane_n.dot_product(AB));
//     return Vec3(A + ((AB) * t));
// }
Vec3 intersection_plane_line(const Vec3& A, const Vec3& B, const Vec3& plane_n, const Vec3& plane_p) {
    // t from 0 to 1, represent position on line AB
    Vec3 AB = B - A;
    float denom = plane_n.dot_product(AB);
    if (fabs(denom) < 1e-6f) 
    {
        std::cout << "ERROR: Parallel\n";
        return A; // Avoid division by zero, return A as fallback
    }
    float t = (plane_n.dot_product(plane_p - A)) / denom;
    return AB * t + A;
}
// Pass the origin obj triangles into this
// If we dont change obj size, this could just run once
void cal_circle_info(const std::vector<Triangle>& obj, Vec3& out_center, float& out_radius) {
    if (obj.empty()) {
        out_center = Vec3(0, 0, 0);
        out_radius = 0.0f;
        return;
    }

    float max_x = obj[0].p[0].x, max_y = obj[0].p[0].y, max_z = obj[0].p[0].z;
    float min_x = obj[0].p[0].x, min_y = obj[0].p[0].y, min_z = obj[0].p[0].z;

    for (const auto& tri : obj) {
        for (const auto& p : tri.p) {
            max_x = std::max(p.x, max_x);
            max_y = std::max(p.y, max_y);
            max_z = std::max(p.z, max_z);

            min_x = std::min(p.x, min_x);
            min_y = std::min(p.y, min_y);
            min_z = std::min(p.z, min_z);
        }
    }

    Vec3 center = Vec3((max_x + min_x) / 2.0f, (max_y + min_y) / 2.0f, (max_z + min_z) / 2.0f);

    float max_dist_sq = 0.0f;
    for (const auto& tri : obj) {
        for (const auto& p : tri.p) {
            float dist_sq = (p - center).dot_product(p - center);
            if (dist_sq > max_dist_sq) {
                max_dist_sq = dist_sq;
            }
        }
    }
    float radius = std::sqrt(max_dist_sq);

    out_center = center;
    out_radius = radius;
}

float signed_distance_plane_point(const Vec3& plane_n, const Vec3& plane_p, const Vec3& point) {
    return (point - plane_p).dot_product(plane_n);
}

void rotate_vec2(float rad, Vec2& vec) {
    Mat2 rotate_mat(cos(rad), -sin(rad), sin(rad), cos(rad));
    vec = std::move(rotate_mat * vec);
}