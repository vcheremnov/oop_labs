#include "gamecontroller.h"

ConsoleController::ConsoleController(GameModel *model) {
    if (model == nullptr) {
        throw std::runtime_error("ConsoleController::ConsoleController():"
                                 "model is NULL");
    }
    // init listeners
    ListenerFactory::instance().get_all_listeners(_listeners);
    // bind listeners to the model
    for (auto &mapItem: _listeners) {
        mapItem.second->bind_to_model(model);
    }
}

void ConsoleController::switch_listener(GameState gameState) {
    if (_get_human_player() == nullptr) {
        throw std::runtime_error("ConsoleController::switch_listener():"
                                 " human player is NULL");
    }
    if (_get_bot_player() == nullptr) {
        throw std::runtime_error("ConsoleController::switch_listener():"
                                 " bot player is NULL");
    }
    _get_human_player()->set_listener(_listeners[gameState].get());
    _get_bot_player()->set_listener(_listeners[gameState].get());
}
