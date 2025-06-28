#ifndef DEBUG
#define DEBUG

#include <iostream>
#include "variable.h"
#include "SFML/Graphics.hpp"


namespace debug {
    class Console {
    public:
        void log(const std::string& msg);
    };

    class UI {
    public:
        UI();
        void log(const std::string& msg);
        void draw();
    private:
        sf::Text debug_ui_info;
        std::string debug_ui_text;
    };

    extern Console console;
    extern UI ui;
}

#endif