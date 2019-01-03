#include "board.h"
#include "gamemodel/gamemodel.h"
#include <curses.h>

// Board

void Board::_draw_field_grid() {
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

void Board::_draw_field_cells(const Field &field) {
    static const int startX = 1 + CELL_WIDTH / 2, startY = 1 + CELL_HEIGHT / 2;
    auto window = _get_window();
    auto &fieldMap = field.get_field_map();

    // draw field
    for (auto rowIx = 0u; rowIx < fieldMap.size(); ++rowIx) {
        for (auto colIx = 0u; colIx < fieldMap.size(); ++colIx) {
            if (fieldMap[rowIx][colIx] == Field::Cell::Ship) {
                window->add_character_at(CELL_HEIGHT * rowIx + startY,
                                         CELL_WIDTH * colIx + startX, ACS_BOARD);
            }
        }
    }
}

// placement board

void PlacementBoard::_draw_object(GameModel *model) {
    _draw_field_grid();
    _draw_field_cells(model->game_data().get_active_field_pair().first);
    _draw_current_ship(model);
}

void PlacementBoard::_draw_current_ship(GameModel *model) {
    static const int startX = 1 + Board::CELL_WIDTH / 2, startY = 1 + Board::CELL_HEIGHT / 2;
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
        window->add_character_at(Board::CELL_HEIGHT * cell.first + startY,
                                 Board::CELL_WIDTH * cell.second + startX, ACS_BOARD);
    }
    // reset colors & attributes
    window->reset_attributes();
    window->reset_color_pair();
}

// BattleBoard

void BattleBoard::_draw_object(GameModel *model) {
    auto &gameData = model->game_data();
    _draw_field_grid();
    _draw_field_cells(gameData.get_active_field_pair().second);
    if (!gameData.game_has_finished()) {
        if (model->move_maker().move_was_made()) {
            _highlight_target(model);
        }
        else if (gameData.player_is_human(gameData.get_active_player())) {
            _draw_cursor(model);
        }
    }
}

void BattleBoard::_draw_field_cells(const Field &field) {
    auto window = _get_window();
    auto &fieldMap = field.get_field_map();

    // draw field
    for (auto rowIx = 0u; rowIx < fieldMap.size(); ++rowIx) {
        for (auto colIx = 0u; colIx < fieldMap.size(); ++colIx) {
            auto posX = Board::CELL_WIDTH * colIx + startX;
            auto posY = Board::CELL_HEIGHT * rowIx + startY;
            switch (fieldMap[rowIx][colIx]) {
            case Field::Cell::Unknown:
                break;
            case Field::Cell::Ship:
                window->add_character_at(posY, posX, ACS_BOARD);
                break;
            case Field::Cell::ShipDestroyed:
                window->set_attributes({TextAttr::Bold});
                window->add_character_at(posY, posX, chtype('X'));
                break;
            case Field::Cell::Miss:
                window->set_attributes({TextAttr::Dim});
                window->add_character_at(posY, posX, ACS_BULLET);
                break;
            default:
                break;
            }
            window->reset_attributes();
        }
    }
}

void BattleBoard::_draw_cursor(GameModel *model) {
    auto &moveMaker = model->move_maker();
    auto &targetCell = moveMaker.get_target();
    auto window = _get_window();
    // choose color
    window->set_color_pair(moveMaker.move_is_valid() ? TextColor::GREEN_ON_BLACK : TextColor::RED_ON_BLACK);
    // draw cursor
    auto posY = Board::CELL_HEIGHT * targetCell.first + startY;
    auto posX = Board::CELL_WIDTH * targetCell.second + startX;
    window->add_character_at(posY, posX, chtype('@'));
    window->reset_color_pair();
}

void BattleBoard::_highlight_target(GameModel *model) {
    auto window = _get_window();
    auto &moveMaker = model->move_maker();
    auto &targetCell = moveMaker.get_last_shot_cell();
    auto moveResult = moveMaker.last_move_result();

    auto posY = Board::CELL_HEIGHT * targetCell.first + startY;
    auto posX = Board::CELL_WIDTH * targetCell.second + startX;
    window->set_attributes({TextAttr::Bold});
    if (moveResult == MoveMaker::MoveResult::Miss) {
        window->set_color_pair(TextColor::RED_ON_BLACK);
        window->add_character_at(posY, posX, ACS_BULLET);
    }
    else {
        window->set_color_pair(TextColor::GREEN_ON_BLACK);
        window->add_character_at(posY, posX, chtype('X'));
    }

    window->reset_color_pair();
    window->reset_attributes();
}

// MiniBoard

void MiniBoard::_draw_object(GameModel *model) {
    auto &gameData = model->game_data();
    if (gameData.field_is_hidden(gameData.get_active_player())) {
        _hide();
    }
    else {
        _draw_field_grid();
        _draw_field_cells(gameData.get_active_field_pair().first);
    }
}

void MiniBoard::_hide() {
    auto window = _get_window();
    window->set_attributes({TextAttr::Dim});
    for (auto line = 0; line < window->get_height(); ++line) {
        for (auto col = 0; col < window->get_width(); ++col) {
            window->add_character_at(line, col, ACS_CKBOARD);
        }
    }
    window->reset_attributes();
}

void MiniBoard::_draw_field_grid() {
    auto window = _get_window();
    window->set_attributes({TextAttr::Dim});
    for (auto line = startY; line < endY; line += MiniBoard::CELL_HEIGHT) {
        for (auto col = startX; col < endX; col += MiniBoard::CELL_WIDTH) {
            window->add_character_at(line, col, chtype('-'));
        }
    }
    window->reset_attributes();
}

void MiniBoard::_draw_field_cells(const Field &field) {
    auto window = _get_window();
    auto &fieldMap = field.get_field_map();

    window->set_attributes({TextAttr::Bold});
    for (auto row = 0; row < Field::HEIGHT; ++row) {
        for (auto col = 0; col < Field::WIDTH; ++col) {
            auto posX = startX + col * MiniBoard::CELL_WIDTH;
            auto posY = startY + row * MiniBoard::CELL_HEIGHT;
            switch (fieldMap[row][col]) {
            case Field::Cell::Ship:
                window->set_color_pair(TextColor::GREEN_ON_BLACK);
                window->add_character_at(posY, posX, ACS_BOARD);
                break;
            case Field::Cell::ShipDestroyed:
                window->set_color_pair(TextColor::RED_ON_BLACK);
                window->add_character_at(posY, posX, chtype('X'));
                break;
            case Field::Cell::Miss:
                window->add_character_at(posY, posX, ACS_BULLET);
                break;
            default:
                break;
            }
            window->reset_color_pair();
        }
    }
    window->reset_attributes();
}
