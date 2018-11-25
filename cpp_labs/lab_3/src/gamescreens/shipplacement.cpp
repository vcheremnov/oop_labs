#include "gamemodel.h"
#include "basescreen.h"
#include "consolewindow.h"

namespace {

class ShipPlacementScreen: public ConsoleScreen {
public:
    ShipPlacementScreen() = default;
};

bool register_screen() {
    ScreenFactory::instance().register_screen<ShipPlacementScreen>(GameState::ShipPlacement);
    return true;
}

bool screenReg = register_screen();

}
