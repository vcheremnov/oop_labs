#pragma once

#include "gameview/gameview.h"
#include "consolewindow.h"
#include "consolescreen.h"

class ConsoleView: public GameView {
public:
    static const int CONSOLE_WIDTH = 80;
    static const int CONSOLE_HEIGHT = 25;
    ConsoleView(GameModel*);
    ~ConsoleView() override;
    void show() override;
private:
    // private methods
    void _init_colors();
    // data
    std::unique_ptr<ConsoleWindow> _consoleWin;
    std::map<GameState, std::unique_ptr<ConsoleScreen>> _screens;
};
