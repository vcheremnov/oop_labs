#pragma once

#include "consolewindow.h"
#include <memory>

class GameModel;

class Object {
public:
    virtual ~Object() = default;
    virtual void render(GameModel*) = 0;
};

class ConsoleObject: public Object {
public:
    using size_type = Window::size_type;
    using pos = Window::pos;
    ConsoleObject(size_type width, size_type height, pos line, pos col):
        _win(new ConsoleWindow(width, height, line, col)) {}
protected:
    ConsoleWindow *_get_window()
        { return _win.get(); }
private:
    std::unique_ptr<ConsoleWindow> _win;
};
