#include "gamemodel/gamemodel.h"
#include "gamecontroller/eventlistener.h"

namespace  {

using Option = PauseMenuSelector::Option;

class PauseScreenListener: public EventListener {
public:
    void key_event_occurred(const KeyEvent&) override;
private:
    void _select_action(Option);
};

void PauseScreenListener::key_event_occurred(const KeyEvent &keyEvent) {
    auto &pauseMenuSelector = _get_model()->pause_menu_selector();
    Option option = _get_model()->pause_menu_selector().get_option();
    switch (keyEvent.get_keycode()) {
    case KeyCode::KeyUP:
        pauseMenuSelector.prev_option();
        break;
    case KeyCode::KeyDOWN:
        pauseMenuSelector.next_option();
        break;
    case KeyCode::KeyENTER:
        _select_action(option);
        break;
    default:
        break;
    }
}

void PauseScreenListener::_select_action(Option opt) {
    switch (opt) {
    case Option::Continue:
        _get_model()->unpause();
        break;
    case Option::GotoMainMenu:
        _get_model()->finish_game();
        _get_model()->main_menu();
        break;
    case Option::Quit:
        _get_model()->quit();
        break;
    default:
        break;
    }
}

bool register_listener() {
    ListenerFactory::instance().register_listener<PauseScreenListener>(GameState::Pause);
    return true;
}

bool regHandlers = register_listener();

}
