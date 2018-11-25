#pragma once

#include "gamemodel.h"
#include "gameview.h"
#include "gamecontroller.h"
#include "player.h"
#include <memory>

// game types
enum class GameType {
    ConsoleGame, GuiGame
};

class Game {
public:
    // constructor
    Game(GameType gameType);
    // public methods
    void run();
private:
    // private methods
    void _switch_active_player();
    // private data
    std::unique_ptr<GameModel> _model;
    std::unique_ptr<GameView> _view;
    std::unique_ptr<GameController> _controller;
    std::shared_ptr<HumanPlayer> _humanPlayer;
    std::shared_ptr<BotPlayer> _botPlayer;
    std::shared_ptr<Player> _activePlayer;
};

