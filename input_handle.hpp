#include "variable.h"

bool isKeyPressedOnce(sf::Keyboard::Key key) {
    static std::unordered_map<sf::Keyboard::Key, bool> wasPressed;
    bool currentlyPressed = sf::Keyboard::isKeyPressed(key);
    bool pressedOnce = currentlyPressed && !wasPressed[key];
    wasPressed[key] = currentlyPressed;
    return pressedOnce;
}

void input_handle(float dt) {
    float move_speed = k_move_speed;
    if (over || sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        window.close();
    }
    if (isKeyPressedOnce(sf::Keyboard::U)) {
        debug_view_on = !debug_view_on;
        if (debug_view_on) std::cout << "debug view on\n";
        else std::cout << "debug view off\n";
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
        move_speed *= 3;
    } 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        camera_vec = camera_vec + camera_dir * dt * move_speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        camera_vec = camera_vec - camera_dir * dt * move_speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        camera_vec = camera_vec + Vec3(0.f, 1.f, 0.f).cross_product(camera_dir * dt * move_speed);
        // light_dir.x -= offset;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        camera_vec = camera_vec - Vec3(0.f, 1.f, 0.f).cross_product(camera_dir * dt * move_speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        camera_vec = camera_vec + Vec3(0.f, 1.f, 0.f) * dt * move_speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        camera_vec = camera_vec - Vec3(0.f, 1.f, 0.f) * dt * move_speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        camera_dir = (rotation_matrix({0, 1.f, 0}, k_rotation_speed * dt) * camera_dir.to_vec4()).to_vec3();
        camera_dir = camera_dir.normalize();
        // light_dir = (rotation_matrix({0, 1.f, 0}, 0.1f) * light_dir.to_vec4()).to_vec3();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        camera_dir = (rotation_matrix({0, 1.f, 0}, -k_rotation_speed * dt) * camera_dir.to_vec4()).to_vec3();
        camera_dir = camera_dir.normalize();
        // light_dir = (rotation_matrix({0, 1.f, 0}, -0.1f) * light_dir.to_vec4()).to_vec3();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
        displacement += 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
        displacement -= 1.f;
    }
    
    // =========== Mouse Input ==========
    // sf::Vector2i current_mouse_pos = sf::Mouse::getPosition();
    // sf::Vector2i relative_mouse_pos = current_mouse_pos - last_mouse_pos;
    // last_mouse_pos = current_mouse_pos;
    // std::cout << relative_mouse_pos.x << " " << relative_mouse_pos.y << "\n";
    // camera_mat = rotation_matrix({0, 1.f, 0}, 0.01f * relative_mouse_pos.x) * camera_mat;
    // camera_mat = rotation_matrix({1.f, 0, 0}, -0.01f * relative_mouse_pos.y) * camera_mat;
}
