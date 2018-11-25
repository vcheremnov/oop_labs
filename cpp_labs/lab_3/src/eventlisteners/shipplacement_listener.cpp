#include "gamemodel.h"
#include "eventlistener.h"

namespace  {

class ShipPlacementListener: public EventListener {
public:
    void key_event_occurred(const KeyEvent&) {
        _get_model()->go_to_menu();
    }
};


bool register_listener() {
    ListenerFactory::instance().register_listener<ShipPlacementListener>(GameState::ShipPlacement);
    return true;
}

bool regHandlers = register_listener();

}
