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

enum class ShiftDirection {
    Left, Right, Up, Down
};

class GameModel {
public:
    friend class ShipInitializer;
    // constructor
    GameModel();
    // menu selection
    MenuSelector &menu_selector()
        { return _menuSelector; }
    // view
    void attach_view(GameView *view) {
        if (view == nullptr) {
            throw std::runtime_error("VIEW IS NULL...");
        }
        _views.push_back(view);
    }
    void detach_view(GameView *view) {
        _views.remove(view);
    }
    // game state
    void go_to_menu() {
        _state = GameState::MenuSelect;
        notify_views();
    }
    void start_ship_init() {
        _state = GameState::ShipPlacement;
        _activePlayer = ActivePlayer::Player1;
        _init_fields();
        _shipInitializer.start_initialization(_activePlayer);
        notify_views();
    }
    void start_game() {
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
    ActivePlayer get_active_player()
        { return _activePlayer; }
    // field
    void change_pos(ShiftDirection);
    void make_shot();
    // ships placement
    bool is_overlapping(const Ship&);
    bool accept_choice();
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
    // active player
    ActivePlayer _activePlayer;
    void _next_player();
    // ship initialization
    void _init_fields();
    bool _place_ship(const Ship&);
    void _remove_ship(const Ship&);
    using FieldPair = std::pair<Field, Field>;
    std::map<ActivePlayer, FieldPair> _fieldPairs;
    using ShipList = std::list<Ship>;
    std::map<ActivePlayer, ShipList> _ships;
    // ship initializer
    ShipInitializer _shipInitializer;
    // menu selector
    MenuSelector _menuSelector;
};
