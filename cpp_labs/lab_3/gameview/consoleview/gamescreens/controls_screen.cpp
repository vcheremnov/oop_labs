#include "gamemodel/gamemodel.h"
#include "gameview/consoleview/consolescreen.h"
#include "gameview/consoleview/consolewindow.h"
#include "gameview/consoleview/consoleview.h"
#include "gameview/consoleview/consoleobject.h"

namespace {

class ControlsInfo: public ConsoleObject {
public:
    using ConsoleObject::ConsoleObject;
protected:
    void _draw_object(GameModel*) override;
};

void ControlsInfo::_draw_object(GameModel*) {
    auto window = _get_window();
    // common controls
    window->set_attributes({TextAttr::Underline, TextAttr::Bold});
    window->print_text_at(1, 1, "General:");
    window->reset_attributes();
    window->print_text_at(2, 1, "* Navigation: ^, v, ->, <-");
    window->print_text_at(3, 1, "* Accept: ENTER");
    window->print_text_at(4, 1, "* Pause: P");
    // ship placement
    window->set_attributes({TextAttr::Underline, TextAttr::Bold});
    window->print_text_at(5, 1, "Ship placement:");
    window->reset_attributes();
    window->print_text_at(6, 1, "* Change ship type: TAB");
    window->print_text_at(7, 1, "* Change ship: Q, E");
    window->print_text_at(8, 1, "* Rotate ship: SPACE");
    window->print_text_at(9, 1, "* Random placement: R");
    window->print_text_at(10, 1, "* Cancel: Z");
    // battle
    window->set_attributes({TextAttr::Underline, TextAttr::Bold});
    window->print_text_at(11, 1, "Battle:");
    window->reset_attributes();
    window->print_text_at(12, 1, "* Make shot: ENTER");
    window->print_text_at(13, 1, "* Hide/show the field: H");
}

// back button

class BackButton: public ConsoleObject {
public:
    using ConsoleObject::ConsoleObject;
    const std::string& get_button_text()
        { return _buttonText; }
protected:
    void _draw_object(GameModel*) override;
private:
    const std::string _buttonText = "Back to menu";
};

void BackButton::_draw_object(GameModel *model) {
    auto window = _get_window();
    auto buttonWidth = window->get_width();
    auto buttonHeight = window->get_height();

    window->set_attributes({TextAttr::Highlight});
    window->print_text_at(buttonHeight / 2, (buttonWidth - _buttonText.length()) / 2, _buttonText.c_str());
    window->reset_attributes();
}


class ControlsScreen: public ConsoleScreen {
public:
    ControlsScreen() {
        int screenWidth = ConsoleView::CONSOLE_WIDTH;
        int screenHeight = ConsoleView::CONSOLE_HEIGHT;

        // controls info panel
        int panelWidth = 30;
        int panelHeight = 20;
        int panelStartX = (screenWidth - panelWidth) / 2;
        int panelStartY = 1;
        _add_object(new ControlsInfo(panelWidth, panelHeight, panelStartY, panelStartX));

        // back button
        int buttonWidth = 16;
        int buttonHeight = 3;
        int buttonStartX = panelStartX + (panelWidth - buttonWidth) / 2;
        int buttonStartY = panelStartY + panelHeight;
        _add_object(new BackButton(buttonWidth, buttonHeight, buttonStartY, buttonStartX));
    }
};

// register screen

bool register_screen() {
    ConsoleScreenFactory::instance().register_screen<ControlsScreen>(GameState::ControlsMenu);
    return true;
}

bool screenReg = register_screen();

}
