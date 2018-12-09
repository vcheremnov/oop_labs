#pragma once

#include "gamestate.h"
#include "ship.h"
#include "field.h"
#include <list>

class GameModel;

class GameData {
public:
    friend class GameModel;
    friend class MenuSelector;
    friend class ShipInitializer;
    friend class MoveMaker;

    using ShipList = std::list<Ship>;
    using FieldPair = std::pair<Field, Field>;
    // fields
    std::size_t ships_left(PlayerNumber player, Ship::Type type = Ship::Type::Total) const;
    bool field_is_hidden(PlayerNumber) const;
    void hide_field(PlayerNumber player, bool isHidden)
        { (player == PlayerNumber::Player1 ? _playerFieldIsHidden1 : _playerFieldIsHidden2) = isHidden; }
    const FieldPair &get_active_field_pair() const
        { return _fieldPairs.at(_activePlayer); }
    // game info
    GameMode get_gamemode() const
        { return _gameMode; }
    bool game_has_started() const
        { return _gameStarted; }
    bool game_has_finished() const;
    bool is_quit() const
        { return _isQuit; }
    GameState get_game_state() const
        { return _gameState; }
    // players info
    PlayerNumber get_active_player() const
        { return _activePlayer; }
    PlayerNumber get_inactive_player() const
        { return _inactivePlayer; }
    bool player_is_human(PlayerNumber) const;
    bool player_is_bot(PlayerNumber) const;
    Difficulty get_difficulty() const
        { return _difficulty; }
    Difficulty get_AI_level(PlayerNumber player) const
        { return player == PlayerNumber::Player1 ? _aiLevel1 : _aiLevel2; }
    // players' names
    const std::string &get_player_name(PlayerNumber) const;
    void set_human_player_name(PlayerNumber player, const std::string &name)
        { (player == PlayerNumber::Player1 ? _playerName1 : _playerName2) = name; }
    void set_bot_player_name(PlayerNumber player, const std::string &name)
        { (player == PlayerNumber::Player1 ? _botName1 : _botName2) = name; }
private:
    // private methods
    void _init_fields();
    void _add_ship(const Ship&, PlayerNumber);
    void _delete_ship(const Ship&, PlayerNumber);
    void _save_game_state()
        { _savedState = _gameState; }
    void _restore_game_state()
        { _gameState = _savedState; }
    void _set_game_state(GameState gameState)
        { _gameState = gameState; }
    void _set_game_started_flag(bool gameStarted)
        { _gameStarted = gameStarted; }
    void _next_player()
        { std::swap(_activePlayer, _inactivePlayer); }
    void _set_active_player(PlayerNumber activePlayer)
        { _activePlayer = activePlayer; }
    void _set_inactive_player(PlayerNumber inactivePlayer)
        { _inactivePlayer = inactivePlayer; }
    void _set_difficulty(Difficulty difficulty)
        { _difficulty = difficulty; }
    void _set_AI_level(PlayerNumber player, Difficulty aiLevel)
        { (player == PlayerNumber::Player1 ? _aiLevel1 : _aiLevel2) = aiLevel; }
    void _set_gamemode(GameMode gameMode)
        { _gameMode = gameMode; }
    void _quit()
        { _isQuit = true; }
    FieldPair &_get_field_pair_for(PlayerNumber player)
        { return _fieldPairs[player]; }
    ShipList &_get_ship_list_for(PlayerNumber player)
        { return _ships[player]; }

    // game state
    GameState _gameState = GameState::SplashScreen;
    GameState _savedState = GameState::MainMenu;
    GameMode _gameMode = GameMode::Player_vs_Bot;
    // game process
    bool _gameStarted = false;
    bool _isQuit = false;
    // active/inactive players
    const std::string _defaultName1 = "Player1";
    const std::string _defaultName2 = "Player2";
    std::string _botName1 = _defaultName1;
    std::string _botName2 = _defaultName2;
    std::string _playerName1 = _defaultName1;
    std::string _playerName2 = _defaultName2;
    PlayerNumber _activePlayer = PlayerNumber::Player1;
    PlayerNumber _inactivePlayer = PlayerNumber::Player2;
    Difficulty _difficulty = Difficulty::Easy;
    Difficulty _aiLevel1 = Difficulty::Easy, _aiLevel2 = Difficulty::Easy;
    // ships & fields
    bool _playerFieldIsHidden1 = false;
    bool _playerFieldIsHidden2 = false;
    std::map<PlayerNumber, FieldPair> _fieldPairs;
    std::map<PlayerNumber, ShipList> _ships;
};
