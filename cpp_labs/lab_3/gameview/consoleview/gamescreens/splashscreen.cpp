#include "gamemodel/gamemodel.h"
#include "gameview/consoleview/consolescreen.h"
#include "gameview/consoleview/consolewindow.h"
#include "gameview/consoleview/consoleview.h"

namespace {

class SplashScreen: public ConsoleScreen {
public:
    SplashScreen() {
        _load_background("ascii/splash");
    }
};

// register screen

bool register_screen() {
    ConsoleScreenFactory::instance().register_screen<SplashScreen>(GameState::SplashScreen);
    return true;
}

bool screenReg = register_screen();

}
