#pragma once

#include <memory>
#include "eventlistener.h"
#include "game/gamestate.h"
#include "gamemodel/gamedata.h"
#include "gamemodel/shipinitializer.h"
#include "gamemodel/movemaker.h"

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

// game controller factory

class BaseControllerCreator {
public:
    virtual ~BaseControllerCreator() = default;
    virtual GameController *get_game_controller(GameModel*) = 0;
};

template<typename ConcreteController>
class ControllerCreator: public BaseControllerCreator {
public:
    static_assert(std::is_base_of<GameController, ConcreteController>::value,
                  "Class ConcreteController has to inherit from GameController");
    GameController *get_game_controller(GameModel *model) override
        { return new ConcreteController(model); }
};

class ControllerFactory {
public:
    // singleton access
    static ControllerFactory &instance() {
        static ControllerFactory obj;
        return obj;
    }
    // registration
    template<typename ConcreteController>
    void register_controller(GameType gameType) {
         _registry[gameType].reset(new ControllerCreator<ConcreteController>());
    }
    // factory methods
    GameController *get_game_controller(GameType gameType, GameModel *model)
        { return _registry.at(gameType)->get_game_controller(model); }
private:
    ControllerFactory() = default;
    ControllerFactory(const ControllerFactory&) = delete;
    ControllerFactory &operator=(const ControllerFactory&) = delete;
    // registry
    std::map<GameType, std::unique_ptr<BaseControllerCreator>> _registry;
};
