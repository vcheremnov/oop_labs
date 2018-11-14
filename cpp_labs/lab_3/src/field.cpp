#include <algorithm>
#include "field.h"

void Field::clear() {
    for (auto &row: _map) {
        std::fill(row.begin(), row.end(), Cell::Empty);
    }
}
