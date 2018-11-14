#include "game.h"
#include <map>

// game factory

class GameCreator {
public:
    virtual ~GameCreator() = default;
    virtual std::shared_ptr<GameController> get_controller() = 0;
    virtual std::shared_ptr<GameView> get_view() = 0;
};

template<typename Controller, typename View>
class ConcreteGameCreator: public GameCreator {
    static_assert(std::is_base_of<GameController, Controller>::value,
                  "Class Controller has to inherit from GameController");
    static_assert(std::is_base_of<GameView, View>::value,
                  "Class View has to inherit from GameView");
    std::shared_ptr<GameController> get_controller()
        { return std::make_shared<Controller>(); }
    std::shared_ptr<GameView> get_view()
        { return std::make_shared<View>(); }
};

class GameFactory {
public:
    // singleton access
    static GameFactory &instance() {
        static GameFactory obj;
        return obj;
    }
    // registration
    template<typename Creator>
    void register_creator(GameType gameType) {
         static_assert(std::is_base_of<GameCreator, Creator>::value,
                       "Class Creator has to inherit from GameCreator");
         _registry[gameType].reset(new Creator());
    }
    void forget_creator(GameType gameType) {
        _registry.erase(gameType);
    }
    // factory methods
    std::shared_ptr<GameController> get_controller(GameType gameType)
        { return _registry[gameType]->get_controller(); }
    std::shared_ptr<GameView> get_view(GameType gameType)
        { return _registry[gameType]->get_view(); }
private:
    GameFactory() = default;
    GameFactory(const GameFactory&) = delete;
    GameFactory &operator=(const GameFactory&) = delete;
    // registry
    std::map<GameType, std::unique_ptr<GameCreator>> _registry;
};

// Game

Game::Game(GameType gameType):
    _controller{GameFactory::instance().get_controller(gameType)},
    _view{GameFactory::instance().get_view(gameType)} {

}

void Game::run() {

}
