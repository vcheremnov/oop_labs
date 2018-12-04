#include "gamemodel.h"
#include "eventlistener.h"

namespace  {

using Option = MenuSelector::Option;

class MenuListener: public EventListener {
public:
    void key_event_occurred(const KeyEvent &keyPress) override;
private:
    void _select_action(Option);
};

void MenuListener::key_event_occurred(const KeyEvent &keyPress) {
    Option option = _get_model()->menu_selector().get_option();
    switch (keyPress.get_keycode()) {
    case KeyCode::KeyUP:
        _get_model()->menu_selector().prev_option();
        break;
    case KeyCode::KeyDOWN:
        _get_model()->menu_selector().next_option();
        break;
    case KeyCode::KeyLEFT:
        switch (option) {
        case Option::Difficulty:
            _get_model()->menu_selector().prev_difficulty();
            break;
        case Option::GameMode:
            _get_model()->menu_selector().prev_gamemode();
            break;
        case Option::AI_level_1:
            _get_model()->menu_selector().prev_AI_level_first();
            break;
        case Option::AI_level_2:
            _get_model()->menu_selector().prev_AI_level_second();
            break;
        default:
            break;
        }
        break;
    case KeyCode::KeyRIGHT:
        switch (option) {
        case Option::Difficulty:
            _get_model()->menu_selector().next_difficulty();
            break;
        case Option::GameMode:
            _get_model()->menu_selector().next_gamemode();
            break;
        case Option::AI_level_1:
            _get_model()->menu_selector().next_AI_level_first();
            break;
        case Option::AI_level_2:
            _get_model()->menu_selector().next_AI_level_second();
            break;
        default:
            break;
        }
        break;
    case KeyCode::KeyENTER:
        _select_action(option);
        break;
    default:
        break;
    }
}

void MenuListener::_select_action(Option option) {
    switch (option) {
    case Option::StartGame:
        _get_model()->start_ship_init();
//        _get_model()->start_game();
        break;
    case Option::Quit:
        _get_model()->quit();
        break;
    default:
        break;
    }
}

// register listener

bool register_listener() {
    ListenerFactory::instance().register_listener<MenuListener>(GameState::MenuSelect);
    return true;
}

bool regHandlers = register_listener();

}
