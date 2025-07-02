// TODO:
// Debug GUI
// mouse rotation
// camera edge clipping
// point light
// light trace
// Better rotation function (rotate x, y at difference speed according to dt)

#include <SFML/Graphics.hpp>
#include <iostream>
#include "variable.h"
#include "basic_type.h"
#include "util_3d.h"
#include "input_handle.hpp"
#include "debug.h"

void draw_vec2(const Vec2& vec) {
    const float scale = 10.f;
    sf::VertexArray line(sf::Lines, 2);
    line[0].position = sf::Vector2f(0 + screenX/2, 0 + screenY/2);
    line[1].position = sf::Vector2f(scale * vec.x + screenX/2, scale * vec.y + screenY/2);
    line[0].color = sf::Color::Green;
    line[1].color = sf::Color::Red;
    window.draw(line);
}

void draw_vec3(const Vec3& vec) {
    Vec3 v = vec.normalize();
    const float scale = 100.f;
    sf::VertexArray line(sf::LineStrip, 2);
    line[0].position = sf::Vector2f(screenX/2, screenY/2);
    line[1].position = sf::Vector2f(v.x * scale + screenX/2, v.y * scale + screenY/2);
    line[0].color = sf::Color::Blue;
    line[1].color = sf::Color::Blue;
    window.draw(line);
}

void draw_tri(const Triangle t) {
    const float scale = 3000.f;
    sf::VertexArray triangle(sf::Triangles, 3);

    // Define the triangle vertices
    triangle[0].position = sf::Vector2f(t.p[0].x * scale + screenX/2, t.p[0].y * scale + screenY/2);
    triangle[1].position = sf::Vector2f(t.p[1].x * scale + screenX/2, t.p[1].y * scale + screenY/2);
    triangle[2].position = sf::Vector2f(t.p[2].x * scale + screenX/2, t.p[2].y * scale + screenY/2);

    // Fill with scale according to Illumination
    // light_strength is now clamped to [0, 1], so just scale to 0-255
    uint8_t intensity = static_cast<uint8_t>(t.light_strength * 255);
    sf::Color tri_color(intensity, intensity, intensity);
    triangle[0].color = tri_color;
    triangle[1].color = tri_color;
    triangle[2].color = tri_color;

    window.draw(triangle);

    // DEBUG LINE
    if (enable_debug_line) {
        sf::VertexArray line(sf::LineStrip, 4);
        line[0].position = sf::Vector2f(t.p[0].x * scale + screenX/2, t.p[0].y * scale + screenY/2);
        line[1].position = sf::Vector2f(t.p[1].x * scale + screenX/2, t.p[1].y * scale + screenY/2);
        line[2].position = sf::Vector2f(t.p[2].x * scale + screenX/2, t.p[2].y * scale + screenY/2);
        line[3].position = sf::Vector2f(t.p[0].x * scale + screenX/2, t.p[0].y * scale + screenY/2);

        line[0].color = sf::Color::Red;
        line[1].color = sf::Color::Red;
        line[2].color = sf::Color::Red;
        line[3].color = sf::Color::Red;

        window.draw(line);
    }
}

void draw_camera_near() {
    // Draw the camera's near plane as a semi-transparent quad
    // Use camera_dir (normal), camera_vec (camera position), and n (plane distance)
    Vec3 plane_n = camera_dir.normalize();
    float plane_d = n;

    // Find two orthogonal vectors to plane_n
    Vec3 up(0, 1, 0);
    Vec3 right = plane_n.cross_product(up);
    if (right.length() < 1e-3f) {
        up = Vec3(1, 0, 0);
        right = plane_n.cross_product(up);
    }
    right = right.normalize();
    up = right.cross_product(plane_n).normalize();

    // Center of the near plane in world space
    Vec3 center = camera_vec + plane_n * plane_d;

    float plane_size = 0.04; // Adjust for visual size

    // Four corners in world space
    Vec3 corners[4];
    corners[0] = center + right * plane_size + up * plane_size;
    corners[1] = center - right * plane_size + up * plane_size;
    corners[2] = center - right * plane_size - up * plane_size;
    corners[3] = center + right * plane_size - up * plane_size;

    // Transform to view space, then project to screen
    sf::VertexArray quad(sf::Quads, 4);
    for (int i = 0; i < 4; ++i) {
        Vec3 view_space = (view_mat * corners[i].to_vec4()).to_vec3();
        Vec3 projected = (projection_mat * view_space.to_vec4()).to_vec3();
        quad[i].position = sf::Vector2f(
            projected.x * 3000.f + screenX / 2,
            projected.y * 3000.f + screenY / 2
        );
        quad[i].color = sf::Color(255, 0, 0, 100); // Red, semi-transparent
    }
    window.draw(quad);
}

