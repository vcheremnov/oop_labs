#include <algorithm>
#include "field.h"

// field

Field::Field():
    _map(HEIGHT, FieldRow(WIDTH, Cell::Empty)) {}

void Field::clear(Cell cellType) {
    for (auto &row: _map) {
        std::fill(row.begin(), row.end(), cellType);
    }
}

bool Field::is_valid_pos(pos row, pos col) {
    return (row >= 0) && (row < HEIGHT) &&
           (col >= 0) && (col < WIDTH);
}
