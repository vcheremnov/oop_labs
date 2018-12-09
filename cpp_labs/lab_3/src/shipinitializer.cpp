#include "headers/shipinitializer.h"
#include "gamemodel.h"
#include <stdexcept>
#include <algorithm>
#include <random>

ShipInitializer::ShipInitializer(GameModel *model): _model(model) {
    if (model == nullptr) {
        throw std::runtime_error("ShipInitializer::ShipInitializer(..): model is NULL");
    }
    _init_all_ships();
}

void ShipInitializer::random_initialization() {
    // reset all placed ships
    auto &gameData = _model->game_data();
    for (auto &[shipType, shipPairSet]: _ships) {
        for (auto &shipPair: shipPairSet) {
            if (!shipPair.second) {
                _remove_ship_from_field(shipPair.first);
                gameData._delete_ship(shipPair.first, gameData.get_active_player());
            }
        }
        shipPairSet.clear();
    }
    _reset_ship_counters();
    _reset_current_indexes();
    // get random vectors of indexes
    std::map<Ship::Type, std::vector<std::size_t>> indexes;
    _get_random_ship_indexes(indexes);
    // place ships
    for (auto shipType = Ship::Type::Ship4; shipType != Ship::Type::Total; --shipType) {
        for (auto ix = 0u; ix < indexes[shipType].size() && _shipCounters[shipType] > 0; ++ix) {
            _set_ship(_allShips[shipType][indexes[shipType][ix]]);
        }
    }
    _model->notify_views();
}

void ShipInitializer::_reset_ship_counters() {
    _shipCounters[Ship::Type::Ship1] = SHIP1_NUM;
    _shipCounters[Ship::Type::Ship2] = SHIP2_NUM;
    _shipCounters[Ship::Type::Ship3] = SHIP3_NUM;
    _shipCounters[Ship::Type::Ship4] = SHIP4_NUM;
}

void ShipInitializer::_reset_current_indexes() {
    _curIndexes[Ship::Type::Ship1] = 0;
    _curIndexes[Ship::Type::Ship2] = 0;
    _curIndexes[Ship::Type::Ship3] = 0;
    _curIndexes[Ship::Type::Ship4] = 0;
}

void ShipInitializer::_init_all_ships() {
    // get all posible positions of ships
    for (auto shipType = Ship::Type::Ship1; shipType != Ship::Type::Total; ++shipType) {
        // add horizontal positions
        _add_all_ship_variations(shipType, Ship::Orientation::Horizontal);
        // add vertical positions
        if (shipType != Ship::Type::Ship1) {
            _add_all_ship_variations(shipType, Ship::Orientation::Vertical);
        }
    }
}

void ShipInitializer::_add_all_ship_variations(Ship::Type shipType, Ship::Orientation orientation) {
    Ship ship = Ship(shipType, orientation);
    for (auto row = 0; ; ++row) {
        do {
            _allShips[shipType].push_back(ship);
        } while (ship.shift_ship(row % 2 == 0 ? Ship::ShiftDirection::Right : Ship::ShiftDirection::Left));
        if (!ship.shift_ship(Ship::ShiftDirection::Down)) {
            break;
        }
    }
}