void draw_plane(const Vec3& plane_n, float plane_d) {
    // Draw a large quad representing the near plane in view space
    // Assume plane_n is normalized

    // Find two vectors orthogonal to plane_n to define the plane
    Vec3 up(0, 1, 0);
    Vec3 right = plane_n.cross_product(up);
    if (right.length() < 1e-3f) {
        // If plane_n is parallel to up, use another vector
        up = Vec3(1, 0, 0);
        right = plane_n.cross_product(up);
    }
    right = right.normalize();
    up = right.cross_product(plane_n).normalize();

    // Center of the plane in view space
    // Vec3 center = plane_n * (plane_d);
    Vec3 center = plane_n - plane_d;

    // Size of the plane (make it large enough to cover the screen)
    float plane_size = 1.0f; // Adjust as needed

    // Four corners of the plane
    Vec3 corners[4];
    corners[0] = center + right * plane_size + up * plane_size;
    corners[1] = center - right * plane_size + up * plane_size;
    corners[2] = center - right * plane_size - up * plane_size;
    corners[3] = center + right * plane_size - up * plane_size;

    // Project corners to screen
    sf::VertexArray quad(sf::Quads, 4);
    for (int i = 0; i < 4; ++i) {
        Vec3 projected = (projection_mat * (view_mat * corners[i].to_vec4())).to_vec3();
        quad[i].position = sf::Vector2f(
            projected.x * 3000.f + screenX / 2,
            projected.y * 3000.f + screenY / 2
        );
        quad[i].color = sf::Color(0, 0, 255, 100); // Blue, semi-transparent
    }
    window.draw(quad);
}

void tri_clipping(std::vector<Triangle>& tris) {
// for test
// n = 4.f;
    bool clipped = true;
    if (tris.size() == 0) {
        // std::cout << "empty tris\n";
        return;
    }
    std::vector<Triangle> clipped_tris;

    for (auto tri : tris) {
        // Store distances to plane for each vertex
        float d[3];
        // camera_dir.print();
        // d[0] = signed_distance_plane_point(camera_dir, camera_vec, tri.p[0]);
        // d[1] = signed_distance_plane_point(camera_dir, camera_vec, tri.p[1]);
        // d[2] = signed_distance_plane_point(camera_dir, camera_vec, tri.p[2]);

        d[0] = signed_distance_plane_point(Vec3(0, 0, 1), Vec3(0, 0, n), tri.p[0]);
        d[1] = signed_distance_plane_point(Vec3(0, 0, 1), Vec3(0, 0, n), tri.p[1]);
        d[2] = signed_distance_plane_point(Vec3(0, 0, 1), Vec3(0, 0, n), tri.p[2]);

        // tri.p[0].print();
        // std::cout << d[0] << "\n";
        // tri.p[1].print();
        // std::cout << d[1] << "\n";
        // tri.p[2].print();
        // std::cout << d[2] << "\n";

        // Classify points as inside or outside
        std::vector<Vec3> inside_points, outside_points;
        for (int i = 0; i < 3; ++i) {
            if (d[i] >= 0)
                inside_points.push_back(tri.p[i]);
            else
                outside_points.push_back(tri.p[i]);
        }
        if (inside_points.size() == 0) {
            clipped = false;
            // All points are outside
            // std::cout << "ALL OUTSIDE\n";
            continue;
        }
        if (inside_points.size() == 3) {
            // All points are inside
            // std::cout << "ALL INSIDE\n";
            clipped_tris.emplace_back(tri);
            continue;
        }
        if (inside_points.size() == 1 && outside_points.size() == 2) {
            // One point inside, two outside: form one new triangle
            // std::cout << "ONE INSIDE\n";
            Vec3 A = inside_points[0];
            Vec3 B = outside_points[0];
            Vec3 C = outside_points[1];
            Vec3 clipped_B = intersection_plane_line(A, B, Vec3(0.f, 0.f, 1.f), Vec3(0.f, 0.f, n));
            Vec3 clipped_C = intersection_plane_line(A, C, Vec3(0.f, 0.f, 1.f), Vec3(0.f, 0.f, n));
            Triangle new_tri = Triangle(A, clipped_C, clipped_B);
            new_tri.light_strength = tri.light_strength;
            clipped_tris.emplace_back(new_tri);
            // clipped_tris.emplace_back(Triangle(A, clipped_C, clipped_B));
            continue;
        }
        if (inside_points.size() == 2 && outside_points.size() == 1) {
            // Two points inside, one outside: form two new triangles
            // std::cout << "TWO INSIDE\n";
            Vec3 A = inside_points[0];
            Vec3 B = inside_points[1];
            Vec3 C = outside_points[0];
            Vec3 clipped_A = intersection_plane_line(A, C, Vec3(0.f, 0.f, 1.f), Vec3(0.f, 0.f, n));
            Vec3 clipped_B = intersection_plane_line(B, C, Vec3(0.f, 0.f, 1.f), Vec3(0.f, 0.f, n));
            Triangle new_tri1 = Triangle(A, B, clipped_A);
            Triangle new_tri2 = Triangle(B, clipped_B, clipped_A);
            new_tri1.light_strength = tri.light_strength;
            new_tri2.light_strength = tri.light_strength;
            // clipped_tris.emplace_back(Triangle(A, B, clipped_A));
            // clipped_tris.emplace_back(Triangle(B, clipped_B, clipped_A));
            clipped_tris.emplace_back(new_tri1);
            clipped_tris.emplace_back(new_tri2);
            continue;
        }
    }
    // std::cout << "Tri num: " << clipped_tris.size() << "\n";
    // std::cout << "================================\n";
    tris = clipped_tris;
}

