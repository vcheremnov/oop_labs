#pragma once

// game states
enum class GameState {
    SplashScreen,
    MenuSelect,
    ShipPlacement,
    Battle,
    Stats,
    QuitScreen
};

// active player
enum class ActivePlayer {
    Player1, Player2
};
