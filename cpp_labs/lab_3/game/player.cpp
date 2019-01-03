#include <stdexcept>
#include "player.h"

// player

Player::Player(GameController *controller): _controller(controller) {
    if (controller == nullptr) {
        throw std::runtime_error("Player::Player() error: game controller is NULL");
    }
}

