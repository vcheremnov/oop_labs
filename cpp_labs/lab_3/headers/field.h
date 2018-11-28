#pragma once

#include <cstddef>
#include <vector>
#include <map>

class Field {
public:
    // cell types
    enum class Cell {
        Empty, ShipDestoryed, Ship, Miss, Unknown
    };
    // type names
    using size_type = std::size_t;
    using pos = long;
    using FieldRow = std::vector<Cell>;
    using Map = std::vector<FieldRow>;
    // static members
    static const size_type HEIGHT = 10, WIDTH = 10;
    // constructor
    Field(): _map(HEIGHT, FieldRow(WIDTH, Cell::Empty)) {}
    // public methods
    void set_cell_type(pos row, pos col, Cell cellType)
        { _map[row][col] = cellType; }
    Cell get_cell_type(pos row, pos col)
        { return _map[row][col]; }
    void clear(Cell cellType = Cell::Empty);
private:
    Map _map;
};
