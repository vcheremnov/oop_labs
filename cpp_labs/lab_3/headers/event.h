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
    KeyTAB,
    KeySPACE,
    KeyBACKSPACE,
    KeyQ,
    KeyE,
    KeyH,
    KeyZ,
    KeyR,
    KeyP,
    KeyUNKNOWN
};

class KeyEvent: public Event {
public:
    KeyEvent(KeyCode keyCode, int rawCode, bool isSpecialKey):
        _keyCode(keyCode), _rawCode(rawCode), _isSpecialKey(isSpecialKey) {}
    KeyCode get_keycode() const
        { return _keyCode; }
    int get_raw_code() const
        { return _rawCode; }
    bool is_special_key() const
        { return _isSpecialKey; }
private:
    KeyCode _keyCode;
    int _rawCode;
    bool _isSpecialKey;
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

