#ifndef BASESCREEN_H
#define BASESCREEN_H

#include <vector>
#include <ncurses.h>
#include <cstddef>
#include "baseobject.h"

class BaseWindow {
public:
    // type names
    using pos = int;
    using size_type = int;
    // constructors
    BaseWindow(size_type width, size_type height, pos screenPosX, pos screenPosY):
        _width(width), _height(height), _screenPosX(screenPosX), _screenPosY(screenPosY) {}
    // public methods
    virtual void render_object(BaseObject &obj);
    size_type get_width()
        { return _width; }
    size_type get_height()
        { return _height; }
private:
    size_type _width = 0, _height = 0;
    pos _screenPosX = 0, _screenPosY = 0;
};

class ConsoleWindow: public BaseWindow {
public:
    ConsoleWindow(size_type width, size_type height, pos screenPosX, pos screenPosY):
        BaseWindow(width, height, screenPosX, screenPosY) {
        _win = ::newwin(height, width, screenPosY, screenPosX);
    }
    void render_object(BaseObject &obj) override;
    void clear()
        { ::wclear(_win); }
private:
    ::WINDOW *_win;
    pos _cursorPosX = 0, _cursorPosY = 0;
};

class BaseScreen {
public:
    void render();
private:
    std::vector<BaseObject*> _uiElements;
};

class MenuScreen: public BaseScreen {

};

#endif // BASESCREEN_H
