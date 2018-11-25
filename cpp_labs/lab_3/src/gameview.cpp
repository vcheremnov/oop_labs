#include "gameview.h"
#include "gamemodel.h"
#include <ncurses.h>
#include <stdexcept>

// base view

GameView::GameView(GameModel *model) {
    if (model == nullptr) {
        throw std::runtime_error("GameView::GameView(): model is NULL");
    }
    _model = model;
}

void GameView::update(GameModel *model) {
    if (_model == model) {
        _isUpdated = true;
    }
}

// console view

ConsoleView::ConsoleView(GameModel *model): GameView (model) {

    // init ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    curs_set(0);
    wresize(stdscr, consoleHeight, consoleWidth);
    ScreenFactory::instance().get_all_screens(_screens);
}

ConsoleView::~ConsoleView() {
    endwin();
}

void ConsoleView::show() {
    if (_isUpdated) {
        GameState gameState = _model->get_game_state();
        _screens[gameState]->render(_model);
    }
}
