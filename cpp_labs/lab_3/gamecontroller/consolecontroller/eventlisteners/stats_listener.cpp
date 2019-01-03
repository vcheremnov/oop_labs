#include "gamemodel/gamemodel.h"
#include "gamecontroller/eventlistener.h"

namespace  {

class StatsScreenListener: public EventListener {
public:
    void key_event_occurred(const KeyEvent&) override;
private:
};

void StatsScreenListener::key_event_occurred(const KeyEvent &keyEvent) {
    auto model = _get_model();
    switch (keyEvent.get_keycode()) {
    case KeyCode::KeyENTER:
        model->main_menu();
        break;
    default:
        break;
    }
}

bool register_listener() {
    ListenerFactory::instance().register_listener<StatsScreenListener>(GameState::Stats);
    return true;
}

bool regHandlers = register_listener();

}
