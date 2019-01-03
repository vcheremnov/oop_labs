#pragma once

#include "consoleobject.h"

class ConsoleMenu: public ConsoleObject {
public:
    using ConsoleObject::ConsoleObject;
protected:
    virtual const std::string& _get_header() = 0;
    virtual void _draw_options(GameModel*) = 0;
    void _draw_object(GameModel*) override;
    int _get_starting_line()
        { return _startY; }
private:
    static const int _startY = 3;
};
