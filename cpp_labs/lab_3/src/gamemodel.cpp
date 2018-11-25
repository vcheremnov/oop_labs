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

GameModel::GameModel(): _menuSelector(this) {

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
    _fields.front().first.clear();
    _fields.front().second.clear(Field::Cell::Unknown);
    _fields.back().first.clear();
    _fields.back().second.clear(Field::Cell::Unknown);
    // add available ships for placement
}

void GameModel::accept_choice() {

}