void obj_clipping(std::vector<Triangle>&tris, const Vec3& center, float radius){
    // Cal distance d to clipping face
    // camera_dir and n(near face) define the face
    // dot(a, b) = |a||b|cos(theta)
    camera_dir = camera_dir.normalize();
    // Calculate distance from camera to object's center along camera_dir
    // float d = n + (center - camera_vec).dot_product(camera_dir);
    float d = signed_distance_plane_point(Vec3(0.f, 0.f, 1.f), Vec3(0.f, 0.f, n), center);
    // std::cout << "d " << d << "\n";
    if (d > radius) {
        // std::cout << "INSIDE\n";
        return;
    }
    else if (d < -radius) {
        tris.clear();
        // std::cout << "OUTSIDE\n";
        return;
    }
    else {
        tri_clipping(tris);
    }
    // if d > r, inside, if d < -r outside
    // if abs(d) < r, use triangle_clipping
}

bool setup() {
    if(!mesh_cube.load_from_obj(k_model_path)) {
        std::cerr << "Cant load from obj\n";
        return 0;
    }
    if (!Alkhemikal.loadFromFile("../res/font/Alkhemikal.ttf")) {
        std::cerr << "Cant load font\n";
        return 0;
    }

    return 1;
}

int main() {
    float radius;
    Vec3 center;
    if (!setup()) {
        return 0;
    }
// for test
// mesh_cube = tri_for_test;
    // TEST
    if (1) {
        std::cout << Vec3(1.f, 1.f, 1.f) << "\n";
        std::cout << projection_mat;
        debug::console.log("hello");
        debug::ui.register_var("camera_vec", &camera_vec);
        debug::ui.register_var("camera_dir", &camera_dir);
        debug::ui.register_var("projection mat", &projection_mat);
        debug::ui.register_var("fps", &fps);
    }

    Vec3 up(0, 1, 0);
    Vec3 target = camera_vec + camera_dir;
    camera_mat = pointat_matrix(camera_vec, target, up);
    view_mat = quick_inverse_matrix(camera_mat);
    if (1) {
        window.setFramerateLimit(240);
        sf::Clock clock;
        float theta = 0;
        Vec3 rotation_point = Vec3(1.f, 0.f, 1.f).normalize();
        sf::Vector2i last_mouse_pos= sf::Mouse::getPosition();
        cal_circle_info(mesh_cube.tris_to_raster, center, radius);
        Vec3 center_new = center;

        while (window.isOpen()) {
            sf::Event event;
            dt = clock.restart().asSeconds();
            fps = (dt > 0.0f) ? (1.f / dt) : 0.0f;
            input_handle(dt);
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            
            theta += dt;
            window.clear();
            std::vector<Triangle> translated_tris;
            std::vector<Triangle> viewed_tris;
            std::vector<Triangle> projected_tris;

            target = camera_vec + camera_dir;
            camera_mat = pointat_matrix(camera_vec, target, up);
            view_mat = quick_inverse_matrix(camera_mat);

            for (auto tri : mesh_cube.tris_to_raster) {
                move_triangle(tri, tri.cal_norm_vec(), displacement);
                Triangle tri_translated;
                tri_translated = tri;
                // Rotation
                if (enable_rotation) {
                    tri_translated.p[0] = (rotation_matrix(rotation_point, theta) * tri.p[0].to_vec4()).to_vec3();
                    tri_translated.p[1] = (rotation_matrix(rotation_point, theta) * tri.p[1].to_vec4()).to_vec3();
                    tri_translated.p[2] = (rotation_matrix(rotation_point, theta) * tri.p[2].to_vec4()).to_vec3();
                    center_new = (rotation_matrix(rotation_point, theta) * center.to_vec4()).to_vec3();
                }
// If the cross_product z is negative(points out), we dont draw it
// if (tri_translated.cal_norm_vec().z <= 0) {

// This method dont work so well, so use the dot product method below...

// If the dot_product of tri.p and norm_vec is larger or equal to 90 degree, we dont draw it
// dot_product >= 90degree == dot_product >= 0.f

                // Do every complex computation after this check for optimization
                Vec3 norm_vec = tri_translated.cal_norm_vec();
                if ((norm_vec.dot_product((tri_translated.p[0] - camera_vec).normalize())) >= 0.f) {
                    continue;
                }
// BOMB!
// move_triangle(tri_translated, norm_vec, displacement);
                // Calculate each light strength and store it in pair
                float light_strength = light_dir.dot_product(tri_translated.cal_norm_vec().normalize());
                // Clamp light_strength to [0, 1] for proper shading
                light_strength = std::max(0.1f, std::min(1.f, light_strength));
                // Convert World Space --> View Space
                Triangle tri_viewed;
                tri_viewed.p[0] = (view_mat * tri_translated.p[0].to_vec4()).to_vec3();
                tri_viewed.p[1] = (view_mat * tri_translated.p[1].to_vec4()).to_vec3();
                tri_viewed.p[2] = (view_mat * tri_translated.p[2].to_vec4()).to_vec3();
                tri_viewed.light_strength = light_strength;

                viewed_tris.emplace_back(tri_viewed);
            }

            center_new = (view_mat * center.to_vec4()).to_vec3();
            // tri_clipping(viewed_tris);
            obj_clipping(viewed_tris, center_new, radius);

            for (auto tri_viewed : viewed_tris) {
                Triangle tri_projected;
                // Project triangles from 3D --> 2D
                
                // (projection_mat * tri_viewed.p[0].to_vec4()).print();
                // (projection_mat * tri_viewed.p[1].to_vec4()).print();
                // (projection_mat * tri_viewed.p[2] .to_vec4()).print();
                tri_projected.p[0] = (projection_mat * tri_viewed.p[0].to_vec4()).to_vec3();
                tri_projected.p[1] = (projection_mat * tri_viewed.p[1].to_vec4()).to_vec3();
                tri_projected.p[2] = (projection_mat * tri_viewed.p[2].to_vec4()).to_vec3();

                tri_projected.light_strength = tri_viewed.light_strength;
                projected_tris.push_back(tri_projected);
            }

            // Deep test
            // Sort projected_tris for depth test based on the triangle's middle z value
            std::sort(projected_tris.begin(), projected_tris.end(),
                [](const Triangle& a, const Triangle& b) {
                    return a.get_middle_z() > b.get_middle_z();
                });

            for (auto tri: projected_tris) {
                draw_tri(tri);
            }
            if (debug_view_on) {
                debug::ui.draw(); 
            }
            window.display();
        }
    }
    return 0;
}
