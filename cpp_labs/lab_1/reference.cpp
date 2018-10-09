#include "tritset.h"

using Reference = TritSet::Reference;

Reference::operator Trit() const {
    return _value();
}

// assignment operators

Reference &Reference::operator= (Trit value) {
    _set._set_value_at(_tritIndex, value);
    return *this;
}

Reference &Reference::operator= (const Reference &ref) {
    return *this = ref._value();
}

Reference &Reference::operator|= (Trit value) {
    return *this = (*this | value);
}

Reference &Reference::operator&= (Trit value) {
    return *this = (*this & value);
}

// private methods

Reference::Reference(TritSet &set, TritSet::size_type tritIndex):
    _set(set), _tritIndex(tritIndex) {}

Trit Reference::_value() const {
    return _set._get_value_at(_tritIndex);
}
