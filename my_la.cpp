#include "my_la.h"
Vec::~Vec() {};
Mat::~Mat() {};
Vec2::Vec2(float x, float y) : x(x), y(y) {
    std::cout << "Vec2[" << x << "," << y << "] constructed\n";
}
Vec2::~Vec2() {
    std::cout << "Vec2[" << x << "," << y << "] destructed\n";
}

Mat2::Mat2(float a, float b, float c, float d) : m_a(a), m_b(b), m_c(c), m_d(d) {
    std::cout << "Mat2[" << m_a << "," << m_b << "," << m_c << m_d << "] constructed\n";
}
Mat2::~Mat2() {
    std::cout << "Mat2[" << m_a << "," << m_b << "," << m_c << m_d << "] destructed\n";
}
Vec2 Mat2::operator*(const Vec2 &vec) {
    float x = m_a * vec.x + m_b * vec.y;
    float y = m_c * vec.x + m_d * vec.y;
    return std::move(Vec2(x, y));
}

Vec3::Vec3() {}
Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
Vec3::~Vec3() {
    // std::cout << "Vec3 destructed\n";
}
Vec3 Vec3::normalize() const {
    float l = sqrt(x*x + y*y + z*z);
    return Vec3(x/l, y/l, z/l);
}
Vec3 Vec3::cross_product(const Vec3 &v) const {
    return Vec3((y*v.z - z*v.y),(z*v.x - x*v.z), (x*v.y - y*v.x));
}
float Vec3::dot_product(const Vec3& v) const {
    return (x * v.x + y * v.y + z * v.z);
}
Vec3 Vec3::operator*(float n) const {
    return Vec3(x * n, y * n, z * n);
}
Vec3 Vec3::operator+(const Vec3& v) const {
    return Vec3(x + v.x, y + v.y, z + v.z);
}
Vec3 Vec3::operator+(float n) const {
    return Vec3(x + n, y + n, z + n);
}
Vec3 Vec3::operator-(float n) const {
    return Vec3(x - n, y - n, z - n);
}
Vec3 Vec3::operator-(const Vec3& v) const {
    return Vec3(x - v.x, y - v.y, z - v.z);
}
Vec3 Vec3::operator-() const {
    return Vec3(-x, -y, -z);
}
bool Vec3::operator==(const Vec3& v) const {
    return (x == v.x && y == v.y && z == v.z);
}
float Vec3::length() const {
    return std::sqrt(x*x + y*y + z*z);
}
std::string Vec3::to_string() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3)
    << "[" << x << ", " << y << ", " << z << "]";
    return oss.str();
}
std::ostream& operator<< (std::ostream& os, Vec3 vec) {
    return os << vec.to_string();
}
void Vec3::print() const {
    std::cout << "[" << x << ", " << y << ", " << z << "]\n";
}
Vec4 Vec3::to_vec4() const {
    return Vec4(x, y, z, 1.f);
}

Vec4::Vec4() {
    x = 0; y = 0; z = 0; w = 1.f;
};
Vec4::Vec4(float a, float b, float c, float d) {
    x = a; y = b; z = c; w = d;
}
Vec3 Vec4::to_vec3() const{
    if (w == 0) {
        std::cerr << "w is 0\n";
    }
    return Vec3(x/w, y/w, z/w);
}
void Vec4::print() const {
    std::cout << "[" << x << ", " << y << ", " << z << ", " << w << "]\n";
}
std::string Vec4::to_string() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3)
    << "[" << x << ", " << y << ", " << z << ", " << w << "]";
    return oss.str();
}
std::ostream& operator<< (std::ostream& os, Vec4 vec) {
    return os << vec.to_string();
}
Vec4::~Vec4() {
    // std::cout << "Vec4 destructed\n";
}

Mat4::Mat4() {
    for (int i = 0; i < 4; ++i){
        for (int j = 0; j < 4; ++j) {
            if (i == j) { val[i][j] = 1; }
            else { val[i][j] = 0; }
        }
    }
}
Mat4::Mat4 (
    float a1, float a2, float a3, float a4,
    float b1, float b2, float b3, float b4,
    float c1, float c2, float c3, float c4,
    float d1, float d2, float d3, float d4
) {
    val[0][0] = a1, val[0][1] = a2, val[0][2] = a3, val[0][3] = a4;
    val[1][0] = b1, val[1][1] = b2, val[1][2] = b3, val[1][3] = b4;
    val[2][0] = c1, val[2][1] = c2, val[2][2] = c3, val[2][3] = c4;
    val[3][0] = d1, val[3][1] = d2, val[3][2] = d3, val[3][3] = d4;
}

// bullshit
Vec4 Mat4::operator* (const Vec4& vec) {
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 0;
    x += val[0][0] * vec.x;
    x += val[0][1] * vec.y;
    x += val[0][2] * vec.z;
    x += val[0][3] * vec.w;

    y += val[1][0] * vec.x;
    y += val[1][1] * vec.y;
    y += val[1][2] * vec.z;
    y += val[1][3] * vec.w;

    z += val[2][0] * vec.x;
    z += val[2][1] * vec.y;
    z += val[2][2] * vec.z;
    z += val[2][3] * vec.w;

    w += val[3][0] * vec.x;
    w += val[3][1] * vec.y;
    w += val[3][2] * vec.z;
    w += val[3][3] * vec.w;
    if (x != x || y!= y || z != z || w != w) {
        std::cerr << "\nNaN\n\n";
        over = true;
    }
    if (std::isinf(x) || std::isinf(y) || std::isinf(z) || std::isinf(w)) {
        std::cerr << "\ninf\n\n";
        over = true;
    }
    return Vec4(x, y, z, w);
}
Mat4 Mat4::operator*(const Mat4& mat) {
    Mat4 new_mat;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            new_mat.val[i][j] = 0;
            for (int k = 0; k < 4; ++k) {
                new_mat.val[i][j] += val[i][k] * mat.val[k][j];
            }
        }
    }
    return new_mat;
}
Mat4::~Mat4() {
    // std::cout << "Mat4 destruct\n";
}

std::string Mat4::to_string() const {
    std::ostringstream oss;
    for (int i = 0; i < 4; ++i) {
        if (i == 0) oss << "⎡";
        else if (i == 3) oss << "⎣";
        else oss << "⎜";
        for (int j = 0; j < 4; ++j) {
            oss << std::setw(6) << std::right << std::fixed << std::setprecision(3) << val[i][j] << " ";
        }
        if (i == 0) oss << "⎤";
        else if (i == 3) oss << "⎦";
        else oss << "⎟";
        oss << "\n";
    }
    oss << "\n";
    return oss.str();
}
std::ostream& operator<< (std::ostream& os, Mat4 mat) {
    return os << mat.to_string();
}

void Mat4::print() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << val[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}



