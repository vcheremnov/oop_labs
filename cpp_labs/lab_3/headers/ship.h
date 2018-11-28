#pragma once

#include <vector>
#include <map>
#include "field.h"

class Ship {
public:
    enum ShiftDirection {
        Up, Down, Left, Right
    };
    enum class Type {
        Ship1, Ship2, Ship3, Ship4, Total
    };
    Ship(Type);
    // left pos <=> y, right pos <=> x
    using ShipCell = std::pair<Field::pos, Field::pos>;
    using ShipBody = std::vector<ShipCell>;
    void rotate();
    bool shift(ShiftDirection);
    void hit_ship(const ShipCell&);
    bool is_destroyed() const;
    // is it really needed?? (can just take fields's cell state & check)
    bool is_overlapped_by(const Ship&) const;
    // test
    const ShipBody &get_coordinates() const
        { return _body; }
    bool is_horizontal();
    bool is_vertical();
    // friends
    friend bool operator==(const Ship&, const Ship&);
private:
    Type _shipType;
    ShipBody _body;
    std::vector<bool> _bodyCondition;
    // private methods
    void _correct_position();
    void _shift_left(ShipBody&);
    void _shift_up(ShipBody&);
    void _shift_right(ShipBody&);
    void _shift_down(ShipBody&);
    bool _check_validity_of(const ShipBody&);
    // ship centers
    const static std::map<Type, std::size_t> _centerIndexes;
};

bool operator==(const Ship &ship1, const Ship &ship2);
