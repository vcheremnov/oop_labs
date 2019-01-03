#include "gamemodel/gamemodel.h"
#include "gameview/consoleview/consolescreen.h"
#include "gameview/consoleview/consolewindow.h"
#include "gameview/consoleview/consoleview.h"
#include "gameview/consoleview/menu.h"
#include <fstream>
#include <sstream>
#include <memory>

namespace {

using Option = MenuSelector::Option;

// menu box

class MainMenu: public ConsoleMenu {
public:
    using ConsoleMenu::ConsoleMenu;
protected:
    const std::string &_get_header() override
        { return _header; }
    void _draw_options(GameModel*) override;
private:
    const std::string _header = "*** Main Menu ***";
};

void MainMenu::_draw_options(GameModel *model) {
    auto window = _get_window();
    auto &gameData = model->game_data();
    auto &menuSelector = model->menu_selector();
    int line = _get_starting_line(), width = window->get_width();
    std::string optName;
    for (Option opt = Option::StartGame; opt != Option::Total; ++opt) {
        if (!menuSelector.is_active_option(opt)) {
            continue;
        }
        if (opt == menuSelector.get_option()) {
            window->set_attributes({TextAttr::Highlight});
        }
        optName = menuSelector.get_option_name(opt);
        window->print_text_at(line, (width - optName.size()) / 2, optName.c_str());
        window->reset_attributes();
        ++line;
    }
}

// menu screen

class MenuScreen: public ConsoleScreen {
public:
    MenuScreen() {
        _load_background("ascii/menu");
        _add_object(new MainMenu(22, 11, 5, 5));
    }
};

// register screen

bool register_screen() {
    ConsoleScreenFactory::instance().register_screen<MenuScreen>(GameState::MainMenu);
    return true;
}

bool screenReg = register_screen();

}
