#include <fstream>
#include <sstream>
#include "consolescreen.h"

void ConsoleScreen::render(GameModel *model) {
    // draw background
    auto window = _get_console_window();
    // temporary auto resize fix
    window->resize(window->get_width(), window->get_height());
    window->clear();
    window->print_text("%s", _get_background().c_str());
    window->draw_border();
    window->refresh();
    // draw objects
    _render_objects(model);
}

void ConsoleScreen::_load_background(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open " + filename);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    _background = buffer.str();
}

