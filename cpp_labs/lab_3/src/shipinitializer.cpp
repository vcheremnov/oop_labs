#include "headers/shipinitializer.h"
#include "gamemodel.h"
#include <stdexcept>
#include <algorithm>

ShipInitializer::ShipInitializer(GameModel *model): _model(model) {
    if (model == nullptr) {
        throw std::runtime_error("ShipInitializer::ShipInitializer(..): model is NULL");
    }
}

void ShipInitializer::start_initialization() {
    const static auto shipPair1 = std::pair<Ship, bool>(Ship(Ship::Type::Ship1), true);
    const static auto shipPair2 = std::pair<Ship, bool>(Ship(Ship::Type::Ship2), true);
    const static auto shipPair3 = std::pair<Ship, bool>(Ship(Ship::Type::Ship3), true);
    const static auto shipPair4 = std::pair<Ship, bool>(Ship(Ship::Type::Ship4), true);
    // add ship pairs
    _ships[Ship::Type::Ship1] = std::vector<ShipInitPair>(SHIP1_NUM, shipPair1);
    _ships[Ship::Type::Ship2] = std::vector<ShipInitPair>(SHIP2_NUM, shipPair2);
    _ships[Ship::Type::Ship3] = std::vector<ShipInitPair>(SHIP3_NUM, shipPair3);
    _ships[Ship::Type::Ship4] = std::vector<ShipInitPair>(SHIP4_NUM, shipPair4);
    // init ship indexes
    _curIndexes[Ship::Type::Ship1] = 0;
    _curIndexes[Ship::Type::Ship2] = 0;
    _curIndexes[Ship::Type::Ship3] = 0;
    _curIndexes[Ship::Type::Ship4] = 0;
    // reset initial ship type
    _curType = Ship::Type::Ship1;
}

void ShipInitializer::next_type() {
    if (++_curType == Ship::Type::Total) {
        _curType = Ship::Type::Ship1;
    }
    _model->notify_views();
}

void ShipInitializer::prev_type() {
    if (--_curType == Ship::Type::Total) {
        --_curType;
    }
    _model->notify_views();
}

const Ship &ShipInitializer::current_ship() {
    return const_cast<const Ship&>(_current_ship());
}

Ship &ShipInitializer::_current_ship() {
    auto curShipIx = _curIndexes[_curType];
    return _ships[_curType][curShipIx].first;
}

void ShipInitializer::next_ship() {
    auto &curShipIx = _curIndexes[_curType];
    curShipIx = (curShipIx + 1) % _ships[_curType].size();
    _model->notify_views();
}

void ShipInitializer::prev_ship() {
    auto &curShipIx = _curIndexes[_curType];
    if (curShipIx == 0) {
        curShipIx = _ships[_curType].size();
    }
    --curShipIx;
    _model->notify_views();
}

void ShipInitializer::rotate_ship() {
    if (_current_ship()._rotate()) {
        _model->notify_views();
    }
}

void ShipInitializer::shift_ship(ShiftDirection direction) {
    auto &ship = _current_ship();
    auto bodyCopy = ship._body;
    auto peripheryCopy = ship._shipPeriphery;
    switch (direction) {
    case ShiftDirection::Left:
        ship._shift_left(bodyCopy, peripheryCopy);
        break;
    case ShiftDirection::Up:
        ship._shift_up(bodyCopy, peripheryCopy);
        break;
    case ShiftDirection::Right:
        ship._shift_right(bodyCopy, peripheryCopy);
        break;
    case ShiftDirection::Down:
        ship._shift_down(bodyCopy, peripheryCopy);
        break;
    }
    if (ship._check_validity_of(bodyCopy)) {
        ship._set_body(bodyCopy, peripheryCopy);
        _model->notify_views();
    }
}

void ShipInitializer::set_ship() {
    if (!ship_is_overlapping()) {
        auto curShipIx = _curIndexes[_curType];
        auto &curShipPair = _ships[_curType][curShipIx];
        // check if ship hasn't been placed yet
        if (curShipPair.second) {
            _model->_place_ship(curShipPair.first);
            curShipPair.second = false;
            next_ship();
        }
        _model->notify_views();
    }
}

void ShipInitializer::reset_ship() {
    auto curShipIx = _curIndexes[_curType];
    auto &curShipPair = _ships[_curType][curShipIx];
    // check if ship has been placed
    if (!curShipPair.second) {
        // remove ship
        _model->_remove_ship(curShipPair.first);
        // restore flag
        curShipPair.second = true;
        _model->notify_views();
    }
}

bool ShipInitializer::placementDone() {
    for (auto &shipPairSet: _ships) {
        for (auto &shipPair: shipPairSet.second) {
            if (shipPair.second) {
                return false;
            }
        }
    }
    return true;
}

bool ShipInitializer::ship_is_overlapping() {
    auto &field = _model->get_field_pair().first;
    auto &curShip = _ships[_curType][_curIndexes[_curType]].first;
    for (auto &posPair: curShip.get_body()) {
        if (field.get_cell_type(posPair.first, posPair.second) == Field::Cell::Ship) {
            return true;
        }
    }
    for (auto &posPair: curShip.get_periphery()) {
        if (Field::is_valid_pos(posPair.first, posPair.second)) {
            if (field.get_cell_type(posPair.first, posPair.second) == Field::Cell::Ship) {
                return true;
            }
        }
    }
    return false;
}

int ShipInitializer::ships_remained(Ship::Type shipType) {
    int cnt = 0;
    auto &shipPairSet = _ships[shipType];
    for (auto &shipPair: shipPairSet) {
        if (shipPair.second) {
            ++cnt;
        }
    }
    return cnt;
}

bool ShipInitializer::ship_is_placed() {
    auto curShipIx = _curIndexes[_curType];
    return !_ships[_curType][curShipIx].second;
}

void ShipInitializer::_switch_to_next_ship() {
    auto shipType = _curType;
    auto shipIx = _curIndexes[_curType];

}
