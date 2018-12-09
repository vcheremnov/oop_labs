#pragma once

// game types
enum class GameType {
    ConsoleGame, GuiGame
};

// game states
enum class GameState {
    SplashScreen,
    MainMenu,
    ControlsMenu,
    ShipPlacement,
    Battle,
    Stats,
    Pause
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
