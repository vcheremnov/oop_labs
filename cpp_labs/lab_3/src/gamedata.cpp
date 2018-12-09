#include "gamedata.h"
#include <algorithm>


bool GameData::game_has_finished() const {
    return _gameState == GameState::Battle &&
           (_ships.at(_activePlayer).size() == 0 || _ships.at(_inactivePlayer).size() == 0);
}

bool GameData::player_is_human(PlayerNumber player) const {
    return _gameMode == GameMode::Player_vs_Player ||
            (_gameMode == GameMode::Player_vs_Bot && player == PlayerNumber::Player1);
}

bool GameData::player_is_bot(PlayerNumber player) const {
    return _gameMode == GameMode::Bot_vs_Bot ||
            (_gameMode == GameMode::Player_vs_Bot && player == PlayerNumber::Player2);
}

bool GameData::field_is_hidden(PlayerNumber player) const {
    return (player == PlayerNumber::Player1) ? _playerFieldIsHidden1 : _playerFieldIsHidden2;
}

std::size_t GameData::ships_left(PlayerNumber player, Ship::Type type) const {
    if (type == Ship::Type::Total) {
        return _ships.at(player).size();
    }
    return std::count_if(_ships.at(player).begin(), _ships.at(player).end(),
                         [type](const Ship &ship) { return ship.get_type() == type; });
}

const std::string& GameData::get_player_name(PlayerNumber player) const {
    if (player == PlayerNumber::Player1) {
        return player_is_human(player) ? _playerName1 : _botName1;
    }
    return player_is_human(player) ? _playerName2 : _botName2;
}

void GameData::_init_fields() {
    // clear fields
    _fieldPairs[PlayerNumber::Player1].first.clear();
    _fieldPairs[PlayerNumber::Player1].second.clear(Field::Cell::Unknown);
    _fieldPairs[PlayerNumber::Player2].first.clear();
    _fieldPairs[PlayerNumber::Player2].second.clear(Field::Cell::Unknown);
    // hide players' fields
    switch (_gameMode) {
    case GameMode::Bot_vs_Bot:
        _playerFieldIsHidden1 = false;
        _playerFieldIsHidden2 = false;
        break;
    case GameMode::Player_vs_Bot:
        _playerFieldIsHidden1 = false;
        _playerFieldIsHidden2 = true;
        break;
    case GameMode::Player_vs_Player:
        _playerFieldIsHidden1 = true;
        _playerFieldIsHidden2 = true;
        break;
    default:
        break;
    }
    // clear ships
    _ships.clear();
}

void GameData::_add_ship(const Ship &ship, PlayerNumber player) {
    _ships[player].push_back(ship);
}

void GameData::_delete_ship(const Ship &ship, PlayerNumber player) {
    _ships[player].remove(ship);
}
