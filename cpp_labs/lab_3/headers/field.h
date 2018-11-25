#pragma once

#include <cstddef>
#include <vector>
#include <set>

class Field {
public:
    // cell types
    enum class Cell {
        Empty, ShipDestoryed, Ship, Miss, Unknown
    };
    // type names
    using size_type = std::size_t;
    using pos = std::size_t;
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

class Ship {
public:
    using Pos = std::pair<Field::pos, Field::pos>;
    using ShipPart = std::pair<bool, Pos>;
    enum class Type {
        Ship1, Ship2, Ship3, Ship4
    };
//    Ship(Type shipType): _shipType(shipType) {
//        auto beg = _defaultParts.begin();
//        switch (shipType) {
//        case Type::Ship1:
//            _shipParts.insert(beg, beg + 1);
//            break;
//        case Type::Ship2:
//            _shipParts.insert(beg, beg + 2);
//            break;
//        case Type::Ship3:
//            _shipParts.insert(beg, beg + 3);
//            break;
//        case Type::Ship4:
//            _shipParts.insert(beg, beg + 4);
//            break;
//        }
//    }
//    void hit_ship(Pos pos);
//    bool is_destroyed();
//private:
//    const std::set<ShipPart> _defaultParts =
//        {ShipPart(true, Pos(0, 0)), ShipPart(true, Pos(0, 1)),
//         ShipPart(true, Pos(0, 2)), ShipPart(true, Pos(0, 3))};
//    Type _shipType;
//    std::set<ShipPart> _shipParts;
};
