#include "gameview.h"
#include "gamemodel.h"
#include <ncurses.h>
#include <stdexcept>

// base view

GameView::GameView(GameModel *model): _model(model) {
    if (model == nullptr) {
        throw std::runtime_error("GameView::GameView(): model is NULL");
    }
}

void GameView::update(GameModel *model) {
    if (_model == model) {
        _isUpdated = true;
    }
}

// console view

ConsoleView::ConsoleView(GameModel *model):
    GameView (model) {
    // init ncurses
//    use_env(true);
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    curs_set(0);
    // "create" main window
    _consoleWin = std::make_unique<ConsoleWindow>();
    // resize main window
    _consoleWin->resize(CONSOLE_WIDTH, CONSOLE_HEIGHT);
    // init screens
    ScreenFactory::instance().get_all_screens(_screens);
    for (auto &mapItem: _screens) {
        mapItem.second->set_console_window(_consoleWin.get());
    }
}

ConsoleView::~ConsoleView() {
    endwin();
}

void ConsoleView::show() {
    if (_is_updated()) {
        _reset_update();
        GameState gameState = _get_model()->get_game_state();
        _screens[gameState]->render(_get_model());
    }
}
