#include "gamemodel.h"
#include "basescreen.h"
#include "consolewindow.h"

namespace {

class SplashScreen: public ConsoleScreen {
public:
    SplashScreen()
        { _load_background("ascii/splash"); }
    void render(GameModel*) override;
};

void SplashScreen::render(GameModel *model) {
    clear();
    printw("%s", _get_background().c_str());
    box(stdscr, 0, 0);
    refresh();
}

// register screen

bool register_screen() {
    ScreenFactory::instance().register_screen<SplashScreen>(GameState::SplashScreen);
    return true;
}

bool screenReg = register_screen();

}