void ShipInitializer::_get_random_ship_indexes(std::map<Ship::Type, std::vector<std::size_t>> &indexes) {
    for (auto &mapItem: _allShips) {
        auto &indexVec = indexes[mapItem.first];
        indexVec.resize(mapItem.second.size());
        std::iota(indexVec.begin(), indexVec.end(), 0);
        std::shuffle(indexVec.begin(), indexVec.end(), std::mt19937{std::random_device{}()});
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
    _reset_current_indexes();
    // init ship counters
    _reset_ship_counters();
    // reset initial ship type
    _curType = Ship::Type::Ship1;
}


bool ShipInitializer::accept_choice() {
    if (!placement_done()) {
        return false;
    }
    _model->game_data()._next_player();
    if (_model->game_data().get_active_player() == PlayerNumber::Player1) {
        _model->start_game();
    }
    else {
        start_initialization();
    }
    _model->notify_views();
    return true;
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

ShipInitializer::ShipInitPair& ShipInitializer::_current_ship_pair() {
    auto curShipIx = _curIndexes[_curType];
    return _ships[_curType][curShipIx];
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
    if (_current_ship().rotate_ship()) {
        _model->notify_views();
    }
}

void ShipInitializer::shift_ship(Ship::ShiftDirection direction) {
    if (_current_ship().shift_ship(direction)) {
        _model->notify_views();
    }
}

bool ShipInitializer::_set_ship(const Ship &ship) {
    if (_shipCounters[ship.get_type()] > 0 && !_ship_is_overlapping(ship)) {
        auto &gameData = _model->game_data();
        gameData._add_ship(ship, gameData.get_active_player());
        _place_ship_on_field(ship);
        _ships[ship.get_type()].push_back(ShipInitPair(ship, false));
        --_shipCounters[ship.get_type()];
        return true;
    }
    return false;
}

bool ShipInitializer::set_ship() {
    if (_shipCounters[_curType] > 0 && !ship_is_placed() && !ship_is_overlapping()) {
        auto &gameData = _model->game_data();
        gameData._add_ship(current_ship(), gameData.get_active_player());
        _place_ship_on_field(current_ship());
        _model->notify_views();
        --_shipCounters[_curType];
        _current_ship_pair().second = false;
        next_ship();
        return true;
    }
    return false;
}

void ShipInitializer::reset_ship() {
    // check if ship has been placed
    if (ship_is_placed()) {
        // remove ship
        auto &ship = current_ship();
        auto &gameData = _model->game_data();
        gameData._delete_ship(ship, gameData.get_active_player());
        _remove_ship_from_field(ship);
        // restore flag
        _current_ship_pair().second = true;
        ++_shipCounters[_curType];
        _model->notify_views();
    }
}

void ShipInitializer::_place_ship_on_field(const Ship &ship) {
    auto &gameData = _model->game_data();
    auto &coordinates = ship.get_body();
    auto &field = gameData._get_field_pair_for(gameData.get_active_player()).first;
    for (auto &[row, col]: coordinates) {
        field.set_cell_type(row, col, Field::Cell::Ship);
    }
}

void ShipInitializer::_remove_ship_from_field(const Ship &ship) {
    auto &gameData = _model->game_data();
    auto &coordinates = ship.get_body();
    auto &field = gameData._get_field_pair_for(gameData.get_active_player()).first;
    for (auto &[row, col]: coordinates) {
        field.set_cell_type(row, col, Field::Cell::Empty);
    }
}

bool ShipInitializer::placement_done() {
    for (auto &shipCounterPair: _shipCounters) {
        if (shipCounterPair.second > 0) {
            return false;
        }
    }
    return true;
}

bool ShipInitializer::ship_is_overlapping() {
    return _ship_is_overlapping(_current_ship());
}

bool ShipInitializer::_ship_is_overlapping(const Ship &ship) {
    auto &field = _model->game_data().get_active_field_pair().first;
    for (auto &cell: ship.get_body()) {
        if (field.get_cell_type(cell.first, cell.second) == Field::Cell::Ship) {
            return true;
        }
    }
    for (auto &cell: ship.get_periphery()) {
        if (Field::is_valid_pos(cell.first, cell.second)) {
            if (field.get_cell_type(cell.first, cell.second) == Field::Cell::Ship) {
                return true;
            }
        }
    }
    return false;
}

std::size_t ShipInitializer::ships_remained(Ship::Type shipType) {
    return _shipCounters[shipType];
}

bool ShipInitializer::ship_is_placed() {
    auto curShipIx = _curIndexes[_curType];
    return !_ships[_curType][curShipIx].second;
}
