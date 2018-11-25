#include <algorithm>
#include "field.h"

void Field::clear(Cell cellType) {
    for (auto &row: _map) {
        std::fill(row.begin(), row.end(), cellType);
    }
}
