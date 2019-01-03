#pragma once

#include <map>
#include "basescreen.h"
#include "observer_pattern.h"
#include "game/gamestate.h"

class GameModel;

class GameView: public Observer {
public:
    GameView(GameModel*);
    virtual void show() = 0;
protected:
    GameModel *_get_model()
        { return _model; }
private:
    GameModel *_model = nullptr;
};

// game view factory

class BaseViewCreator {
public:
    virtual ~BaseViewCreator() = default;
    virtual GameView *get_game_view(GameModel*) = 0;
};

template<typename ConcreteView>
class ViewCreator: public BaseViewCreator {
public:
    static_assert(std::is_base_of<GameView, ConcreteView>::value,
                  "Class ConcreteView has to inherit from GameView");
    GameView *get_game_view(GameModel *model) override
        { return new ConcreteView(model); }
};

class GameViewFactory {
public:
    // singleton access
    static GameViewFactory &instance() {
        static GameViewFactory obj;
        return obj;
    }
    // registration
    template<typename ConcreteView>
    void register_controller(GameType gameType) {
         _registry[gameType].reset(new ViewCreator<ConcreteView>());
    }
    // factory methods
    GameView *get_game_view(GameType gameType, GameModel *model)
        { return _registry.at(gameType)->get_game_view(model); }
private:
    GameViewFactory() = default;
    GameViewFactory(const GameViewFactory&) = delete;
    GameViewFactory &operator=(const GameViewFactory&) = delete;
    // registry
    std::map<GameType, std::unique_ptr<BaseViewCreator>> _registry;
};
