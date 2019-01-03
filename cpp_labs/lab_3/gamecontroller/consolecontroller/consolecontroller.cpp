#include "consolecontroller.h"
#include "gamemodel/gamedata.h"

ConsoleController::ConsoleController(GameModel *model): GameController (model) {
    // init listeners
    ListenerFactory::instance().get_all_listeners(_listeners);
    // bind listeners to the model
    for (auto &mapItem: _listeners) {
        mapItem.second->bind_to_model(model);
    }
}

EventListener* ConsoleController::get_event_listener() {
    return _listeners[get_game_data().get_game_state()].get();
}

namespace {

bool register_controller() {
    ControllerFactory::instance().register_controller<ConsoleController>(GameType::ConsoleGame);
    return true;
}

bool regController = register_controller();

} // anonymous namespace
