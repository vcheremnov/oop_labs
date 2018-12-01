#pragma once

#include "ship.h"
#include <vector>
#include "gamestate.h"

class GameModel;

class ShipInitializer {
public:
    enum ShiftDirection {
        Up, Down, Left, Right
    };
    const std::size_t SHIP1_NUM = 4;
    const std::size_t SHIP2_NUM = 3;
    const std::size_t SHIP3_NUM = 2;
    const std::size_t SHIP4_NUM = 1;
    // initialization
    ShipInitializer(GameModel*);
    void start_initialization();
    // interaction
    int ships_remained(Ship::Type);
    void next_type();
    void prev_type();
    void next_ship();
    void prev_ship();
    // ship actions
    void shift_ship(ShiftDirection);
    void rotate_ship();
    void set_ship();
    void reset_ship();
    // info
    const Ship &current_ship();
    bool ship_is_placed();
    bool placementDone();
    bool ship_is_overlapping();
private:
    Ship &_current_ship();
    void _switch_to_next_ship();
    GameModel *_model = nullptr;
    Ship::Type _curType;
    using ShipInitPair = std::pair<Ship, bool>;
    std::map<Ship::Type, std::vector<ShipInitPair>> _ships;
    std::map<Ship::Type, std::size_t> _curIndexes;
};
