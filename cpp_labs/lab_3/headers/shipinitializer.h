#pragma once

#include "ship.h"
#include <vector>
#include "gamestate.h"

class GameModel;

class ShipInitializer {
public:
    const std::size_t SHIP1_NUM = 4;
    const std::size_t SHIP2_NUM = 3;
    const std::size_t SHIP3_NUM = 2;
    const std::size_t SHIP4_NUM = 1;
    // initialization
    ShipInitializer(GameModel*);
    void start_initialization();
    void random_initialization();
    // interaction
    void next_type();
    void prev_type();
    void next_ship();
    void prev_ship();
    // ship actions
    void shift_ship(Ship::ShiftDirection);
    void rotate_ship();
    bool set_ship();
    void reset_ship();
    // info
    const Ship &current_ship();
    bool ship_is_placed();
    bool placement_done();
    bool accept_choice();
    bool ship_is_overlapping();
    std::size_t ships_remained(Ship::Type);
private:
    using ShipInitPair = std::pair<Ship, bool>;
    // private methods
    void _reset_ship_counters();
    void _reset_current_indexes();
    void _switch_to_next_ship();
    bool _ship_is_overlapping(const Ship&);
    bool _set_ship(const Ship&);
    Ship &_current_ship();
    ShipInitPair &_current_ship_pair();
    // data
    GameModel *_model = nullptr;
    Ship::Type _curType;
    std::map<Ship::Type, size_t> _shipCounters;
    std::map<Ship::Type, std::size_t> _curIndexes;
    std::map<Ship::Type, std::vector<ShipInitPair>> _ships;
    // random placement
    std::map<Ship::Type, std::vector<Ship>> _allShips;
    void _init_all_ships();
    void _get_random_ship_indexes(std::map<Ship::Type, std::vector<std::size_t>>&);
    void _add_all_ship_variations(Ship::Type, Ship::Orientation);
};
