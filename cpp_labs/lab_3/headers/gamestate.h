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

// player's number
enum class PlayerNumber {
    Player1, Player2
};

enum class GameMode {
    Player_vs_Bot, Bot_vs_Bot, Player_vs_Player, Total
};

enum class Difficulty {
    Easy, Normal, Hard, Total
};
