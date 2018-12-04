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
    void _create_players();
    // private data
    std::unique_ptr<GameModel> _model;
    std::unique_ptr<GameView> _view;
    std::unique_ptr<GameController> _controller;
    std::unique_ptr<HumanPlayer> _UIuser;
    std::shared_ptr<Player> _player1, _player2;
    std::shared_ptr<Player> _activePlayer;
};

