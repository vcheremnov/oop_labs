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
    static const int CONSOLE_WIDTH = 80;
    static const int CONSOLE_HEIGHT = 24;
    ConsoleView(GameModel*);
    ~ConsoleView() override;
    void show() override;
private:
    std::unique_ptr<ConsoleWindow> _consoleWin;
    std::map<GameState, std::unique_ptr<ConsoleScreen>> _screens;
};
