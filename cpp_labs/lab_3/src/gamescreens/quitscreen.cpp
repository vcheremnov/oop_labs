#include "gamemodel.h"
#include "basescreen.h"
#include "consolewindow.h"

namespace {

class QuitScreen: public ConsoleScreen {
public:
    void render(GameModel *model) override {}
private:

};

bool register_screen() {
    ScreenFactory::instance().register_screen<QuitScreen>(GameState::QuitScreen);
    return true;
}

bool screenReg = register_screen();

}
