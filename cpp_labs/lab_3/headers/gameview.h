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
    GameModel *_get_model()
        { return _model; }
    bool _is_updated()
        { return _isUpdated; }
    void _reset_update()
        { _isUpdated = false; }
private:
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
    std::unique_ptr<ConsoleWindow> _consoleWin;
    std::map<GameState, std::unique_ptr<ConsoleScreen>> _screens;
};
