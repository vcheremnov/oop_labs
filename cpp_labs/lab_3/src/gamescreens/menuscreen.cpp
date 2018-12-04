#include "gamemodel.h"
#include "basescreen.h"
#include "consolewindow.h"
#include <fstream>
#include <sstream>
#include <memory>

namespace {

using Option = MenuSelector::Option;

// menu box

class MenuBox: public ConsoleObject {
public:
    using ConsoleObject::ConsoleObject;
protected:
    void _draw_object(GameModel*) override;
};

void MenuBox::_draw_object(GameModel *model) {
    auto window = _get_window();
    int line = 2, width = window->get_width();
    std::string optName;
    for (Option opt = Option::StartGame; opt != Option::Total; ++opt) {
        if (!model->menu_selector().is_active_option(opt)) {
            continue;
        }
        if (opt == model->menu_selector().get_option()) {
            window->set_attributes({TextAttr::Highlight});
        }
        optName = model->menu_selector().get_option_name(opt);
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
        _add_object(new MenuBox(22, 11, 5, 5));
    }
};

// register screen

bool register_screen() {
    ScreenFactory::instance().register_screen<MenuScreen>(GameState::MenuSelect);
    return true;
}

bool screenReg = register_screen();

}
