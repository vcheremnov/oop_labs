#include "consolewindow.h"
#include <ncurses.h>
#include <stdexcept>

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

ConsoleWindow::ConsoleWindow() {
    use_env(false);
    _win = initscr();
    getmaxyx(_win, _height, _width);
    cbreak();
    noecho();
    keypad(_win, true);
    curs_set(0);
}

ConsoleWindow::ConsoleWindow(size_type width, size_type height, pos line, pos col):
    _win(newwin(height, width, line, col)) {
    getmaxyx(_win, _height, _width);
}

ConsoleWindow::ConsoleWindow(ConsoleWindow &win, size_type width, size_type height,
                             pos line, pos col):
    _win(subwin(win._win, height, width, line, col)) {
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

void ConsoleWindow::print_text(const char *text) {
    wprintw(_win, "%s", text);
}

void ConsoleWindow::print_text_at(pos line, pos col, const char *text) {
    mvwprintw(_win, line, col, "%s", text);
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

// move & resize

void ConsoleWindow::resize(size_type width, size_type height) {
    wresize(_win, height, width);
    getmaxyx(_win, _height, _width);
}
