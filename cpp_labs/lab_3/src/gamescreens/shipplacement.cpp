#include "gamemodel.h"
#include "basescreen.h"
#include "consolewindow.h"
#include "board.h"
#include <sstream>

namespace {

// accept choice button

class AcceptButton: public ConsoleObject {
public:
    using ConsoleObject::ConsoleObject;
private:
    void _draw_object(GameModel*) override;
    const std::string _buttonText = "Start Game";
};

void AcceptButton::_draw_object(GameModel *model) {
    auto window = _get_window();
    if (model->ship_initializer().placement_done()) {
        window->set_attributes({TextAttr::Highlight});
    }
    else {
        window->set_attributes({TextAttr::Dim});
    }
    window->print_text_at(1, (window->get_width() - _buttonText.length()) / 2, _buttonText.c_str());
    window->reset_attributes();
}

// current player panel

class CurrentPlayerPanel: public ConsoleObject {
public:
    using ConsoleObject::ConsoleObject;
protected:
    void _draw_object(GameModel*) override;
    const std::string _panelHeader = "Field of the";
};

void CurrentPlayerPanel::_draw_object(GameModel *model) {
    auto window = _get_window();
    auto panelWidth = window->get_width();

    auto currentPlayer = model->game_data().get_active_player();
    auto &playerName = model->game_data().get_player_name(currentPlayer);

    window->print_text_at(1, (panelWidth - _panelHeader.length()) / 2, _panelHeader.c_str());
    window->set_attributes({TextAttr::Bold});
    window->print_text_at(2, (panelWidth - playerName.length()) / 2, playerName.c_str());
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

    static const int lineWidth = std::string("Ship1: 0").length();
    static const int startX = (_get_window()->get_width() - lineWidth) / 2 - 1;
    static const int startY = 1;
    auto line = startY;
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
        int screenWidth = ConsoleView::CONSOLE_WIDTH;
        int screenHeight = ConsoleView::CONSOLE_HEIGHT;
        // add board
        int boardWidth = Field::WIDTH * Board::CELL_WIDTH + 3;
        int boardHeight = Field::HEIGHT * Board::CELL_HEIGHT + 3;
        int boardStartX = 20;
        int boardStartY = 1;
        _add_object(new PlacementBoard(boardWidth, boardHeight, boardStartY, boardStartX));

        // add current player panel
        int panelWidth = 16;
        int panelHeight = 4;
        int panelStartX = (boardStartX - panelWidth) / 2;
        int panelStartY = boardStartY;
        _add_object(new CurrentPlayerPanel(panelWidth, panelHeight, panelStartY, panelStartX));

        // add ship info
        int shipPanelWidth = panelWidth;
        int shipPanelHeight = 6;
        int shipPanelStartX = panelStartX;
        int shipPanelStartY = panelStartY + panelHeight;
        _add_object(new ShipInfo(shipPanelWidth, shipPanelHeight, shipPanelStartY, shipPanelStartX));

        // add accept button
        int buttonWidth = panelWidth;
        int buttonHeight = 3;
        int buttonStartX = panelStartX;
        int buttonStartY = screenHeight - buttonHeight - 1;
        _add_object(new AcceptButton(buttonWidth, buttonHeight, buttonStartY, buttonStartX));
    }
};


bool register_screen() {
    ScreenFactory::instance().register_screen<ShipPlacementScreen>(GameState::ShipPlacement);
    return true;
}

bool screenReg = register_screen();

}
