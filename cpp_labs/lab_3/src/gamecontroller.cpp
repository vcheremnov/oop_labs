#include "gamecontroller.h"
#include "gamemodel.h"

// game controller

GameController::GameController(GameModel *model): _model(model) {
    if (model == nullptr) {
        throw std::runtime_error("GameController::GameController() error: model is NULL");
    }
}

const GameData &GameController::get_game_data() {
    return _get_model()->game_data();
}

ShipInitializer& GameController::get_ship_initializer() {
    return _get_model()->ship_initializer();
}

MoveMaker& GameController::get_move_maker() {
    return _get_model()->move_maker();
}

// console controller

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
