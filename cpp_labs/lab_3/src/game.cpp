#include "game.h"
#include "gamemodel.h"
#include "menuselector.h"
#include <map>

// game factory

namespace {

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
    // create UI user
    _UIuser = std::make_unique<HumanPlayer>(_controller.get());
}

void Game::run() {
    // show splash screen
    _view->show();
    // game loop
    while (!_model->is_quit()) {
        if (_model->game_started()) {
            // check active player & switch
            _switch_active_player();
            // wait event from the active player
            _activePlayer->wait_event();
        }
        else {
            // wait event from the ui user
            _UIuser->wait_event();
            if (_model->game_started()) {
                _create_players();
            }
        }
        // rendering
        _view->show();
    }
    // quit screen
    _UIuser->wait_event();
}

void Game::_switch_active_player() {
    switch (_model->get_active_player()) {
    case PlayerNumber::Player1:
        _activePlayer = _player1;
        break;
    case PlayerNumber::Player2:
        _activePlayer = _player2;
        break;
    }
}

void Game::_create_players() {
    auto &menuSelector = _model->menu_selector();
    auto &botFactory = BotFactory::instance();
    switch (menuSelector.get_gamemode()) {
    case GameMode::Player_vs_Bot:
        _player1.reset(new HumanPlayer(_controller.get()));
        _player2.reset(botFactory.get_bot_player(menuSelector.get_difficulty(), _controller.get()));
        break;
    case GameMode::Player_vs_Player:
        _player1.reset(new HumanPlayer(_controller.get()));
        _player2.reset(new HumanPlayer(_controller.get()));
        break;
    case GameMode::Bot_vs_Bot:
        _player1.reset(botFactory.get_bot_player(menuSelector.get_AI_level_first(), _controller.get()));
        _player2.reset(botFactory.get_bot_player(menuSelector.get_AI_level_second(), _controller.get()));
        break;
    default:
        break;
    }
}
