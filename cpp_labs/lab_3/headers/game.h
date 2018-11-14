#ifndef GAME_H
#define GAME_H

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
    enum class GameCondition {
        SplashScreen, MenuScreen, GameScreen, StatsScreen
    };
    // constructor
    Game(GameType gameType);
    // public methods
    void run();
private:
    void startBattle();
    std::shared_ptr<Player> _player1, _player2;
    std::shared_ptr<GameController> _controller;
    std::shared_ptr<GameView> _view;
    GameModel _model;
    AILevel _aiPlayer1, _aiPlayer2;
};

#endif // GAME_H
