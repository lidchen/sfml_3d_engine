#include "input_handle.h"
sf::Vector2i current_mouse_pos = sf::Mouse::getPosition();
static sf::Vector2i last_mouse_pos = current_mouse_pos;

bool isKeyPressedOnce(sf::Keyboard::Key key) {
    static std::unordered_map<sf::Keyboard::Key, bool> wasPressed;
    bool currentlyPressed = sf::Keyboard::isKeyPressed(key);
    bool pressedOnce = currentlyPressed && !wasPressed[key];
    wasPressed[key] = currentlyPressed;
    return pressedOnce;
}

bool isMousePressedOnce(sf::Mouse::Button button) {
    static std::unordered_map<sf::Mouse::Button, bool> wasPressed;
    bool currentlyPressed = sf::Mouse::isButtonPressed(button);
    bool pressedOnce = currentlyPressed && !wasPressed[button];
    wasPressed[button] = currentlyPressed;
    return pressedOnce;
}

void gaze_handle(float diff_x, float diff_y, float diff_z) {
  // Scale gaze deltas
  constexpr float gaze_to_game_scale_x = 10.0f;
  constexpr float gaze_to_game_scale_y = 10.0f;
  constexpr float gaze_to_game_scale_z = -20.0f;

  // Calculate movement in camera space
  Vec3 move = camera_dir.normalize() * (diff_z * gaze_to_game_scale_z)
        + Vec3(0.f, 1.f, 0.f) * (diff_y * gaze_to_game_scale_y)
        + camera_dir.cross_product(Vec3(0.f, 1.f, 0.f)).normalize() * (diff_x * gaze_to_game_scale_x);

  camera_vec = camera_vec + move;
}

void input_handle(float dt) {
    float move_speed = k_move_speed;
    if (over || sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        window.close();
    }
    if (isKeyPressedOnce(sf::Keyboard::U)) {
        debug_view_on = !debug_view_on;
        if (debug_view_on) {
            enable_mouse_release = true;
            std::cout << "debug view on\n";
        }
        else {
            enable_mouse_release = false;
            last_mouse_pos = sf::Mouse::getPosition();
            std::cout << "debug view off\n";
        }
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        camera_vec = camera_vec + Vec3(0.f, 1.f, 0.f) * dt * move_speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
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
    
    //=========== Mouse Input ==========
    if (!enable_mouse_release) {
        current_mouse_pos = sf::Mouse::getPosition();
        sf::Vector2i mouse_movement = current_mouse_pos - last_mouse_pos;
        last_mouse_pos = current_mouse_pos;
        // std::cout << mouse_movement.x << " " << relative_mouse_pos.y << "\n";
        // debug::console.log(camera_dir.dot_product(world_up));

        // Use static variables to store yaw and pitch
        static float yaw = 0.0f;
        static float pitch = 0.0f;

        yaw   -= k_mouse_sensitivity * mouse_movement.x;
        pitch -= k_mouse_sensitivity * mouse_movement.y;

        // Clamp pitch to avoid flipping
        const float pitch_limit = 1.55f; // ~89 degrees
        if (pitch > pitch_limit) pitch = pitch_limit;
        if (pitch < -pitch_limit) pitch = -pitch_limit;

        // Convert yaw and pitch to direction vector
        camera_dir.x = cosf(pitch) * sinf(yaw);
        camera_dir.y = sinf(pitch);
        camera_dir.z = cosf(pitch) * cosf(yaw);
        camera_dir = camera_dir.normalize();
    }
}
