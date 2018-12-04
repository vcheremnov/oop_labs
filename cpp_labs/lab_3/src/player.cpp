#include <ncurses.h>
#include <stdexcept>
#include "player.h"
#include "event.h"
#include <random>
#include <algorithm>
#include <numeric>
#include <unistd.h>

namespace  {

KeyCode int_to_keycode(int code) {
    switch (code) {
    case KEY_UP:
        return KeyCode::KeyUP;
    case KEY_DOWN:
        return KeyCode::KeyDOWN;
    case KEY_LEFT:
        return KeyCode::KeyLEFT;
    case KEY_RIGHT:
        return KeyCode::KeyRIGHT;
    case '\t':
        return KeyCode::KeyTAB;
    case 'q': case 'Q':
        return KeyCode::KeyQ;
    case 'e': case 'E':
        return KeyCode::KeyE;
    case 'z': case 'Z':
        return KeyCode::KeyZ;
    case 'r': case 'R':
        return KeyCode::KeyR;
    case ' ':
        return KeyCode::KeySPACE;
    case '\n':
        return KeyCode::KeyENTER;
    default:
        return KeyCode::KeyUNKNOWN;
    }
}

} // anonymous namespace

// player

Player::Player(GameController *controller): _controller(controller) {
    if (controller == nullptr) {
        throw std::runtime_error("Player::Player() error: game controller is NULL");
    }
}

// human player

void HumanPlayer::wait_event() {
    auto eventListener = _get_controller()->get_event_listener();
    flushinp();
    int code = getch();
    if (code == KEY_MOUSE) {
        MouseEvent event;
        // filling mouse event...
        // ...
        eventListener->mouse_event_occurred(event);
    }
    else {
        KeyCode keyCode = int_to_keycode(code);
        KeyEvent event(keyCode);
        eventListener->key_event_occurred(event);
    }
}

// bot player

void BotPlayer::wait_event() {
    switch (_get_controller()->get_game_state()) {
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

// easy bot player
namespace {

class EasyBotPlayer: public BotPlayer {
public:
    using BotPlayer::BotPlayer;
protected:
    void _init_shooting_sequence() override;
    void _make_move() override;
private:
    std::size_t _curTargetIx;
    std::vector<Field::FieldCell> _cells;
};


void EasyBotPlayer::_make_move() {
    usleep(1000000);
    // wait for 1 second
    auto &moveMaker = _get_controller()->get_move_maker();
    do {
        auto &cell = _cells[_curTargetIx];
        moveMaker.make_shot(cell.first, cell.second);
        ++_curTargetIx;
    } while (!moveMaker.move_was_made() && _curTargetIx < _cells.size());
    moveMaker.proceed();
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

// register bot

bool register_bot_player() {
    BotFactory::instance().register_bot_player<EasyBotPlayer>(Difficulty::Easy);
    return true;
}

bool reg = register_bot_player();

}

