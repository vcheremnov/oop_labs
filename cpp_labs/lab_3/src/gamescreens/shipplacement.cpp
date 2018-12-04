#include "gamemodel.h"
#include "basescreen.h"
#include "consolewindow.h"
#include "board.h"
#include <sstream>

namespace {

// accept choice button

class AcceptButton: public ConsoleObject {
public:
    static const std::string buttonText;
    using ConsoleObject::ConsoleObject;
private:
    void _draw_object(GameModel*) override;
};

const std::string AcceptButton::buttonText = "Start Game";

void AcceptButton::_draw_object(GameModel *model) {
    auto window = _get_window();
    if (model->ship_initializer().placement_done()) {
        window->set_attributes({TextAttr::Highlight});
    }
    else {
        window->set_attributes({TextAttr::Dim});
    }
    window->print_text_at(1, (window->get_width() - buttonText.length()) / 2, buttonText.c_str());
    window->reset_attributes();
}

// ship info panel

class ShipInfo: public ConsoleObject {
public:
    using ConsoleObject::ConsoleObject;
protected:
    void _draw_object(GameModel*) override;
};

void ShipInfo::_draw_object(GameModel *model) {
    auto &shipInitializer = model->ship_initializer();
    auto curShipType = shipInitializer.current_ship().get_type();
    auto window = _get_window();

    static const int startX = 1, startY = 1;
    auto line = startX;
    for (auto shipType = Ship::Type::Ship1; shipType != Ship::Type::Total; ++shipType) {
        if (shipType == curShipType) {
            window->set_attributes({TextAttr::Bold, TextAttr::Underline});
        }
        window->print_text_at(line, startX, "Ship%d: %dx", line, shipInitializer.ships_remained(shipType));
        window->reset_attributes();
        ++line;
    }
}

class ShipPlacementScreen: public ConsoleScreen {
public:
    ShipPlacementScreen() {
        int boardWidth = Field::WIDTH * Board::CELL_WIDTH + 3;
        int boardHeight = Field::HEIGHT * Board::CELL_HEIGHT + 3;
        _add_object(new PlacementBoard(boardWidth, boardHeight, 1, 20));
        _add_object(new ShipInfo(15, 6, 1, 1));
        _add_object(new AcceptButton(AcceptButton::buttonText.length() + 2, 3, 20, 1));
    }
};


bool register_screen() {
    ScreenFactory::instance().register_screen<ShipPlacementScreen>(GameState::ShipPlacement);
    return true;
}

bool screenReg = register_screen();

}
