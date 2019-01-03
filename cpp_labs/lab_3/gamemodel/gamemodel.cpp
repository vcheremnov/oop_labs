#include "gamemodel.h"
#include <random>
#include "game/player.h"

GameModel::GameModel():
    _gameData(new GameData()),
    _shipInitializer(new ShipInitializer(this)),
    _moveMaker(new MoveMaker(this)),
    _menuSelector(new MenuSelector(this)),
    _pauseMenuSelector(new PauseMenuSelector(this)) {

    _playerStats[PlayerNumber::Player1].reset(new PlayerStats());
    _playerStats[PlayerNumber::Player2].reset(new PlayerStats());
}

// game states

void GameModel::main_menu() {
    _gameData->_set_game_state(GameState::MainMenu);
    _menuSelector->reset();
    notify_observers();
}

void GameModel::start_ship_init() {
    _gameData->_set_game_started_flag(true);
    _gameData->_set_game_state(GameState::ShipPlacement);
    _gameData->_set_active_player(PlayerNumber::Player1);
    _gameData->_set_inactive_player(PlayerNumber::Player2);
    _gameData->_init_fields();
    _shipInitializer->start_initialization();
    // set names
    switch (_gameData->get_gamemode()) {
    case GameMode::Player_vs_Player: {
        auto &defaultName = _gameData->get_default_name();
        _gameData->set_player_name(PlayerNumber::Player1, defaultName + " #1");
        _gameData->set_player_name(PlayerNumber::Player2, defaultName + " #2");
        break;
    }
    case GameMode::Player_vs_Bot: {
        auto difficulty = _gameData->get_difficulty();
        auto &botName = BotPlayerFactory::instance().get_bot_name(difficulty);
        _gameData->set_player_name(PlayerNumber::Player1, _gameData->get_default_name());
        _gameData->set_player_name(PlayerNumber::Player2, botName);
        break;
    }
    case GameMode::Bot_vs_Bot: {
        auto aiLevel1 = _gameData->get_ai_level_1();
        auto aiLevel2 = _gameData->get_ai_level_2();
        auto botName1 = BotPlayerFactory::instance().get_bot_name(aiLevel1);
        auto botName2 = BotPlayerFactory::instance().get_bot_name(aiLevel2);
        if (botName1 == botName2) {
            botName1 += " #1";
            botName2 += " #2";
        }
        _gameData->set_player_name(PlayerNumber::Player1, botName1);
        _gameData->set_player_name(PlayerNumber::Player2, botName2);
        break;
    }
    default:
        break;
    }
    notify_observers();
}

void GameModel::start_game() {
    if (_gameData->get_game_state() == GameState::ShipPlacement) {
        _gameData->_set_game_started_flag(true);
        _gameData->_set_game_state(GameState::Battle);
        // reset stats
        _playerStats[PlayerNumber::Player1]->_reset_stats();
        _playerStats[PlayerNumber::Player2]->_reset_stats();

        notify_observers();
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
        notify_observers();
    }
}

void GameModel::controls_menu() {
    if (_gameData->get_game_state() == GameState::MainMenu) {
        _gameData->_set_game_state(GameState::ControlsMenu);
        notify_observers();
    }
}

void GameModel::stats() {
    auto gameState = _gameData->get_game_state();
    if (gameState == GameState::MainMenu || gameState == GameState::Battle) {
        _gameData->_set_game_state(GameState::Stats);
        notify_observers();
    }
}

void GameModel::pause() {
    auto gameState = _gameData->get_game_state();
    if (gameState == GameState::ShipPlacement || gameState == GameState::Battle) {
        _gameData->_save_game_state();
        _gameData->_set_game_state(GameState::Pause);
        _pauseMenuSelector->reset();
        notify_observers();
    }
}

void GameModel::unpause() {
    if (_gameData->get_game_state() == GameState::Pause) {
        _gameData->_restore_game_state();
        notify_observers();
    }
}

void GameModel::quit() {
    _gameData->_quit();
    notify_observers();
}
