#include "gamemodel.h"
#include <random>
#include "player.h"

GameModel::GameModel():
    _gameData(new GameData()),
    _shipInitializer(new ShipInitializer(this)),
    _moveMaker(new MoveMaker(this)),
    _menuSelector(new MenuSelector(this)),
    _pauseMenuSelector(new PauseMenuSelector(this)) {

    _playerStats[PlayerNumber::Player1].reset(new PlayerStats());
    _playerStats[PlayerNumber::Player2].reset(new PlayerStats());
}

// view

void GameModel::attach_view(GameView *view) {
    if (view == nullptr) {
        throw std::runtime_error("GameModel::attach_view() error: view is NULL");
    }
    _views.push_back(view);
}
void GameModel::detach_view(GameView *view) {
    _views.remove(view);
}

void GameModel::notify_views() {
    for (auto &view: _views) {
        view->update(this);
    }
}

// game states

void GameModel::main_menu() {
    _gameData->_set_game_state(GameState::MainMenu);
    _menuSelector->reset();
    notify_views();
}

void GameModel::start_ship_init() {
    _gameData->_set_game_started_flag(true);
    _gameData->_set_game_state(GameState::ShipPlacement);
    _gameData->_set_active_player(PlayerNumber::Player1);
    _gameData->_set_inactive_player(PlayerNumber::Player2);
    _gameData->_init_fields();
    _shipInitializer->start_initialization();
    notify_views();
}

void GameModel::start_game() {
    if (_gameData->get_game_state() == GameState::ShipPlacement) {
        _gameData->_set_game_started_flag(true);
        _gameData->_set_game_state(GameState::Battle);
        // set bot names
        switch (_gameData->get_gamemode()) {
        case GameMode::Player_vs_Bot: {
            auto difficulty = _gameData->get_difficulty();
            auto &botName = BotPlayerFactory::instance().get_bot_player_name(difficulty);
            _gameData->set_bot_player_name(PlayerNumber::Player2, botName);
            break;
        }
        case GameMode::Bot_vs_Bot: {
            auto aiLevel1 = _gameData->get_AI_level(PlayerNumber::Player1);
            auto aiLevel2 = _gameData->get_AI_level(PlayerNumber::Player2);
            auto &botName1 = BotPlayerFactory::instance().get_bot_player_name(aiLevel1);
            auto &botName2 = BotPlayerFactory::instance().get_bot_player_name(aiLevel2);
            _gameData->set_bot_player_name(PlayerNumber::Player1, botName1 + "1");
            _gameData->set_bot_player_name(PlayerNumber::Player2, botName2 + "2");
            break;
        }
        default:
            break;
        }
        // reset stats
        _playerStats[PlayerNumber::Player1]->_reset_stats();
        _playerStats[PlayerNumber::Player2]->_reset_stats();

        notify_views();
    }
}

void GameModel::finish_game() {
    auto gameState = _gameData->get_game_state();
    if (gameState == GameState::Battle || gameState == GameState::Pause) {
        _gameData->_set_game_started_flag(false);
        _gameData->_set_game_state(GameState::Stats);
        _playerStats.at(_gameData->get_active_player())->_set_winner();
        // reset last move info
        _moveMaker->proceed();
        notify_views();
    }
}

void GameModel::controls_menu() {
    if (_gameData->get_game_state() == GameState::MainMenu) {
        _gameData->_set_game_state(GameState::ControlsMenu);
        notify_views();
    }
}

void GameModel::stats() {
    auto gameState = _gameData->get_game_state();
    if (gameState == GameState::MainMenu || gameState == GameState::Battle) {
        _gameData->_set_game_state(GameState::Stats);
        notify_views();
    }
}

void GameModel::pause() {
    auto gameState = _gameData->get_game_state();
    if (gameState == GameState::ShipPlacement || gameState == GameState::Battle) {
        _gameData->_save_game_state();
        _gameData->_set_game_state(GameState::Pause);
        _pauseMenuSelector->reset();
        notify_views();
    }
}

void GameModel::unpause() {
    if (_gameData->get_game_state() == GameState::Pause) {
        _gameData->_restore_game_state();
        notify_views();
    }
}

void GameModel::quit() {
    _gameData->_quit();
    notify_views();
}
