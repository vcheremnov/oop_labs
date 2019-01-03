#include "consoleview.h"
#include "gamemodel/gamemodel.h"
#include <curses.h>
#include <stdexcept>

ConsoleView::ConsoleView(GameModel *model):
    GameView (model) {
    // init ncurses
    initscr();
    noecho();
    keypad(stdscr, true);
    curs_set(0);
    // turn on colors
    _init_colors();
    // fix ncurses bug:
    // clearing the screen by getch() when used for the first time
    halfdelay(1);
    getch();
    cbreak();
    // "create" main window
    wresize(stdscr, CONSOLE_HEIGHT, CONSOLE_WIDTH);
    _consoleWin.reset(new ConsoleWindow(ConsoleView::CONSOLE_WIDTH,
                                        ConsoleView::CONSOLE_HEIGHT, 0, 0));
    // init screens
    ConsoleScreenFactory::instance().get_all_screens(_screens);
    for (auto &mapItem: _screens) {
        mapItem.second->set_console_window(_consoleWin.get());
    }
}

void ConsoleView::_init_colors() {
    if (!has_colors()) {
        throw std::runtime_error("Terminal doesn't support colors!");
    }
    start_color();

    init_pair(static_cast<short>(TextColor::RED_ON_BLACK), COLOR_RED, COLOR_BLACK);
    init_pair(static_cast<short>(TextColor::GREEN_ON_BLACK), COLOR_GREEN, COLOR_BLACK);
    init_pair(static_cast<short>(TextColor::BLUE_ON_BLACK), COLOR_BLUE, COLOR_BLACK);
}


ConsoleView::~ConsoleView() {
    endwin();
}

void ConsoleView::show() {
    if (_is_updated()) {
        _reset_update();
        GameState gameState = _get_model()->game_data().get_game_state();
        _screens[gameState]->render(_get_model());
    }
}

namespace {

bool register_view() {
    GameViewFactory::instance().register_controller<ConsoleView>(GameType::ConsoleGame);
    return true;
}

bool regView = register_view();

} // anonymous namespace
