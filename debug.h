#ifndef DEBUG
#define DEBUG

#include <iostream>
#include <any>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include "SFML/Graphics.hpp"

extern sf::Font Alkhemikal;
extern sf::RenderWindow window;

struct Debugable {
    virtual std::string to_string() const = 0;
    virtual ~Debugable() = default;
};

template<typename T>
struct DebugWrapper : public Debugable {
    T* ptr;
    DebugWrapper(T* p) : ptr(p) {}

    std::string to_string() const override {
        if constexpr (std::is_same_v<T, float>) {
            std::ostringstream oss;
            oss << std::setprecision(3);
            oss << *ptr;
            oss << "\n";
            return oss.str();
        } else {
            return ptr->to_string();  // requires T has to_string()
        }
    }
};


namespace debug {
    class Console {
    public:
        void log(const std::string& msg);
    };

    class UI {
    public:
        UI();
        void log(const std::string& msg);
        template<typename T>
        void register_var(const std::string& name, T* var_ptr) {
            debug_entries[name] = std::make_shared<DebugWrapper<T>>(var_ptr);
        }
        void draw();
    private:
        sf::Text debug_ui_info;
        std::string debug_ui_text;
        std::unordered_map<std::string, std::shared_ptr<Debugable>> debug_entries;
    };

    extern Console console;
    extern UI ui;
}

#endif