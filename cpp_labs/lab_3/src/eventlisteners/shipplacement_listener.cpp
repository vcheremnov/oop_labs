#include "gamemodel.h"
#include "eventlistener.h"

namespace  {

class ShipPlacementListener: public EventListener {
public:
    void key_event_occurred(const KeyEvent&);
};

void ShipPlacementListener::key_event_occurred(const KeyEvent &keyEvent) {
    auto model = _get_model();
    auto &shipInitializer = model->ship_initializer();
    if (!shipInitializer.ship_is_placed()) {
        switch (keyEvent.get_keycode()) {
        case KeyCode::KeyLEFT:
            shipInitializer.shift_ship(Ship::ShiftDirection::Left);
            break;
        case KeyCode::KeyUP:
            shipInitializer.shift_ship(Ship::ShiftDirection::Up);
            break;
        case KeyCode::KeyRIGHT:
            shipInitializer.shift_ship(Ship::ShiftDirection::Right);
            break;
        case KeyCode::KeyDOWN:
            shipInitializer.shift_ship(Ship::ShiftDirection::Down);
            break;
        case KeyCode::KeySPACE:
            shipInitializer.rotate_ship();
            break;
        case KeyCode::KeyENTER:
            shipInitializer.set_ship();
            return;
        default:
            break;
        }
    }
    switch (keyEvent.get_keycode()) {
    case KeyCode::KeyENTER:
        if (shipInitializer.placement_done()) {
            shipInitializer.accept_choice();
        }
        break;
    case KeyCode::KeyE:
        shipInitializer.next_ship();
        break;
    case KeyCode::KeyQ:
        shipInitializer.prev_ship();
        break;
    case KeyCode::KeyR:
        shipInitializer.random_initialization();
        break;
    case KeyCode::KeyTAB:
        shipInitializer.next_type();
        break;
    case KeyCode::KeyZ:
        shipInitializer.reset_ship();
        break;
    default:
        break;
    }
}

bool register_listener() {
    ListenerFactory::instance().register_listener<ShipPlacementListener>(GameState::ShipPlacement);
    return true;
}

bool regHandlers = register_listener();

}
