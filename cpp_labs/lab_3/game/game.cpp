#include "game.h"
#include <map>

Game::Game(GameType gameType):
    _gameType(gameType),
    _model{new GameModel()},
    _view{GameViewFactory::instance().get_game_view(gameType, _model.get())},
    _controller{ControllerFactory::instance().get_game_controller(gameType, _model.get())} {

    // register view as model's observer
    _model->attach_observer(_view.get());
    _model->notify_observers();
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
        _player1.reset(botPlayerFactory.get_bot_player(gameData.get_ai_level_1(), _controller.get()));
        _player2.reset(botPlayerFactory.get_bot_player(gameData.get_ai_level_2(), _controller.get()));
        break;
    default:
        break;
    }
}
