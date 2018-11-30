#include "gamemodel.h"
#include "basescreen.h"
#include "consolewindow.h"
#include <sstream>

namespace {

// accept choice button

class AcceptButton: public ConsoleObject {
public:
    static const std::string buttonText;
    AcceptButton(size_type width, size_type height, pos line, pos col):
        ConsoleObject(width, height, line, col) {}
private:
    void _draw_object(GameModel*) override;
};

const std::string AcceptButton::buttonText = "Start Game";

void AcceptButton::_draw_object(GameModel *model) {
    auto window = _get_window();
    if (model->ship_initializer().placementDone()) {
        window->set_attributes({TextAttr::Highlight});
    }
    else {
        window->set_attributes({TextAttr::Dim});
    }
    window->print_text_at(1, (window->get_width() - buttonText.length()) / 2, buttonText.c_str());
    window->reset_attributes({TextAttr::Highlight, TextAttr::Dim});
}

// ship info panel

class ShipInfo: public ConsoleObject {
public:
    ShipInfo(size_type width, size_type height, pos line, pos col):
        ConsoleObject(width, height, line, col) {}
private:
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
        window->reset_attributes({TextAttr::Bold, TextAttr::Underline});
        ++line;
    }
}

// board window

class Board: public ConsoleObject {
public:
    static const int CELL_WIDTH = 4;
    static const int CELL_HEIGHT = 2;
    Board(size_type width, size_type height, pos line, pos col):
        ConsoleObject(width, height, line, col) {}
private:
    void _draw_object(GameModel*) override;
    void _draw_field(GameModel*);
    void _draw_current_ship(GameModel*);
    void _draw_placed_ships(GameModel*);
};

void Board::_draw_object(GameModel *model) {
    _draw_field(model);
    _draw_placed_ships(model);
    _draw_current_ship(model);
}

void Board::_draw_current_ship(GameModel *model) {
    static const int startX = 1 + CELL_WIDTH / 2, startY = 1 + CELL_HEIGHT / 2;
    auto window = _get_window();
    auto &shipInitializer = model->ship_initializer();
    auto &curShip = shipInitializer.current_ship();
    // set colors & attributes
    if (shipInitializer.ship_is_placed()) {
        window->set_color_pair(TextColor::BLUE_ON_BLACK);
    }
    else if (shipInitializer.ship_is_overlapping()) {
        window->set_color_pair(TextColor::RED_ON_BLACK);
    }
    else {
        window->set_color_pair(TextColor::GREEN_ON_BLACK);
    }
    window->set_attributes({TextAttr::Bold});
    // draw current ship
    for (auto &cell: curShip.get_body()) {
        window->add_character_at(CELL_HEIGHT * cell.first + startY,
                                 CELL_WIDTH * cell.second + startX, chtype('#'));
    }
    // reset colors & attributes
    window->reset_attributes({TextAttr::Bold});
//    window->set_color_pair(TextColor::BLUE_ON_BLACK);
//    for (auto &cell: curShip.get_periphery()) {
//        window->add_character_at(CELL_HEIGHT * cell.first + startY,
//                                 CELL_WIDTH * cell.second + startX, chtype('*'));
//    }
    window->reset_color_pair();
}

void Board::_draw_placed_ships(GameModel *model) {
    static const int startX = 1 + CELL_WIDTH / 2, startY = 1 + CELL_HEIGHT / 2;
    auto window = _get_window();
    auto &fieldMap = model->get_field_pair().first.get_field_map();

    // draw field
    for (auto rowIx = 0u; rowIx < fieldMap.size(); ++rowIx) {
        for (auto colIx = 0u; colIx < fieldMap.size(); ++colIx) {
            if (fieldMap[rowIx][colIx] == Field::Cell::Ship) {
                window->add_character_at(CELL_HEIGHT * rowIx + startY,
                                         CELL_WIDTH * colIx + startX, chtype('#'));
            }
        }
    }
}

void Board::_draw_field(GameModel *model) {
    auto window = _get_window();
    static const int startX = 1, startY = 1;
    const int endX = window->get_width() - 2, endY = window->get_height() - 2;

    // draw horizontal lines
    for(int line = startY; line <= endY; line += CELL_HEIGHT) {
        for(int col = startX; col <= endX; ++col) {
            window->add_character_at(line, col, ACS_HLINE);
        }
    }
    // draw vertical lines
    for(int col = startX; col <= endX; col += CELL_WIDTH) {
        for(int line = startY; line <= endY; ++line) {
            window->add_character_at(line, col, ACS_VLINE);
        }
    }
    // add corners
    window->add_character_at(startY, startX, ACS_ULCORNER);
    window->add_character_at(startY, endX, ACS_URCORNER);
    window->add_character_at(endY, endX, ACS_LRCORNER);
    window->add_character_at(endY, startX, ACS_LLCORNER);
    // add T symbols
    for(int line = startY + CELL_HEIGHT; line <= endY - CELL_HEIGHT; line += CELL_HEIGHT) {
        window->add_character_at(line, startX, ACS_LTEE);
        window->add_character_at(line, endX, ACS_RTEE);
        for(int col = startX + CELL_WIDTH; col <= endX - CELL_WIDTH; col += CELL_WIDTH) {
            window->add_character_at(line, col, ACS_PLUS);
        }
    }
    for(int col = startX + CELL_WIDTH; col <= endX - CELL_WIDTH; col += CELL_WIDTH) {
        window->add_character_at(startY, col, ACS_TTEE);
        window->add_character_at(endY, col, ACS_BTEE);
    }
}

class ShipPlacementScreen: public ConsoleScreen {
public:
    ShipPlacementScreen() {
        int boardWidth = Field::WIDTH * Board::CELL_WIDTH + 3;
        int boardHeight = Field::HEIGHT * Board::CELL_HEIGHT + 3;
        _add_object(new Board(boardWidth, boardHeight, 1, 20));
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
