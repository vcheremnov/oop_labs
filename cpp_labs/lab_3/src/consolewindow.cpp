#include "consolewindow.h"
#include <ncurses.h>
#include <stdexcept>
#include <cstdarg>

namespace {

decltype(A_BOLD) TextAttr_to_A(TextAttr attr) {
    switch (attr) {
    case TextAttr::Blink:
        return A_BLINK;
    case TextAttr::Bold:
        return A_BOLD;
    case TextAttr::Dim:
        return A_DIM;
    case TextAttr::Highlight:
        return A_REVERSE;
    case TextAttr::Underline:
        return A_UNDERLINE;
    default:
        return A_NORMAL;
    }
}

}

// constructors & destructor

ConsoleWindow::ConsoleWindow(size_type width, size_type height, pos line, pos col):
    _win(newwin(height, width, line, col)) {
    getmaxyx(_win, _height, _width);
}

ConsoleWindow::~ConsoleWindow() {
    delwin(_win);
}

// public methods

void ConsoleWindow::clear() {
    wclear(_win);
}

void ConsoleWindow::refresh() {
    wrefresh(_win);
}

void ConsoleWindow::draw_border() {
    box(_win, 0, 0);
}

void ConsoleWindow::move_cursor(pos line, pos col) {
    wmove(_win, line, col);
}

void ConsoleWindow::add_character(const chtype ch) {
    waddch(_win, ch);
}

void ConsoleWindow::add_character_at(pos line, pos col, const chtype ch) {
    mvwaddch(_win, line, col, ch);
}

void ConsoleWindow::print_text(const char *text, ...) {
    va_list args;
    va_start(args, text);
    vw_printw(_win, text, args);
    va_end(args);
}

void ConsoleWindow::print_text_at(pos line, pos col, const char *text, ...) {
    move_cursor(line, col);
    va_list args;
    va_start(args, text);
    vw_printw(_win, text, args);
    va_end(args);

}

void ConsoleWindow::set_attributes(const std::initializer_list<TextAttr> &attrList) {
    for (auto &attr: attrList) {
        wattron(_win, TextAttr_to_A(attr));
    }
}

void ConsoleWindow::reset_attributes(const std::initializer_list<TextAttr> &attrList) {
    for (auto &attr: attrList) {
        wattroff(_win, TextAttr_to_A(attr));
    }
}

void ConsoleWindow::set_color_pair(TextColor colorPair) {
    _colorPair = static_cast<int>(colorPair);
    wattron(_win, COLOR_PAIR(_colorPair));
}

void ConsoleWindow::reset_color_pair() {
    if (_colorPair != -1) {
        wattroff(_win, COLOR_PAIR(_colorPair));
    }
    _colorPair = -1;
}

// move & resize

void ConsoleWindow::resize(size_type width, size_type height) {
    wresize(_win, height, width);
    getmaxyx(_win, _height, _width);
}
