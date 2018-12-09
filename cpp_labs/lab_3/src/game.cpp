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
    _gameType(gameType),
    _model{new GameModel()},
    _view{GameFactory::instance().get_view(gameType, _model.get())},
    _controller{GameFactory::instance().get_controller(gameType, _model.get())} {

    // register view as model's observer
    _model->attach_view(_view.get());
    // create UI user
    _UIuser.reset(HumanPlayerFactory::instance().get_human_player(_gameType, _controller.get()));
}

void Game::run() {
    auto &gameData = _model->game_data();
    // game loop
    while (!gameData.is_quit()) {
        // rendering
        _view->show();
        // event handling
        if (gameData.game_has_started()) {
            if (_model->move_maker().move_was_made() || gameData.game_has_finished()) {
                // wait the ui user to continue the game
                _UIuser->wait_event();
            }
            else {
                // switch to the active player
                _switch_active_player();
                // wait event from the active player
                _activePlayer->wait_event();
            }
        }
        else {
            // wait event from the ui user
            _UIuser->wait_event();
            if (gameData.game_has_started()) {
                // ui user has started the game
                _create_players();
            }
        }
    }
}

void Game::_switch_active_player() {
    switch (_model->game_data().get_active_player()) {
    case PlayerNumber::Player1:
        _activePlayer = _player1;
        break;
    case PlayerNumber::Player2:
        _activePlayer = _player2;
        break;
    }
}

void Game::_create_players() {
    auto &gameData = _model->game_data();
    auto &humanPlayerFactory = HumanPlayerFactory::instance();
    auto &botPlayerFactory = BotPlayerFactory::instance();
    switch (gameData.get_gamemode()) {
    case GameMode::Player_vs_Bot:
        _player1.reset(humanPlayerFactory.get_human_player(_gameType, _controller.get()));
        _player2.reset(botPlayerFactory.get_bot_player(gameData.get_difficulty(), _controller.get()));
        break;
    case GameMode::Player_vs_Player:
        _player1.reset(humanPlayerFactory.get_human_player(_gameType, _controller.get()));
        _player2.reset(humanPlayerFactory.get_human_player(_gameType, _controller.get()));
        break;
    case GameMode::Bot_vs_Bot:
        _player1.reset(botPlayerFactory.get_bot_player(gameData.get_AI_level(PlayerNumber::Player1), _controller.get()));
        _player2.reset(botPlayerFactory.get_bot_player(gameData.get_AI_level(PlayerNumber::Player2), _controller.get()));
        break;
    default:
        break;
    }
}
