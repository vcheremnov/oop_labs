#include "headers/shipinitializer.h"
#include "gamemodel.h"
#include <stdexcept>
#include <algorithm>

ShipInitializer::ShipInitializer(GameModel *model): _model(model) {
    if (model == nullptr) {
        throw std::runtime_error("ShipInitializer::ShipInitializer(..): model is NULL");
    }
}

void ShipInitializer::start_initialization(ActivePlayer player) {
    const static auto shipPair1 = std::pair<Ship, bool>(Ship(Ship::Type::Ship1), true);
    const static auto shipPair2 = std::pair<Ship, bool>(Ship(Ship::Type::Ship2), true);
    const static auto shipPair3 = std::pair<Ship, bool>(Ship(Ship::Type::Ship3), true);
    const static auto shipPair4 = std::pair<Ship, bool>(Ship(Ship::Type::Ship4), true);
    _curPlayer = player;
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
}

void ShipInitializer::next_type() {
    if (_curType == Ship::Type::Total) {
        _curType = Ship::Type::Ship1;
    }
    else {
        _curType = static_cast<Ship::Type>(static_cast<int>(_curType) + 1);
    }
    _model->notify_views();
}

void ShipInitializer::prev_type() {
    if (_curType == Ship::Type::Ship1) {
        _curType = Ship::Type::Total;
    }
    else {
        _curType = static_cast<Ship::Type>(static_cast<int>(_curType) - 1);
    }
    _model->notify_views();
}

Ship &ShipInitializer::current_ship() {
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

void ShipInitializer::set_ship() {
    auto curShipIx = _curIndexes[_curType];
    auto &curShipPair = _ships[_curType][curShipIx];
    // check if ship hasn't been placed yet
    if (curShipPair.second) {
        // try to place the ship
        if (_model->_place_ship(curShipPair.first)) {
            // ship has been placed, set the flag to false
            curShipPair.second = false;
        }
    }
    _model->notify_views();
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
    }
    _model->notify_views();
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
