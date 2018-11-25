#pragma once

#include "eventlistener.h"
#include <map>

class Player {
public:
    virtual ~Player() = default;
    virtual void wait_event() = 0;
    void set_listener(EventListener *listener)
        { _listener = listener; }
protected:
    EventListener *_get_listener()
        { return _listener; }
private:
    EventListener *_listener = nullptr;
};

// human player

class HumanPlayer: public Player {
public:
    void wait_event() override;
};

// bot player

class BotPlayer: public Player {
public:
    void wait_event() override;
};
