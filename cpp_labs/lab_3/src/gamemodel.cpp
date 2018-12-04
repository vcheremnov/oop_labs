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

// view

void GameModel::attach_view(GameView *view) {
    if (view == nullptr) {
        throw std::runtime_error("VIEW IS NULL...");
    }
    _views.push_back(view);
}
void GameModel::detach_view(GameView *view) {
    _views.remove(view);
}

void GameModel::_next_player() {
    std::swap(_activePlayer, _inactivePlayer);
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

const std::string &GameModel::get_player_name(PlayerNumber player) {
    switch (player) {
    case PlayerNumber::Player1:
        return _playerName1;
    case PlayerNumber::Player2:
        return _playerName2;
    }
}
