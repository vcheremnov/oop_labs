#include <random>
#include <algorithm>
#include <numeric>
#include <unistd.h>
#include "game/player.h"
#include "game/gamestate.h"
#include "gamecontroller/event.h"
#include "gamecontroller/gamecontroller.h"

namespace {

class EasyBotPlayer: public BotPlayer {
public:
    using BotPlayer::BotPlayer;
    void wait_event() override;
protected:
    void _init_shooting_sequence();
    void _make_move() override;
private:
    std::size_t _curTargetIx = 0;
    std::vector<Field::FieldCell> _cells;
};

void EasyBotPlayer::wait_event() {
    switch (_get_controller()->get_game_data().get_game_state()) {
    case GameState::ShipPlacement:
        _init_shooting_sequence();
        _get_controller()->get_ship_initializer().random_initialization();
        _get_controller()->get_ship_initializer().accept_choice();
        break;
    case GameState::Battle:
        _make_move();
        break;
    default:
        break;
    }
}

void EasyBotPlayer::_init_shooting_sequence() {
    // make vector of all field cells
    for (auto row = 0; row < Field::HEIGHT; ++row) {
        for (auto col = 0; col < Field::WIDTH; ++col) {
            _cells.emplace_back(row, col);
        }
    }
    // randomly shuffle the vector
    std::shuffle(_cells.begin(), _cells.end(), std::mt19937{std::random_device{}()});
    // set current target to the beginning
    _curTargetIx = 0;
}

void EasyBotPlayer::_make_move() {
    auto &moveMaker = _get_controller()->get_move_maker();
    // wait for 1 second
    usleep(1000000);
    // shoot
    if (_curTargetIx < _cells.size()) do {
        auto &cell = _cells[_curTargetIx];
        moveMaker.make_shot(cell.first, cell.second);
        ++_curTargetIx;
    } while (!moveMaker.move_was_made() && _curTargetIx < _cells.size());
}

// register bots

bool register_bot_players() {
    BotPlayerFactory::instance().register_bot_player<EasyBotPlayer>("Easy", "EasyBot");
    return true;
}

bool regBotPlayers = register_bot_players();

} // anonymous namespace
