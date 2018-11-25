#include "gamemodel.h"
#include "basescreen.h"
#include "consolewindow.h"

namespace {

class SplashScreen: public ConsoleScreen {
public:
    SplashScreen() {
        _load_background("ascii/splash");
    }
};

// register screen

bool register_screen() {
    ScreenFactory::instance().register_screen<SplashScreen>(GameState::SplashScreen);
    return true;
}

bool screenReg = register_screen();

}
