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
    void render(GameModel*) override;
};

void MenuBox::render(GameModel *model) {
    _get_window()->clear();

    int line = 2, width = _get_window()->get_width();
    std::string optName;
    for (Option opt = Option::StartGame; opt != Option::Total; ++opt) {
        if (opt == model->get_menu_selector().get_option()) {
            _get_window()->set_attributes({TextAttr::Highlight});
        }
        optName = model->get_menu_selector().get_option_name(opt);
        _get_window()->print_text_at(line, (width - optName.size()) / 2, optName.c_str());
        _get_window()->reset_attributes({TextAttr::Highlight});
        ++line;
    }
    // draw border
    _get_window()->draw_border();
    _get_window()->refresh();
}

// menu screen

class MenuScreen: public ConsoleScreen {
public:
    MenuScreen();
    void render(GameModel*) override;
private:
};

MenuScreen::MenuScreen() {
    _load_background("ascii/menu");
    _add_object(new MenuBox(20, 10, 5, 5));
}

void MenuScreen::render(GameModel *model) {
    // draw background
    clear();
    printw("%s", _get_background().c_str());
    box(stdscr, 0, 0);
    refresh();
    // draw objects
    _render_objects(model);
}

// register screen

bool register_screen() {
    ScreenFactory::instance().register_screen<MenuScreen>(GameState::MenuSelect);
    return true;
}

bool screenReg = register_screen();

}
