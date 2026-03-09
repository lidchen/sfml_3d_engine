#ifndef DEBUG
#define DEBUG

#include <iostream>
#include <any>
#include <map>
#include <sstream>
#include <iomanip>
#include "SFML/Graphics.hpp"

extern bool isMousePressedOnce(sf::Mouse::Button button);
extern sf::Font Alkhemikal;
extern sf::RenderWindow window;

struct Debugable {
    virtual std::string to_string() const = 0;
    virtual ~Debugable() = default;
};

template<typename T>
struct DebugWrapper : public Debugable {
    const T* ptr;
    DebugWrapper(const T* p) : ptr(p) {}

    std::string to_string() const override {
        if constexpr (std::is_same_v<T, float> || std::is_same_v<T, int>) {
            std::ostringstream oss;
            oss << std::setprecision(3);
            oss << *ptr;
            return oss.str();
        } else {
            return ptr->to_string();  // requires T has to_string()
        }
    }
};

class Button {
    public:
        Button(const std::string& name, const sf::Vector2f& pos, bool& var);
        void draw() const;
        void update();
        void press();
    private:
        int button_fontsize = 18;
        float width;
        float height;
        bool *var;
        const float height_scale = 1.5;
        std::string name;
        sf::RectangleShape button_shape;
        sf::Text button_text;
        sf::Color text_color = sf::Color::White;
        sf::Color button_color = sf::Color::White;
};



namespace debug {
    class Console {
    public:
        template<typename T>
        void log(const T& msg) {
            std::cout << "[DEBUG]: " << msg << "\n";
        }
    };

    class UI {
    public:
        UI();
        void log(const std::string& msg);
        template<typename T>
        void register_var(const std::string& name, const T* var_ptr) {
            debug_entries[name] = std::make_shared<DebugWrapper<T>>(var_ptr);
        }
        void draw();
    private:
        sf::Text debug_ui_info;
        std::string debug_ui_text;
        std::map<std::string, std::shared_ptr<Debugable>> debug_entries;
    };

    extern Console console;
    extern UI ui;
}

#endif