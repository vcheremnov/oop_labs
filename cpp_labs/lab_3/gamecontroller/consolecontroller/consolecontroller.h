#pragma once

#include "gamecontroller/gamecontroller.h"

class ConsoleController: public GameController {
public:
    ConsoleController(GameModel*);
    EventListener *get_event_listener() override;
private:
    // handlers registry
    std::map<GameState, std::unique_ptr<EventListener>> _listeners;
};
