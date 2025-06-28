#include "basic_type.h"
Triangle::Triangle(const Vec3& a, const Vec3& b, const Vec3& c) {
    p[0] = a;
    p[1] = b;
    p[2] = c;
    light_strength = 1.0f;
}
Triangle::Triangle() = default;
void Triangle::print() {
    p[0].print();
    p[1].print();
    p[2].print();
    std::cout << "\n";
}

Vec3 Triangle::cal_norm_vec() {
    Vec3 v1 = p[1] - p[0];
    Vec3 v2 = p[2] - p[0];
    return v1.cross_product(v2);
}

float Triangle::get_middle_z() const {
    return ((p[0].z + p[1].z + p[2].z) / 3);
}

Mesh::Mesh() {};
Mesh::Mesh(const std::vector<Triangle> &t) {
    tris_to_raster = t;
};
bool Mesh::load_from_obj(const std::string& file_location) {
    std::ifstream f(file_location);
    if (!f.is_open()) return false;
    std::vector<Vec3> verts;
    while(!f.eof()) {
        // Assume one line is no more that 128 char, this is bad...
        char line[128];
        f.getline(line, 128);
        
        std::stringstream s;
        s << line;
        char junk;
        if (line[0] == 'v') {
            Vec3 v;
            s >> junk >> v.x >> v.y >> v.z;
            verts.push_back(v);
        }
        if (line[0] == 'f') {
            int f[3];
            s >> junk >> f[0] >> f[1] >> f[2];
            tris_to_raster.push_back(Triangle((verts[f[0] - 1]), verts[f[1] - 1], verts[f[2] - 1]));
        }
    }
    return true;
}

