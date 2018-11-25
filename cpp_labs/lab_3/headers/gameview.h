#pragma once

#include "basescreen.h"
#include "gamestate.h"
#include <map>

class GameModel;

class GameView {
public:
    GameView(GameModel*);
    void update(GameModel *model);
    virtual ~GameView() = default;
    virtual void show() = 0;
protected:
    bool _isUpdated = true;
    GameModel *_model = nullptr;
};

class ConsoleView: public GameView {
public:
    ConsoleView(GameModel*);
    ~ConsoleView() override;
    void show() override;
private:
    const int consoleWidth = 80, consoleHeight = 24;
    std::map<GameState, std::unique_ptr<ConsoleScreen>> _screens;
};
