#ifndef MY_LA
#define MY_LA
#include <array>
#include <iostream>
#include <sstream>
#include <iomanip>

extern bool over;
class Vec4;

// TODO
// Implement this using Template
// https://chatgpt.com/c/685617f8-3508-8011-a2a8-d0c1a160dc35

class Vec {
public:
    virtual ~Vec() = 0;
    Vec operator* (const Vec &vec);
    Vec operator+ (const Vec &vec);
    Vec operator- (const Vec &vec);
    Vec cross_product (const Vec &vec);
    Vec dot_product (const Vec &vec);
    void print();
};

class Mat {
public:
    virtual ~Mat() = 0;
    Mat operator* (const Mat &mat);
    Mat operator+ (const Mat &mat);
    Mat operator- (const Mat &mat);
    Vec operator* (const Vec &vec);
    void print();
};

class Vec2 : public Vec{
friend class Mat2;
public:
    float x, y;

    Vec2(float x, float y);
    ~Vec2();
};

class Mat2 : public Mat{
public:
    Mat2(float a, float b, float c, float d);
    ~Mat2();
    Vec2 operator*(const Vec2 &vec);
private:
    float m_a = 0;
    float m_b = 0;
    float m_c = 0;
    float m_d = 0;
};

class Vec3 : public Vec{
public:
    float x, y, z;
    Vec3();
    Vec3(float x, float y, float z);
    Vec3 normalize() const;
    Vec3 cross_product(const Vec3 &v) const;
    float dot_product(const Vec3 &v) const;
    Vec3 operator*(float n) const;
    Vec3 operator+(const Vec3 &v) const;
    Vec3 operator+(float n) const;
    Vec3 operator-(float n) const;
    Vec3 operator-(const Vec3 &v) const;
    Vec3 operator-() const;
    bool operator==(const Vec3 &v) const;
    float length() const;
    void print() const;
    std::string to_string() const;
    friend std::ostream& operator<< (std::ostream& os, Vec3 vec);
    Vec4 to_vec4() const;
    ~Vec3();
};

class Vec4 : public Vec{
public:
    Vec4();
    Vec4(float a, float b, float c, float d);
    ~Vec4();
    Vec3 to_vec3() const;
    std::string to_string() const;
    friend std::ostream& operator<< (std::ostream& os, Vec4 vec);
    void print() const;
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 0;
};

class Mat4 : public Mat{
public:
    float val[4][4];
    Mat4 ();
    Mat4 (
        float a1, float b1, float c1, float d1,
        float a2, float b2, float c2, float d2,
        float a3, float b3, float c3, float d3,
        float a4, float b4, float c4, float d4
    );
    ~Mat4();
    Vec4 operator* (const Vec4& vec);
    Mat4 operator* (const Mat4& mat);

    std::string to_string() const;
    friend std::ostream& operator<< (std::ostream& os, Mat4 vec);
    void print();
};

#endif