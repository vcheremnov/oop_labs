#include "gamemodel.h"
#include "eventlistener.h"

namespace  {

class ShipPlacementListener: public EventListener {
public:
    void key_event_occurred(const KeyEvent&);
};

void ShipPlacementListener::key_event_occurred(const KeyEvent &keyEvent) {
//    auto model = _get_model();
//    switch (keyEvent.get_keycode()) {
//    case KeyCode::KeyLEFT:
//        model->get_ship().shift(Ship::ShiftDirection::Left);
//        break;
//    case KeyCode::KeyUP:
//        model->get_ship().shift(Ship::ShiftDirection::Up);
//        break;
//    case KeyCode::KeyRIGHT:
//        model->get_ship().shift(Ship::ShiftDirection::Right);
//        break;
//    case KeyCode::KeyDOWN:
//        model->get_ship().shift(Ship::ShiftDirection::Down);
//        break;
//    case KeyCode::KeySPACE:
//        model->get_ship().rotate();
//        break;
//    case KeyCode::KeyF5:
//        model->next_ship();
//        break;
//    default:
//        break;
//    }
}

bool register_listener() {
    ListenerFactory::instance().register_listener<ShipPlacementListener>(GameState::ShipPlacement);
    return true;
}

bool regHandlers = register_listener();

}
