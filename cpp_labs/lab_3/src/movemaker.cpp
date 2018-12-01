#include <stdexcept>
#include "headers/movemaker.h"
#include "gamemodel.h"

MoveMaker::MoveMaker(GameModel *model): _model(model) {
    if (model == nullptr) {
        throw std::runtime_error("TurnMaker::TurnMaker(..): model is NULL");
    }
    _set_default_pos();
}

void MoveMaker::make_shot() {
    make_shot(_curTarget.first, _curTarget.second);
}

void MoveMaker::make_shot(Field::pos row, Field::pos col) {
    if (!Field::is_valid_pos(row, col)) {
        _lastMoveResult = MoveResult::NotMade;
        return;
    }

    auto &fieldPairs = _model->_fieldPairs;
    auto activePlayer = _model->get_active_player(),
         inactivePlayer = _model->get_inactive_player();
    auto cellType = fieldPairs[inactivePlayer].first.get_cell_type(row, col);

    switch (cellType) {
    case Field::Cell::Ship: {
        fieldPairs[activePlayer].second.set_cell_type(row, col, Field::Cell::ShipDestroyed);
        fieldPairs[inactivePlayer].first.set_cell_type(row, col, Field::Cell::ShipDestroyed);
        auto &ship = _find_ship(row, col);
        ship.hit_ship({row, col});
        if (ship.is_destroyed()) {
            _mark_ship_periphery(ship, fieldPairs[activePlayer].second);
            _mark_ship_periphery(ship, fieldPairs[inactivePlayer].first);
            _lastMoveResult = MoveResult::Destroyed;
        }
        else {
            _lastMoveResult = MoveResult::Hit;
        }
        _model->notify_views();
        break;
    }
    case Field::Cell::Empty:
        fieldPairs[activePlayer].second.set_cell_type(row, col, Field::Cell::Miss);
        fieldPairs[inactivePlayer].first.set_cell_type(row, col, Field::Cell::Miss);
        _lastMoveResult = MoveResult::Miss;
        _model->notify_views();
        break;
    default:
        _lastMoveResult = MoveResult::NotMade;
        break;
    }

}

bool MoveMaker::move_is_valid() const {
    auto &field = _model->get_field_pair().second;
    return field.get_cell_type(_curTarget.first, _curTarget.second) == Field::Cell::Unknown;
}

void MoveMaker::proceed() {
    if (_lastMoveResult == MoveResult::Miss) {
        _model->_next_player();
        _set_default_pos();
    }
    if (_lastMoveResult != MoveResult::NotMade) {
        _lastMoveResult = MoveResult::NotMade;
        _model->notify_views();
    }
}

void MoveMaker::shift_pos(ShiftDirection shiftDirection) {
    switch (shiftDirection) {
    case ShiftDirection::Left:
        if (_curTarget.second > 0) {
            --_curTarget.second;
            _model->notify_views();
        }
        break;
    case ShiftDirection::Up:
        if (_curTarget.first > 0) {
            --_curTarget.first;
            _model->notify_views();
        }
        break;
    case ShiftDirection::Right:
        if (_curTarget.second < Field::WIDTH - 1) {
            ++_curTarget.second;
            _model->notify_views();
        }
        break;
    case ShiftDirection::Down:
        if (_curTarget.first < Field::HEIGHT - 1) {
            ++_curTarget.first;
            _model->notify_views();
        }
        break;
    }
}

Ship &MoveMaker::_find_ship(Field::pos row, Field::pos col) {
    auto &shipSet = _model->_ships[_model->get_inactive_player()];
    for (auto &ship: shipSet) {
        for (auto &cell: ship.get_body()) {
            if (cell.first == row && cell.second == col) {
                return ship;
            }
        }
    }
}

void MoveMaker::_mark_ship_periphery(const Ship &ship, Field &field) {
    auto &shipPeriphery = ship.get_periphery();
    for (auto &cell: shipPeriphery) {
        if (Field::is_valid_pos(cell.first, cell.second)) {
            field.set_cell_type(cell.first, cell.second, Field::Cell::Miss);
        }
    }
}

void MoveMaker::_set_default_pos() {
    _curTarget = {Field::HEIGHT / 2, Field::WIDTH / 2};
}

//#include <cassert>

//void MoveMaker::destroy() {
//    for (auto row = 0; row < Field::HEIGHT; ++row) {
//        for (auto col = 0; col < Field::WIDTH; ++col) {
//            make_shot(row, col);
//        }
//    }
//    auto &shipSet = _model->_ships[_model->_inactivePlayer];
//    for (auto &ship: shipSet) {
//        assert(("Ship must be destroyed", ship.is_destroyed()));
//    }
//}
