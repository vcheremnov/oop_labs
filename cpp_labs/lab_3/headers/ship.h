#pragma once

#include <vector>
#include <map>
#include "field.h"

class Ship {
public:
    enum class Type {
        Ship1, Ship2, Ship3, Ship4, Total
    };
    Ship(Type);
    // left pos <=> y, right pos <=> x
    using ShipCell = std::pair<Field::pos, Field::pos>;
    using ShipBody = std::vector<ShipCell>;
    using ShipPeriphery = std::vector<ShipCell>;
    void hit_ship(const ShipCell&);
    bool is_destroyed() const;
    // test
    const ShipBody &get_body() const
        { return _body; }
    const ShipPeriphery &get_periphery() const
        { return _shipPeriphery; }
    Type get_type() const
        { return _shipType; }
    bool is_horizontal() const;
    bool is_vertical() const;
    // friends
    friend class ShipInitializer;
    friend bool operator==(const Ship&, const Ship&);
private:
    Type _shipType;
    ShipBody _body;
    ShipPeriphery _shipPeriphery;
    std::vector<bool> _bodyCondition;
    const static std::map<Type, std::size_t> _centerIndexes;
    // private methods
    bool _rotate();
    void _init_periphery();
    void _correct_position();
    void _set_body(const ShipBody&, const ShipPeriphery&);
    static void _shift_left(ShipBody&, ShipPeriphery&);
    static void _shift_up(ShipBody&, ShipPeriphery&);
    static void _shift_right(ShipBody&, ShipPeriphery&);
    static void _shift_down(ShipBody&, ShipPeriphery&);
    static bool _check_validity_of(const ShipBody&);
};

bool operator==(const Ship &ship1, const Ship &ship2);
Ship::Type &operator++(Ship::Type&);
Ship::Type &operator--(Ship::Type&);