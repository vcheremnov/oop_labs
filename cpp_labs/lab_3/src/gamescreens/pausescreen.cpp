#include "gamemodel.h"
#include "basescreen.h"
#include "consolewindow.h"
#include "menu.h"

namespace {

using Option = PauseMenuSelector::Option;

class PauseMenu: public ConsoleMenu {
public:
    using ConsoleMenu::ConsoleMenu;
protected:
    void _draw_options(GameModel*) override;
    const std::string &_get_header() override
        { return _header; }
private:
    const std::string _header = "*** Pause Menu ***";
};

void PauseMenu::_draw_options(GameModel *model) {
    auto &pauseMenuSelector = model->pause_menu_selector();
    auto window = _get_window();
    int line = _get_starting_line(), width = window->get_width();
    std::string optName;
    for (Option opt = Option::Continue; opt != Option::Total; ++opt) {
        if (opt == pauseMenuSelector.get_option()) {
            window->set_attributes({TextAttr::Highlight});
        }
        optName = pauseMenuSelector.get_option_name(opt);
        window->print_text_at(line, (width - optName.size()) / 2, optName.c_str());
        window->reset_attributes();
        ++line;
    }
}

class PauseScreen: public ConsoleScreen {
public:
    PauseScreen() {
        auto screenWidth = ConsoleView::CONSOLE_WIDTH;
        auto screenHeight = ConsoleView::CONSOLE_HEIGHT;
        int menuWidth = 23, menuHeight = 7;
        int menuStartX = (screenWidth - menuWidth) / 2;
        int menuStartY = (screenHeight - menuHeight) / 2;
        _add_object(new PauseMenu(menuWidth, menuHeight, menuStartY, menuStartX));
    }
};

// register screen

bool register_screen() {
    ScreenFactory::instance().register_screen<PauseScreen>(GameState::Pause);
    return true;
}

bool screenReg = register_screen();

}
