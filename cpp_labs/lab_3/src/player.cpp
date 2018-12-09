#include <ncurses.h>
#include <stdexcept>
#include "player.h"
#include "event.h"
#include "gamestate.h"
#include <random>
#include <algorithm>
#include <numeric>
#include <unistd.h>

// player

Player::Player(GameController *controller): _controller(controller) {
    if (controller == nullptr) {
        throw std::runtime_error("Player::Player() error: game controller is NULL");
    }
}

// bot player

void BotPlayer::wait_event() {
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

namespace {

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
    case 'h': case 'H':
        return KeyCode::KeyH;
    case 'r': case 'R':
        return KeyCode::KeyR;
    case 'p': case 'P':
        return KeyCode::KeyP;
    case ' ':
        return KeyCode::KeySPACE;
    case KEY_BACKSPACE:
        return KeyCode::KeyBACKSPACE;
    case '\n':
        return KeyCode::KeyENTER;
    default:
        return KeyCode::KeyUNKNOWN;
    }
}

// console player

class ConsolePlayer: public HumanPlayer {
public:
    using HumanPlayer::HumanPlayer;
    void wait_event() override;
};

void ConsolePlayer::wait_event() {
    auto eventListener = _get_controller()->get_event_listener();
    flushinp();
    int code = getch();
    KeyCode keyCode = int_to_keycode(code);
    KeyEvent event(keyCode, code, has_key(code));
    eventListener->key_event_occurred(event);
}

// register human players

bool register_human_players() {
    HumanPlayerFactory::instance().register_human_player<ConsolePlayer>(GameType::ConsoleGame);
    return true;
}

bool regHumanPlayers = register_human_players();

} // anonymous namespace

// bot players

namespace {

class EasyBotPlayer: public BotPlayer {
public:
    using BotPlayer::BotPlayer;
protected:
    void _init_shooting_sequence() override;
    void _make_move() override;
private:
    std::size_t _curTargetIx = 0;
    std::vector<Field::FieldCell> _cells;
};


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

// register bots

bool register_bot_players() {
    BotPlayerFactory::instance().register_bot_player<EasyBotPlayer>(Difficulty::Easy, "EasyBot");
    return true;
}

bool regBotPlayers = register_bot_players();

} // anonymous namespace

