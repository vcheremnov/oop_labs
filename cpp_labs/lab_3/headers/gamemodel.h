#pragma once

#include <vector>
#include <utility>
#include <string>
#include <fstream>
#include <map>
#include <list>
#include "gameview.h"
#include <set>
#include "field.h"
#include "player.h"
#include "gamestate.h"
#include "menuselector.h"

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

enum class ActivePlayer {
    Player1, Player2
};

class GameModel {
public:
    // constructor
    GameModel() = default;
    // menu selection
    MenuSelector &get_menu_selector()
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
        notify();
    }
    void start_game() {
        _state = GameState::ShipPlacement;
        notify();
    }
    void finish_game();
    void quit()
        { _isQuit = true;
          _state = GameState::QuitScreen;
          notify(); }
    bool game_started()
        { return _gameStarted; }
    bool is_quit()
        { return _isQuit; }
    void set_game_state(GameState state)
        { _state = state; notify(); }
    GameState get_game_state()
        { return _state; }
    ActivePlayer get_active_player()
        { return _activePlayer; }
    // field
    void change_pos(ShiftDirection);
    void make_shot();
    // ships placement
    void shift_ship(ShiftDirection);
    void rotate_ship();
    void place_ship();
//    void remove_ship();
    void change_ship();
    void accept_choice();
// view
    void notify() {
        for (auto &view: _views) {
            view->update(this);
        }
    }
private:
    // view
    std::list<GameView*> _views;
    // ship initialization
    void _init_fields();
    // game process
    bool _gameStarted = false;
    ActivePlayer _activePlayer;
//    void _random_player_choice();
    void _next_player();
    Field::pos _posX, _posY;
    static const int PLAYER_NUMBER = 2;
    using FieldPair = std::pair<Field, Field>;
    std::vector<FieldPair> _fields{PLAYER_NUMBER};
    using ShipSet = std::map<Ship::Type, Ship>;
    std::vector<ShipSet> _ships{PLAYER_NUMBER};
    // players info
    PlayerStats _stats;
    // game state
    GameState _state = GameState::SplashScreen;
    bool _isQuit = false;
    // menu selector
    MenuSelector _menuSelector;
};
