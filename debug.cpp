#include "debug.h"



Button::Button(const std::string& name, const sf::Vector2f& pos, bool& var) : name(name), var(&var) {
    button_text.setPosition(pos);
    button_text.setFont(Alkhemikal);
    button_text.setString(name);
    button_text.setCharacterSize(button_fontsize);
    button_text.setFillColor(text_color);
    button_text.setOutlineThickness(1.f);
    button_text.setOutlineColor(sf::Color::Black);
    width = button_text.getGlobalBounds().width;
    height = button_text.getGlobalBounds().height * height_scale;

    button_shape.setPosition(pos);
    button_shape.setSize(sf::Vector2f(width, height));
    button_shape.setFillColor(button_color);
}

void Button::draw() const {
    window.draw(button_shape);
    window.draw(button_text);
}

void Button::update() {
    sf::Vector2i local_pos = sf::Mouse::getPosition(window);
    float mouse_x = local_pos.x;
    float mouse_y = local_pos.y;
    float x = button_shape.getGlobalBounds().left;
    float y = button_shape.getGlobalBounds().top;
    if (mouse_x > x && mouse_x < x + width && mouse_y > y && mouse_y < y + height) {
        if (isMousePressedOnce(sf::Mouse::Button::Left)) {
            press(); 
        }
    }
    button_shape.setFillColor(button_color);
}

void Button::press() {
    if (*var) {
        button_color = sf::Color::White;
    } else {
        button_color = sf::Color::Red;
    }
    *var = !*var;
}

namespace debug {
    // void Console::log(const std::string& msg) {
    //     std::cout << "[DEBUG]: "<< msg << "\n";
    // }
    UI::UI() {
        debug_ui_info.setFont(Alkhemikal);
        debug_ui_info.setCharacterSize(18);
        debug_ui_info.setPosition(0, 0);
        debug_ui_info.setFillColor(sf::Color::White);
        debug_ui_info.setOutlineColor(sf::Color::Black);
        debug_ui_info.setOutlineThickness(1.f);
    }

    void UI::log(const std::string& msg) {
        debug_ui_text.append(msg + "\n");
        debug_ui_info.setString(debug_ui_text);
    }
    void UI::draw() {
        for (const auto& entry: debug_entries) {
            debug_ui_text.append("[" + entry.first + "]: \n");
            debug_ui_text.append(entry.second->to_string() + "\n");
        }
        debug_ui_info.setString(debug_ui_text);
        window.draw(debug_ui_info);
        debug_ui_text = "";
    }

    Console console;
    UI ui;
}