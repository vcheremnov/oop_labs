#pragma once

#include "field.h"
#include <utility>

class Event {
public:
    virtual ~Event() = default;
};

// console key press

enum class KeyCode {
    KeyUP,
    KeyDOWN,
    KeyLEFT,
    KeyRIGHT,
    KeyENTER,
    KeySTAB,
    KeySPACE,
    KeyF5,
    KeyUNKNOWN
};

class KeyEvent: public Event {
public:
    KeyEvent(KeyCode keyCode):
        _keyCode(keyCode) {}
    KeyCode get_keycode() const
        { return _keyCode; }
private:
    KeyCode _keyCode;
};

class MouseEvent: public Event {
public:
};

// bot game move

class BotEvent: public Event {
public:
};

class BotMoveEvent: public BotEvent {
public:
    BotMoveEvent(Field::pos x, Field::pos y):
        _x(x), _y(y) {}
    std::pair<Field::pos, Field::pos> get_shot_pos()
        { return {_x, _y}; }
private:
    Field::pos _x, _y;

};

