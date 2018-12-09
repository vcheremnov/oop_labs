#pragma once

#include "gamestate.h"
#include "eventlistener.h"
#include <memory>
#include "movemaker.h"
#include "shipinitializer.h"
#include "gamedata.h"

class GameModel;

class GameController {
public:
    GameController(GameModel*);
    virtual ~GameController() = default;
    const GameData &get_game_data();
    ShipInitializer &get_ship_initializer();
    MoveMaker &get_move_maker();
    virtual EventListener *get_event_listener() = 0;
protected:
    GameModel *_get_model()
        { return _model; }
private:
    GameModel *_model = nullptr;
};

class ConsoleController: public GameController {
public:
    ConsoleController(GameModel*);
    EventListener *get_event_listener() override;
private:
    // handlers registry
    std::map<GameState, std::unique_ptr<EventListener>> _listeners;
};
