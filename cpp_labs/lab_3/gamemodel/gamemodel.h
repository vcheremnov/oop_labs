#pragma once

#include <list>
#include <memory>
#include "menuselector.h"
#include "shipinitializer.h"
#include "gamedata.h"
#include "movemaker.h"
#include "playerstats.h"
#include "pausemenuselector.h"
#include "observer_pattern.h"

class GameModel: public Subject {
public:
    friend class MenuSelector;
    friend class ShipInitializer;
    friend class MoveMaker;
    using FieldPair = GameData::FieldPair;
    // constructor
    GameModel();
    // game state
    void main_menu();
    void controls_menu();
    void stats();
    void pause();
    void unpause();
    void start_ship_init();
    void start_game();
    void finish_game();
    void quit();
    // Model components
    GameData &game_data()
        { return *_gameData; }
    ShipInitializer &ship_initializer()
        { return *_shipInitializer; }
    MoveMaker &move_maker()
        { return *_moveMaker; }
    PlayerStats &player_stats(PlayerNumber player)
        { return *_playerStats.at(player); }
    // menu selectors
    MenuSelector &menu_selector()
        { return *_menuSelector; }
    PauseMenuSelector &pause_menu_selector()
        { return *_pauseMenuSelector; }
private:
    // game model components
    std::unique_ptr<GameData> _gameData;
    std::unique_ptr<ShipInitializer> _shipInitializer;
    std::unique_ptr<MoveMaker> _moveMaker;
    std::map<PlayerNumber, std::unique_ptr<PlayerStats>> _playerStats;
    // menu selectors
    std::unique_ptr<MenuSelector> _menuSelector;
    std::unique_ptr<PauseMenuSelector> _pauseMenuSelector;
};
