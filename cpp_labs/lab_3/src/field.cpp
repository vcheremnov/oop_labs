#include <algorithm>
#include "field.h"

// field

void Field::clear(Cell cellType) {
    for (auto &row: _map) {
        std::fill(row.begin(), row.end(), cellType);
    }
}
