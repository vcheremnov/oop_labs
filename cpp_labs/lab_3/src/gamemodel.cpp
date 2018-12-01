#include "gamemodel.h"
#include <random>

//void GameModel::_random_player_choice() {
//    std::default_random_engine generator;
//    std::uniform_int_distribution<int> distribution(1,2);
//    switch (distribution(generator)) {
//    case 1:
//        _activePlayer = ActivePlayer::Player1;
//        break;
//    case 2:
//        _activePlayer = ActivePlayer::Player2;
//        break;
//    }
//}

GameModel::GameModel():
    _menuSelector(this),
    _shipInitializer(this),
    _moveMaker(this) {

    // debug
    _init_fields();
}

void GameModel::_next_player() {
//    std::swap(_activePlayer, _inactivePlayer);
}

void GameModel::_init_fields() {
    // clear fields
    _fieldPairs[PlayerNumber::Player1].first.clear();
    _fieldPairs[PlayerNumber::Player1].second.clear(Field::Cell::Unknown);
    _fieldPairs[PlayerNumber::Player2].first.clear();
    _fieldPairs[PlayerNumber::Player2].second.clear(Field::Cell::Unknown);
    // clear ships
    _ships.clear();
}

void GameModel::_place_ship(const Ship &ship) {
    _ships[_activePlayer].push_back(ship);
    auto coordinates = ship.get_body();
    for (auto &posPair: coordinates) {
        auto &field = _fieldPairs[_activePlayer].first;
        field.set_cell_type(posPair.first, posPair.second, Field::Cell::Ship);
    }
}

void GameModel::_remove_ship(const Ship &ship) {
    _ships[_activePlayer].remove(ship);
    auto coordinates = ship.get_body();
    for (auto &posPair: coordinates) {
        auto &field = _fieldPairs[_activePlayer].first;
        field.set_cell_type(posPair.first, posPair.second, Field::Cell::Empty);
    }
}

bool GameModel::accept_choice() {
    if (!_shipInitializer.placementDone()) {
        return false;
    }
    _next_player();
    if (_activePlayer == PlayerNumber::Player1) {
        start_game();

        // TEST
        _fieldPairs[_inactivePlayer] = _fieldPairs[_activePlayer];
        _ships[_inactivePlayer] = _ships[_activePlayer];
    }
    else {
        _shipInitializer.start_initialization();
    }
    return true;
}

const std::string &GameModel::get_player_name(PlayerNumber player) {
    switch (player) {
    case PlayerNumber::Player1:
        return _playerName1;
    case PlayerNumber::Player2:
        return _playerName2;
    }
}
