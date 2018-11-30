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

GameModel::GameModel(): _menuSelector(this), _shipInitializer(this) {
}

void GameModel::_next_player() {
    switch (_activePlayer) {
    case ActivePlayer::Player1:
        _activePlayer = ActivePlayer::Player2;
        break;
    case ActivePlayer::Player2:
        _activePlayer = ActivePlayer::Player1;
        break;
    }
}

void GameModel::_init_fields() {
    // clear fields
    _fieldPairs[ActivePlayer::Player1].first.clear();
    _fieldPairs[ActivePlayer::Player1].second.clear(Field::Cell::Unknown);
    _fieldPairs[ActivePlayer::Player2].first.clear();
    _fieldPairs[ActivePlayer::Player2].second.clear(Field::Cell::Unknown);
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
    if (_activePlayer == ActivePlayer::Player1) {
        start_game();
    }
    else {
        _shipInitializer.start_initialization(_activePlayer);
    }
    return true;
}

