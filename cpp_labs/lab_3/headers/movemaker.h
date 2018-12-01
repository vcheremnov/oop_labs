#pragma once

#include "field.h"
#include "ship.h"

class GameModel;

class MoveMaker {
public:
    enum class ShiftDirection {
        Up, Right, Down, Left
    };
    enum class MoveResult {
        Hit, Destroyed, Miss, NotMade
    };
    using TargetCell = std::pair<Field::pos, Field::pos>;
    MoveMaker(GameModel *model);
    void make_shot();
    void make_shot(Field::pos row, Field::pos col);
    void shift_pos(ShiftDirection);
    MoveResult last_move_result() const
        { return _lastMoveResult; }
    const TargetCell &get_target() const
        { return _curTarget; }
    bool move_was_made() const
        { return _lastMoveResult != MoveResult::NotMade; }
    bool move_is_valid() const;
    void proceed();
//    //TEST
//    void destroy();
private:
    // private methods
    void _set_default_pos();
    Ship &_find_ship(Field::pos row, Field::pos col);
    void _mark_ship_periphery(const Ship&, Field&);
    // data
    GameModel *_model = nullptr;
    MoveResult _lastMoveResult = MoveResult::NotMade;
    TargetCell _curTarget;
};
