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
            shipInitializer.shift_ship(ShipInitializer::ShiftDirection::Left);
            break;
        case KeyCode::KeyUP:
            shipInitializer.shift_ship(ShipInitializer::ShiftDirection::Up);
            break;
        case KeyCode::KeyRIGHT:
            shipInitializer.shift_ship(ShipInitializer::ShiftDirection::Right);
            break;
        case KeyCode::KeyDOWN:
            shipInitializer.shift_ship(ShipInitializer::ShiftDirection::Down);
            break;
        case KeyCode::KeySPACE:
            shipInitializer.rotate_ship();
            break;
        case KeyCode::KeyENTER:
            if (shipInitializer.placementDone()) {
                model->accept_choice();
            }
            else {
                shipInitializer.set_ship();
            }
            break;
        default:
            break;
        }
    }
    switch (keyEvent.get_keycode()) {
    case KeyCode::KeyE:
        shipInitializer.next_ship();
        break;
    case KeyCode::KeyQ:
        shipInitializer.prev_ship();
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
