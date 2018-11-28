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
    void start_initialization(ActivePlayer);
    // interaction
    void next_type();
    void prev_type();
    void next_ship();
    void prev_ship();
    Ship &current_ship();
    void set_ship();
    void reset_ship();
    bool placementDone();
private:
    GameModel *_model = nullptr;
    ActivePlayer _curPlayer;
    Ship::Type _curType;
    using ShipInitPair = std::pair<Ship, bool>;
    std::map<Ship::Type, std::vector<ShipInitPair>> _ships;
    std::map<Ship::Type, std::size_t> _curIndexes;
};
