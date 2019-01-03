#include <random>
#include <algorithm>
#include <numeric>
#include <unistd.h>
#include "game/player.h"
#include "game/gamestate.h"
#include "gamecontroller/event.h"
#include "gamecontroller/gamecontroller.h"

namespace {

class NormalBotPlayer: public BotPlayer {
public:
    enum Direction {
        Up, Right, Down, Left
    };
    using BotPlayer::BotPlayer;
    void wait_event() override;
protected:
    void _init_shooting_sequence();
    void _make_move() override;
private:
    // when ship is found
    bool _foundShip = false;
    bool _foundDirection = false;
    Field::FieldCell _firstHit;
    Field::FieldCell _baseCell;
    Direction _direction;
    std::vector<Direction> _randomDirections = {
        Direction::Up,
        Direction::Down,
        Direction::Left,
        Direction::Right
    };
    std::size_t _directionIndex = 0;
    // random shooting
    std::size_t _curTargetIx = 0;
    std::vector<Field::FieldCell> _cells;
    // private methods
    Field::FieldCell _get_next_cell();
    void _invert_direction();
    void _randomize_directions();
    void _handle_move_result(MoveMaker::MoveResult, const Field::FieldCell&);
};

void NormalBotPlayer::wait_event() {
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

void NormalBotPlayer::_make_move() {
    auto &moveMaker = _get_controller()->get_move_maker();
    // wait for 1 second
    usleep(1000000);
    // shoot
    if (_foundShip) do {
        if (!_foundDirection) {
            _direction = _randomDirections.at(_directionIndex++);
        }
        Field::FieldCell cell = _get_next_cell();
        moveMaker.make_shot(cell.first, cell.second);
        auto moveResult = moveMaker.last_move_result();
        _handle_move_result(moveResult, cell);
    } while (!moveMaker.move_was_made());
    else if (_curTargetIx < _cells.size()) do {
        auto &cell = _cells[_curTargetIx++];
        moveMaker.make_shot(cell.first, cell.second);
        auto moveResult = moveMaker.last_move_result();
        _handle_move_result(moveResult, cell);
    } while (!moveMaker.move_was_made() && _curTargetIx < _cells.size());
}

Field::FieldCell NormalBotPlayer::_get_next_cell() {
    switch (_direction) {
    case Direction::Up:
        return {_baseCell.first - 1, _baseCell.second};
    case Direction::Down:
        return {_baseCell.first + 1, _baseCell.second};
    case Direction::Left:
        return {_baseCell.first, _baseCell.second - 1};
    case Direction::Right:
        return {_baseCell.first, _baseCell.second + 1};
    }
}

void NormalBotPlayer::_invert_direction() {
    switch (_direction) {
    case Direction::Up:
        _direction = Direction::Down;
        break;
    case Direction::Down:
        _direction = Direction::Up;
        break;
    case Direction::Left:
        _direction = Direction::Right;
        break;
    case Direction::Right:
        _direction = Direction::Left;
        break;
    }
}

void NormalBotPlayer::_handle_move_result(MoveMaker::MoveResult moveResult, const Field::FieldCell &cell) {
    switch (moveResult) {
    case MoveMaker::MoveResult::Hit:
        if (_foundShip) {
            _foundDirection = true;
        }
        else {
            _foundShip = true;
            _firstHit = cell;
            _randomize_directions();
        }
        _baseCell = cell;
        break;
    case MoveMaker::MoveResult::Destroyed:
        _foundShip = false;
        _foundDirection = false;
        break;
    case MoveMaker::MoveResult::Miss: case MoveMaker::MoveResult::NotMade:
        if (_foundDirection) {
            _invert_direction();
            _baseCell = _firstHit;
        }
        break;
    default:
        break;
    }
}

void NormalBotPlayer::_randomize_directions() {
    std::shuffle(_randomDirections.begin(), _randomDirections.end(), std::mt19937{std::random_device{}()});
    _directionIndex = 0;
}

void NormalBotPlayer::_init_shooting_sequence() {
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

// register bots

bool register_bot_players() {
    BotPlayerFactory::instance().register_bot_player<NormalBotPlayer>("Normal", "NormalBot");
    return true;
}

bool regBotPlayers = register_bot_players();

} // anonymous namespace
