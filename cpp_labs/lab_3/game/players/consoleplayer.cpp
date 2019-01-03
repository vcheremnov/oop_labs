#include <ncurses.h>
#include "game/player.h"
#include "game/gamestate.h"
#include "gamecontroller/event.h"
#include "gamecontroller/gamecontroller.h"

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
