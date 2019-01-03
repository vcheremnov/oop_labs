#include "gamemodel/gamemodel.h"
#include "gamecontroller/eventlistener.h"

namespace  {

using Option = MenuSelector::Option;

class MenuListener: public EventListener {
public:
    void key_event_occurred(const KeyEvent &keyPress) override;
private:
    void _select_action(Option);
    void _prev_option_variation(Option);
    void _next_option_variation(Option);
};

void MenuListener::key_event_occurred(const KeyEvent &keyPress) {
    auto &menuSelector = _get_model()->menu_selector();
    Option option = menuSelector.get_option();
    switch (keyPress.get_keycode()) {
    case KeyCode::KeyUP:
        menuSelector.prev_option();
        break;
    case KeyCode::KeyDOWN:
        menuSelector.next_option();
        break;
    case KeyCode::KeyLEFT:
        _prev_option_variation(option);
        break;
    case KeyCode::KeyRIGHT:
        _next_option_variation(option);
        break;
    case KeyCode::KeyENTER:
        _select_action(option);
        break;
    default:
        break;
    }
}

void MenuListener::_prev_option_variation(Option option) {
    auto &menuSelector = _get_model()->menu_selector();
    switch (option) {
    case Option::Difficulty:
        menuSelector.prev_difficulty();
        break;
    case Option::GameMode:
        menuSelector.prev_gamemode();
        break;
    case Option::AI_level_1:
        menuSelector.prev_AI_level_first();
        break;
    case Option::AI_level_2:
        menuSelector.prev_AI_level_second();
        break;
    default:
        break;
    }
}

void MenuListener::_next_option_variation(Option option) {
    auto &menuSelector = _get_model()->menu_selector();
    switch (option) {
    case Option::Difficulty:
        menuSelector.next_difficulty();
        break;
    case Option::GameMode:
        menuSelector.next_gamemode();
        break;
    case Option::AI_level_1:
        menuSelector.next_AI_level_first();
        break;
    case Option::AI_level_2:
        menuSelector.next_AI_level_second();
        break;
    default:
        break;
    }
}

void MenuListener::_select_action(Option option) {
    switch (option) {
    case Option::StartGame:
        _get_model()->start_ship_init();
        break;
    case Option::Controls:
        _get_model()->controls_menu();
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
    ListenerFactory::instance().register_listener<MenuListener>(GameState::MainMenu);
    return true;
}

bool regHandlers = register_listener();

}
