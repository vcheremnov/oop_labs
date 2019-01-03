#pragma once

#include <memory>
#include "gameview/object.h"
#include "consolewindow.h"

class ConsoleObject: public Object {
public:
    using size_type = Window::size_type;
    using pos = Window::pos;
    void render(GameModel*) override;
    ConsoleObject(size_type width, size_type height, pos line, pos col):
        _win(new ConsoleWindow(width, height, line, col)) {}
protected:
    virtual void _draw_object(GameModel*) = 0;
    ConsoleWindow *_get_window()
        { return _win.get(); }
private:
    std::unique_ptr<ConsoleWindow> _win;
};
