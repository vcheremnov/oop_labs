#pragma once

#include <memory>
#include <set>
#include <ncurses.h>
#include "gameview/window.h"

enum class TextAttr {
    Highlight,
    Dim,
    Bold,
    Underline,
    Blink
};

enum class TextColor {
    RED_ON_BLACK = 1,
    GREEN_ON_BLACK,
    BLUE_ON_BLACK
};

class ConsoleWindow: public Window {
public:
    // constructors & destructor
    ConsoleWindow(size_type width, size_type height, pos line, pos col);
    ~ConsoleWindow() override;
    // get window sizes
    size_type get_width() override
        { return _width; }
    size_type get_height() override
        { return _height; }
    // clear & refresh window
    void clear() override;
    void refresh() override;
    void draw_border();
    // cursor position
    void move_cursor(pos line, pos col);
    // print text
    void add_character(const chtype);
    void add_character_at(pos line, pos col, const chtype);
    void print_text(const char *text, ...);
    void print_text_at(pos line, pos col, const char *text, ...);
    void set_attributes(const std::initializer_list<TextAttr>&);
    void reset_attributes(const std::initializer_list<TextAttr>&);
    void reset_attributes();
    void set_color_pair(TextColor);
    void reset_color_pair();
    // move & resize
    void resize(size_type width, size_type height);
private:
    size_type _width = 0, _height = 0;
    WINDOW *_win = nullptr;
    int _colorPair = -1;
    std::set<TextAttr> _attributes;
};
