#include "game.h"
#include <map>

class GameModel;

// game factory

class GameCreator {
public:
    virtual ~GameCreator() = default;
    virtual GameController *get_controller(GameModel*) = 0;
    virtual GameView *get_view(GameModel*) = 0;
};

class ConsoleGameCreator: public GameCreator {
public:
    ConsoleController *get_controller(GameModel *model) override
        { return new ConsoleController(model); }
    ConsoleView *get_view(GameModel *model) override
        { return new ConsoleView(model); }
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
    GameController *get_controller(GameType gameType, GameModel *model)
        { return _registry[gameType]->get_controller(model); }
    GameView *get_view(GameType gameType, GameModel *model)
        { return _registry[gameType]->get_view(model); }
private:
    GameFactory() = default;
    GameFactory(const GameFactory&) = delete;
    GameFactory &operator=(const GameFactory&) = delete;
    // registry
    std::map<GameType, std::unique_ptr<GameCreator>> _registry;
};

namespace {

bool register_game_types() {
    auto &factory = GameFactory::instance();
    factory.register_creator<ConsoleGameCreator>(GameType::ConsoleGame);
    return true;
}

bool regCreators = register_game_types();

}   // anonymous namespace

// Game

Game::Game(GameType gameType):
    _model{new GameModel()},
    _view{GameFactory::instance().get_view(gameType, _model.get())},
    _controller{GameFactory::instance().get_controller(gameType, _model.get())} {

    // register view as model's observer
    _model->attach_view(_view.get());
    // players (KOSTYYYYYL)
    _humanPlayer = std::make_shared<HumanPlayer>();
    _botPlayer = std::make_shared<BotPlayer>();
    // bind players to the controller
    _controller->bind_to_human_player(_humanPlayer.get());
    _controller->bind_to_bot_player(_botPlayer.get());
}

void Game::run() {
    // show splash screen
    _view->show();
    // game loop
    while (!_model->is_quit()) {
        _controller->switch_listener(_model->get_game_state());
        if (_model->game_started()) {
            // check active player & switch
            _switch_active_player();
            // wait event from the active player
            _activePlayer->wait_event();
        }
        else {
            // wait event from the ui user
            _humanPlayer->wait_event();
        }
        // rendering
        _view->show();
    }
}

void Game::_switch_active_player() {
    switch (_model->get_active_player()) {
    case ActivePlayer::Player1:
        _activePlayer = _humanPlayer;
        break;
    case ActivePlayer::Player2:
        _activePlayer = _botPlayer;
        break;
    }
}
