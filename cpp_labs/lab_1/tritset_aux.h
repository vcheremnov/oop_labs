#ifndef TRITSET_AUX_H
#define TRITSET_AUX_H

#include "tritset.h"

namespace {

using Reference = TritSet::Reference;
using size_type = TritSet::size_type;
using uint = TritSet::uint;

}   // anonymous namespace

namespace TritSetAux {

extern const uint BITS_PER_TRIT;
extern const uint TRITS_PER_ELEM;

Trit get_value(uint element, size_type pos);
void set_value(Trit value, uint &element, size_type pos);
void set_value(Trit value, uint &element, size_type begPos, size_type endPos);

size_type count_trits(Trit value, uint element, size_type begPos, size_type endPos);

inline size_type get_storage_size(size_type tritsNum) {
    return tritsNum / TRITS_PER_ELEM + (tritsNum % TRITS_PER_ELEM != 0);
}

inline size_type get_element_index(size_type tritIndex) {
    return tritIndex / TRITS_PER_ELEM;
}

inline size_type get_trit_position(size_type tritIndex) {
    return tritIndex % TRITS_PER_ELEM;
}

}   // namespace TritSetAux

#endif // TRITSET_AUX_H
