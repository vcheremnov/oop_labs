#include <algorithm>
#include "field.h"

// field

void Field::clear(Cell cellType) {
    for (auto &row: _map) {
        std::fill(row.begin(), row.end(), cellType);
    }
}

bool Field::is_valid_pos(pos row, pos col) {
    return (row >= 0) && (row < Field::HEIGHT) &&
           (col >= 0) && (col < Field::WIDTH);
}
