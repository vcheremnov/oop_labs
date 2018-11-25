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
    MenuBox(size_type width, size_type height, pos line, pos col):
        ConsoleObject(width, height, line, col) {}
protected:
    void _draw_object(GameModel*) override;
};

void MenuBox::_draw_object(GameModel *model) {
    auto window = _get_window();
    int line = 2, width = window->get_width();
    std::string optName;
    for (Option opt = Option::StartGame; opt != Option::Total; ++opt) {
        if (opt == model->menu_selector().get_option()) {
            window->set_attributes({TextAttr::Highlight});
        }
        optName = model->menu_selector().get_option_name(opt);
        window->print_text_at(line, (width - optName.size()) / 2, optName.c_str());
        window->reset_attributes({TextAttr::Highlight});
        ++line;
    }
}

// menu screen

class MenuScreen: public ConsoleScreen {
public:
    MenuScreen() {
        _load_background("ascii/menu");
        _add_object(new MenuBox(20, 10, 5, 5));
    }
};

// register screen

bool register_screen() {
    ScreenFactory::instance().register_screen<MenuScreen>(GameState::MenuSelect);
    return true;
}

bool screenReg = register_screen();

}
