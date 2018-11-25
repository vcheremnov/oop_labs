#include <fstream>
#include <sstream>
#include "basescreen.h"

void ConsoleScreen::_load_background(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open " + filename);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    _background = buffer.str();
}

