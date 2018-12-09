#include "gamemodel.h"
#include "eventlistener.h"

namespace  {

class ControlsScreenListener: public EventListener {
public:
    void key_event_occurred(const KeyEvent&) override;
};

void ControlsScreenListener::key_event_occurred(const KeyEvent &keyEvent)  {
    if (keyEvent.get_keycode() == KeyCode::KeyENTER) {
        _get_model()->main_menu();
    }
}

bool register_listener() {
    ListenerFactory::instance().register_listener<ControlsScreenListener>(GameState::ControlsMenu);
    return true;
}

bool regHandlers = register_listener();

}
