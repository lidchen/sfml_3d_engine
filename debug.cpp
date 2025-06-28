#include "debug.h"

namespace debug {
    void Console::log(const std::string& msg) {
        std::cout << "[DEBUG]: "<< msg << "\n";
    }
    UI::UI() {
        debug_ui_info.setFont(Alkhemikal);
        debug_ui_info.setCharacterSize(24);
        debug_ui_info.setPosition(0, 10);
    }
    void UI::log(const std::string& msg) {
        debug_ui_text.append(msg + "\n");
        debug_ui_info.setString(debug_ui_text);
    }
    void UI::draw() {
        window.draw(debug_ui_info);
    }

    Console console;
    UI ui;
}