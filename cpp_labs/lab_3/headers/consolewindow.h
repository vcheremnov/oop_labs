#pragma once

#include "window.h"
#include <memory>
#include <ncurses.h>

enum class TextColor {
    Red, Green, Blue, White, Black
};

enum class TextBackgroundColor {

};

enum class TextAttr {
    Highlight,
    Dim,
    Bold,
    Underline,
    Blink
};

class ConsoleWindow: public Window {
public:
    // constructors & destructor
    ConsoleWindow();
    ConsoleWindow(size_type width, size_type height, pos line, pos col);
    ConsoleWindow(ConsoleWindow&, size_type width, size_type height, pos line, pos col);
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
    void print_text(const char *text);
    void print_text_at(pos line, pos col, const char *text);
    void set_attributes(const std::initializer_list<TextAttr>&);
    void reset_attributes(const std::initializer_list<TextAttr>&);
    // move & resize
    void resize(size_type width, size_type height);
private:
    size_type _width = 0, _height = 0;
    WINDOW *_win = nullptr;
    bool _hasColors = false;
};
