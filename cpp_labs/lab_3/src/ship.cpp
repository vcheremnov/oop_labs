#include "headers/ship.h"
#include <algorithm>

// ship

const std::map<Ship::Type, std::size_t> Ship::_centerIndexes =
{
    {Type::Ship1, 0},
    {Type::Ship2, 0},
    {Type::Ship3, 1},
    {Type::Ship4, 1}
};

Ship::Ship(Type shipType): _shipType(shipType) {
    switch (shipType) {
    case Type::Ship1:
        _body = { {ShipCell(0, 0)} };
        break;
    case Type::Ship2:
        _body = { {ShipCell(0, 0)}, {ShipCell(0, 1)} };
        break;
    case Type::Ship3:
        _body = { {ShipCell(0, 0)}, {ShipCell(0, 1)},
                  {ShipCell(0, 2)} };
        break;
    case Type::Ship4:
        _body = { {ShipCell(0, 0)}, {ShipCell(0, 1)},
                  {ShipCell(0, 2)}, {ShipCell(0, 3)} };
        break;
    }
    _bodyCondition.assign(_body.size(), true);
}

void Ship::hit_ship(const ShipCell &pos) {
    for (auto cellIndex = 0u; cellIndex < _body.size(); ++cellIndex) {
        if (_body[cellIndex] == pos) {
            _bodyCondition[cellIndex] = false;
            break;
        }
    }
}

bool Ship::is_destroyed() const {
    for (auto cellIndex = 0u; cellIndex < _bodyCondition.size(); ++cellIndex) {
        if (_bodyCondition[cellIndex]) {
            return false;
        }
    }
    return true;
}

bool Ship::is_overlapped_by(const Ship &ship) const {
    for (auto &shipCell: ship._body) {
        for (auto &cell: _body) {
            if (shipCell == cell) {
                return true;
            }
        }
    }
    return false;
}

void Ship::rotate() {
    // rotate
    auto centerIx = _centerIndexes.at(_shipType);
    for (auto ix = 0u; ix < _body.size(); ++ix) {
        auto posX = _body[ix].second;
        // x = x_c - (y - y_c)
        _body[ix].second = _body[centerIx].second + _body[centerIx].first - _body[ix].first;
        // y = y_c + (x - x_c)
        _body[ix].first = _body[centerIx].first + posX - _body[centerIx].second;
    }
    // correct position if ship crosses the border
    _correct_position();
}

bool Ship::shift(ShiftDirection direction) {
    auto bodyCopy = _body;
    switch (direction) {
    case ShiftDirection::Left:
        _shift_left(bodyCopy);
        break;
    case ShiftDirection::Up:
        _shift_up(bodyCopy);
        break;
    case ShiftDirection::Right:
        _shift_right(bodyCopy);
        break;
    case ShiftDirection::Down:
        _shift_down(bodyCopy);
        break;
    }
    if (_check_validity_of(bodyCopy)) {
        _body = bodyCopy;
        return true;
    }
    return false;
}

void Ship::_shift_left(ShipBody &body) {
    for (auto &cell: body) {
        --cell.second;
    }
}

void Ship::_shift_up(ShipBody &body) {
    for (auto &cell: body) {
        --cell.first;
    }
}

void Ship::_shift_right(ShipBody &body) {
    for (auto &cell: body) {
        ++cell.second;
    }
}

void Ship::_shift_down(ShipBody &body) {
    for (auto &cell: body) {
        ++cell.first;
    }
}

void Ship::_correct_position() {
    for (auto &cell: _body) {
        while (cell.second < 0) {
            _shift_right(_body);
        }
        while (cell.second >= Field::WIDTH) {
            _shift_left(_body);
        }
        while (cell.first < 0) {
            _shift_down(_body);
        }
        while (cell.first >= Field::HEIGHT) {
            _shift_up(_body);
        }
    }
}

bool Ship::_check_validity_of(const ShipBody &body) {
    for (auto &cell: body) {
        if (cell.first < 0 || cell.first >= Field::HEIGHT ||
                cell.second < 0 || cell.second >= Field::WIDTH) {
            return false;
        }
    }
    return true;
}

bool Ship::is_horizontal() {
    if (_shipType == Type::Ship1) {
        return true;
    }
    auto centerIx = _centerIndexes.at(_shipType);
    // there is at least one cell standing after the central cell
    return _body[centerIx].first == _body[centerIx + 1].first;
}

bool Ship::is_vertical() {
    if (_shipType == Type::Ship1) {
        return true;
    }
    auto centerIx = _centerIndexes.at(_shipType);
    // there is at least one cell standing after the central cell
    return _body[centerIx].second == _body[centerIx + 1].second;
}

// friends

bool operator==(const Ship &ship1, const Ship &ship2) {
    const auto &body1 = ship1.get_coordinates();
    const auto &body2 = ship2.get_coordinates();
    if (body1.size() != body2.size()) {
        return false;
    }
    bool isCommonCell = false;
    for (auto &shipCell1: body1) {
        isCommonCell = false;
        for (auto &shipCell2: body2) {
            if (shipCell1 == shipCell2) {
                isCommonCell = true;
                break;
            }
        }
        if (!isCommonCell) {
            return false;
        }
    }
    return true;
}
