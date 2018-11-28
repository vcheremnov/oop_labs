#include "gamemodel.h"
#include "basescreen.h"
#include "consolewindow.h"

namespace {


class Board: public ConsoleObject {
public:
    static const int CELL_WIDTH = 2;
    static const int CELL_HEIGHT = 1;
    Board(size_type width, size_type height, pos line, pos col):
        ConsoleObject(width, height, line, col) {}
private:
    void _draw_object(GameModel*) override;
    void _draw_ships(GameModel*);
};

void Board::_draw_object(GameModel *model) {
    auto window = _get_window();

    static const int startX = 1, startY = 1;
    static const int endX = CELL_WIDTH * Field::WIDTH + 1,
                     endY = CELL_HEIGHT * Field::HEIGHT + 1;

    for(int line = startY; line <= endY; line += CELL_HEIGHT) {
        for(int col = startX; col <= endX; col += CELL_WIDTH) {
            window->add_character_at(line, col, chtype('.'));
        }
    }
    _draw_ships(model);

//    static const int startX = 1, startY = 1;
//    const int endX = window->get_width() - 2, endY = window->get_height() - 2;
//    // draw horizontal lines
//    for(int line = startY; line <= endY; line += CELL_HEIGHT) {
//        for(int col = startX; col <= endX; ++col) {
//            window->add_character_at(line, col, ACS_HLINE);
//        }
//    }
//    // draw vertical lines
//    for(int col = startX; col <= endX; col += CELL_WIDTH) {
//        for(int line = startY; line <= endY; ++line) {
//            window->add_character_at(line, col, ACS_VLINE);
//        }
//    }
//    // add corners
//    window->add_character_at(startY, startX, ACS_ULCORNER);
//    window->add_character_at(startY, endX, ACS_URCORNER);
//    window->add_character_at(endY, endX, ACS_LRCORNER);
//    window->add_character_at(endY, startX, ACS_LLCORNER);
//    // add T symbols
//    for(int line = startY + CELL_HEIGHT; line <= endY - CELL_HEIGHT; line += CELL_HEIGHT) {
//        window->add_character_at(line, startX, ACS_LTEE);
//        window->add_character_at(line, endX, ACS_RTEE);
//        for(int col = startX + CELL_WIDTH; col <= endX - CELL_WIDTH; col += CELL_WIDTH) {
//            window->add_character_at(line, col, ACS_PLUS);
//        }
//    }
//    for(int col = startX + CELL_WIDTH; col <= endX - CELL_WIDTH; col += CELL_WIDTH) {
//        window->add_character_at(startY, col, ACS_TTEE);
//        window->add_character_at(endY, col, ACS_BTEE);
//    }
}

void Board::_draw_ships(GameModel *model) {
//    auto &ship = model->get_ship();
//    auto window = _get_window();

//    const int startX = 1, startY = 1;
//    auto shipCoordinates = ship.get_coordinates();
//    for (auto &cell: shipCoordinates) {
//        window->add_character_at(CELL_HEIGHT * cell.first + startY,
//                                 CELL_WIDTH * cell.second + startX, chtype('#'));
//    }
//    model->notify_views();
}

class ShipPlacementScreen: public ConsoleScreen {
public:
    ShipPlacementScreen() {

        int boardWidth = Field::WIDTH * Board::CELL_WIDTH + 1;
        int boardHeight = Field::HEIGHT * Board::CELL_HEIGHT + 2;
        _add_object(new Board(boardWidth, boardHeight, 1, 3));
    }
};


bool register_screen() {
    ScreenFactory::instance().register_screen<ShipPlacementScreen>(GameState::ShipPlacement);
    return true;
}

bool screenReg = register_screen();

}
