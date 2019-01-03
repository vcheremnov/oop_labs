#include <stdexcept>
#include "movemaker.h"
#include "gamemodel.h"

MoveMaker::MoveMaker(GameModel *model): _model(model) {
    if (model == nullptr) {
        throw std::runtime_error("TurnMaker::TurnMaker(..): model is NULL");
    }
    _lastShotCell = {-1, -1};
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

    auto &gameData = _model->game_data();
    auto &playerStats = _model->player_stats(gameData.get_active_player());
    auto &activeFieldPair = gameData._get_field_pair_for(gameData.get_active_player());
    auto &inactiveFieldPair = gameData._get_field_pair_for(gameData.get_inactive_player());
    auto cellType = inactiveFieldPair.first.get_cell_type(row, col);

    switch (cellType) {
    case Field::Cell::Ship: {
        playerStats._add_hit();
        // change fields
        activeFieldPair.second.set_cell_type(row, col, Field::Cell::ShipDestroyed);
        inactiveFieldPair.first.set_cell_type(row, col, Field::Cell::ShipDestroyed);
        // hit ship
        auto &ship = _find_ship(row, col);
        ship.hit_ship({row, col});
        if (ship.is_destroyed()) {
            _mark_ship_periphery(ship, activeFieldPair.second);
            _mark_ship_periphery(ship, inactiveFieldPair.first);
            gameData._delete_ship(ship, gameData.get_inactive_player());
            _lastMoveResult = MoveResult::Destroyed;
            playerStats._add_destroyed_ship();
        }
        else {
            _lastMoveResult = MoveResult::Hit;
        }
        _lastShotCell = {row, col};
        _model->notify_observers();
        break;
    }
    case Field::Cell::Empty:
        activeFieldPair.second.set_cell_type(row, col, Field::Cell::Miss);
        inactiveFieldPair.first.set_cell_type(row, col, Field::Cell::Miss);
        _lastMoveResult = MoveResult::Miss;
        playerStats._add_miss();

        _lastShotCell = {row, col};
        _model->notify_observers();
        break;
    default:
        _lastMoveResult = MoveResult::NotMade;
        break;
    }

}

bool MoveMaker::move_is_valid() const {
    auto &field = _model->game_data().get_active_field_pair().second;
    return field.get_cell_type(_curTarget.first, _curTarget.second) == Field::Cell::Unknown;
}

void MoveMaker::proceed() {
    if (_lastMoveResult == MoveResult::Miss) {
        _model->game_data()._next_player();
        _set_default_pos();
    }
    if (_lastMoveResult != MoveResult::NotMade) {
        _lastMoveResult = MoveResult::NotMade;
        _model->notify_observers();
    }
}

void MoveMaker::shift_pos(ShiftDirection shiftDirection) {
    switch (shiftDirection) {
    case ShiftDirection::Left:
        if (_curTarget.second > 0) {
            --_curTarget.second;
            _model->notify_observers();
        }
        break;
    case ShiftDirection::Up:
        if (_curTarget.first > 0) {
            --_curTarget.first;
            _model->notify_observers();
        }
        break;
    case ShiftDirection::Right:
        if (_curTarget.second < Field::WIDTH - 1) {
            ++_curTarget.second;
            _model->notify_observers();
        }
        break;
    case ShiftDirection::Down:
        if (_curTarget.first < Field::HEIGHT - 1) {
            ++_curTarget.first;
            _model->notify_observers();
        }
        break;
    }
}

Ship &MoveMaker::_find_ship(Field::pos row, Field::pos col) {
    auto inactivePlayer = _model->game_data().get_inactive_player();
    auto &shipSet = _model->game_data()._get_ship_list_for(inactivePlayer);
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
