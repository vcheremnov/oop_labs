#include "gamemodel.h"
#include "eventlistener.h"

namespace  {

class BattleScreenListener: public EventListener {
public:
    void key_event_occurred(const KeyEvent&) override;
private:
};

void BattleScreenListener::key_event_occurred(const KeyEvent &keyEvent) {
    auto &moveMaker = _get_model()->move_maker();
    if (moveMaker.move_was_made()) {
        if (keyEvent.get_keycode() == KeyCode::KeyENTER) {
            moveMaker.proceed();
        }
    }
    else switch (keyEvent.get_keycode()) {
    case KeyCode::KeyUP:
        moveMaker.shift_pos(MoveMaker::ShiftDirection::Up);
        break;
    case KeyCode::KeyRIGHT:
        moveMaker.shift_pos(MoveMaker::ShiftDirection::Right);
        break;
    case KeyCode::KeyDOWN:
        moveMaker.shift_pos(MoveMaker::ShiftDirection::Down);
        break;
    case KeyCode::KeyLEFT:
        moveMaker.shift_pos(MoveMaker::ShiftDirection::Left);
        break;
    case KeyCode::KeyENTER:
        moveMaker.make_shot();
        break;
    default:
        break;
    }
}

bool register_listener() {
    ListenerFactory::instance().register_listener<BattleScreenListener>(GameState::Battle);
    return true;
}

bool regHandlers = register_listener();

}
