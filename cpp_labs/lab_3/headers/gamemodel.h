#pragma once

#include <vector>
#include <utility>
#include <string>
#include <fstream>
#include <map>
#include <list>
#include <unordered_set>
#include "gameview.h"
#include <set>
#include "field.h"
#include "ship.h"
#include "player.h"
#include "gamestate.h"
#include "menuselector.h"
#include "shipinitializer.h"
#include "movemaker.h"

// human player stats
class PlayerStats {
public:
    using Score = int;
    void add_victory();
    void add_defeat();
    void save_stats(std::ofstream &file);
    void load_stats(std::ofstream &file);
    void change_player_name(const std::string &name);
    const std::string &get_player_name() const;
private:
    Score _victories = 0, _defeats = 0;
    std::string _playerName;
};

class GameModel {
public:
    friend class ShipInitializer;
    friend class MoveMaker;
    using FieldPair = std::pair<Field, Field>;
    // constructor
    GameModel();
    // view
    void attach_view(GameView*);
    void detach_view(GameView*);
    // game state
    void go_to_menu() {
        _state = GameState::MenuSelect;
        notify_views();
    }
    void start_ship_init() {
        _gameStarted = true;
        _state = GameState::ShipPlacement;
        _activePlayer = PlayerNumber::Player1;
        _init_fields();
        _shipInitializer.start_initialization();
        notify_views();
    }
    void start_game() {
        _gameStarted = true;
        _state = GameState::Battle;
        notify_views();
    }
    void finish_game();
    void quit()
        { _state = GameState::QuitScreen;
          notify_views(); }
    bool game_started()
        { return _gameStarted; }
    bool is_quit()
        { return _state == GameState::QuitScreen; }
    GameState get_game_state()
        { return _state; }
    PlayerNumber get_active_player()
        { return _activePlayer; }
    PlayerNumber get_inactive_player()
        { return _inactivePlayer; }
    const std::string &get_player_name(PlayerNumber);
    // field
    const FieldPair &get_field_pair() const
        { return _fieldPairs.at(_activePlayer); }
    // ships placement
    bool is_overlapping(const Ship&);
    // Model components //

    // menu selection
    MenuSelector &menu_selector()
        { return _menuSelector; }
    // ship initializer
    ShipInitializer &ship_initializer()
        { return _shipInitializer; }
    // move maker
    MoveMaker &move_maker()
        { return _moveMaker; }
// view
    void notify_views() {
        for (auto &view: _views) {
            view->update(this);
        }
    }

    // for debug purposes
    void set_game_state(GameState state)
        { _state = state; notify_views(); }
private:
    // game state
    GameState _state = GameState::SplashScreen;
    // game process
    bool _gameStarted = false;
    // views
    std::list<GameView*> _views;
    // active/inactive players
    std::string _playerName1 = "Player1";
    std::string _playerName2 = "Player2";
    PlayerNumber _activePlayer = PlayerNumber::Player1;
    PlayerNumber _inactivePlayer = PlayerNumber::Player2;
    void _next_player();
    // ship initialization
    void _init_fields();
    void _place_ship(const Ship&);
    void _remove_ship(const Ship&);
    std::map<PlayerNumber, FieldPair> _fieldPairs;
    using ShipList = std::list<Ship>;
    std::map<PlayerNumber, ShipList> _ships;
    // menu selector
    MenuSelector _menuSelector;
    // ship initializer
    ShipInitializer _shipInitializer;
    // move maker
    MoveMaker _moveMaker;
};
