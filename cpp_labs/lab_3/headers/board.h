#pragma once

#include "object.h"
#include "field.h"

class GameModel;

class BaseBoard: public ConsoleObject {
public:
    using ConsoleObject::ConsoleObject;
protected:
    virtual void _draw_object(GameModel*) = 0;
    virtual void _draw_field_grid() = 0;
    virtual void _draw_field_cells(const Field&) = 0;
};

class Board: public BaseBoard {
public:
    static const int CELL_WIDTH = 4;
    static const int CELL_HEIGHT = 2;
    using BaseBoard::BaseBoard;
protected:
    void _draw_field_grid() override;
    void _draw_field_cells(const Field&) override;
};

class PlacementBoard: public Board {
public:
    using Board::Board;
protected:
    void _draw_object(GameModel*) override;
private:
    void _draw_current_ship(GameModel*);
};

class BattleBoard: public Board {
public:
    using Board::Board;
protected:
    void _draw_object(GameModel*) override;
    void _draw_field_cells(const Field&) override;
private:
    void _highlight_target(GameModel*);
    void _draw_cursor(GameModel*);
    static const int startX = 1 + Board::CELL_WIDTH / 2,
                     startY = 1 + Board::CELL_HEIGHT / 2;
};

class MiniBoard: public Board {
public:
    static const int CELL_WIDTH = 2;
    static const int CELL_HEIGHT = 1;
    using Board::Board;
protected:
    void _draw_object(GameModel*) override;
    void _draw_field_grid() override;
    void _draw_field_cells(const Field&) override;
private:
    static const int startX = 2, startY = 1;
    static const int endX = Field::WIDTH * MiniBoard::CELL_WIDTH + startX,
                     endY = Field::HEIGHT * MiniBoard::CELL_HEIGHT + startY;
};
