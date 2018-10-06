#include <climits>
#include <sstream>
#include <algorithm>
#include "tritset.h"
#include "reference.h"
#include "tritset_aux.h"

// constructors

TritSet::TritSet(size_type size):
    _capacity(size), _storage(TritSetAux::get_storage_size(size)) {
}

// accessors

size_type TritSet::capacity() const {
    return _capacity;
}

size_type TritSet::length() const {
    return _length;
}

size_type TritSet::cardinality(Trit value) const {
    switch (value) {
    case Trit::False:
        return _falseCount;
    case Trit::True:
        return _trueCount;
    default:
        return _length - _falseCount - _trueCount;
    }
}

std::unordered_map<Trit, size_type, TritHash> TritSet::cardinality() const {
    return  {
                {Trit::False, _falseCount},
                {Trit::True, _trueCount},
                {Trit::Unknown, cardinality(Trit::Unknown)}
            };
}

// modifiers

void TritSet::shrink() {
    resize(length());
}

void TritSet::resize(size_type size) {
    // set capacity equal to the given size
    _capacity = size;
    // set minimum possible size for the storage
    size_type storageSize = TritSetAux::get_storage_size(size);
    _storage.resize(storageSize);
    _storage.shrink_to_fit();
    // clear residual trits within the last element of the storage
    if (storageSize > 0) {
        size_type begPos = TritSetAux::get_trit_position(_capacity - 1) + 1,
                  endPos = TritSetAux::TRITS_PER_ELEM;
        TritSetAux::set_value(Trit::Unknown, _storage.back(), begPos, endPos);
    }
    // refresh the logical length
    if (size < _length) {
        _length = _find_length();
    }
}

void TritSet::trim(size_type lastIndex){
    if (lastIndex < _capacity) {
        // set all storage elements after lastIndex to 0 (since 00 <=> Trit::Unknown)
        size_type elemIndex = TritSetAux::get_element_index(lastIndex) + 1;
        std::fill(_storage.begin() + elemIndex, _storage.end(), 0u);
        // reset residual trits within the element containing lastIndex trit
        size_type begPos = TritSetAux::get_trit_position(lastIndex),
                  endPos = TritSetAux::TRITS_PER_ELEM;
        TritSetAux::set_value(Trit::Unknown, _storage[elemIndex], begPos, endPos);
    }
}

// trit access

Trit TritSet::operator[] (size_type tritIndex) const {
    return _get_value_at(tritIndex);
}

Reference TritSet::operator[] (size_type tritIndex) {
    return Reference(*this, tritIndex);
}

// tritwise operations

TritSet &TritSet::operator&= (const TritSet &set) {
    return *this = (*this & set);
}

TritSet &TritSet::operator|= (const TritSet &set) {
    return *this = (*this | set);
}

TritSet TritSet::operator~ () const {
    TritSet resultSet(*this);
    for (size_type ix = 0; ix < length(); ++ix) {
        resultSet._set_value_at(ix, ~resultSet._get_value_at(ix));
    }
    return resultSet;
}

TritSet TritSet::operator| (const TritSet &set) const {
    // initialize result set with the tritset of minimum logical length
    TritSet resultSet = (this->length() < set.length()) ? *this : set;
    // resize if doesn't fit the result's capacity
    size_type maxCapacity = std::max(this->capacity(), set.capacity());
    if (resultSet.capacity() < maxCapacity) {
        resultSet.resize(maxCapacity);
    }
    // get result of the operation
    for (size_type ix = 0; ix < resultSet.length(); ++ix) {
        resultSet._set_value_at(ix, this->_get_value_at(ix) | set._get_value_at(ix));
    }
    return resultSet;
}

TritSet TritSet::operator& (const TritSet &set) const {
    // initialize result set with the tritset of minimum logical length
    TritSet resultSet = (this->length() < set.length()) ? *this : set;
    // resize if doesn't fit the result's capacity
    size_type maxCapacity = std::max(this->capacity(), set.capacity());
    if (resultSet.capacity() < maxCapacity) {
        resultSet.resize(maxCapacity);
    }
    // get result of the operation
    for (size_type ix = 0; ix < resultSet.length(); ++ix) {
        resultSet._set_value_at(ix, this->_get_value_at(ix) & set._get_value_at(ix));
    }
    return resultSet;
}

// private methods

Trit TritSet::_get_value_at(size_type tritIndex) const {
    if (tritIndex < _capacity) {
        // get storage's element index and trit's position in this element
        size_type elemIndex = TritSetAux::get_element_index(tritIndex);
        size_type tritPos = TritSetAux::get_trit_position(tritIndex);
        // get trit value
        return TritSetAux::get_value(_storage[elemIndex], tritPos);
    }
    return Trit::Unknown;
}

void TritSet::_set_value_at(size_type tritIndex, Trit value) {
    if (tritIndex >= _capacity) {
        if (value == Trit::Unknown) {
            // nothing to set
            return;
        }
        resize(tritIndex + 1);
    }
    // get storage's element index and trit's position in this element
    size_type elemIndex = TritSetAux::get_element_index(tritIndex);
    size_type tritPos = TritSetAux::get_trit_position(tritIndex);
    // save old value
    Trit oldValue = _get_value_at(tritIndex);
    // set value in the given position
    TritSetAux::set_value(value, _storage[elemIndex], tritPos);
    // refresh logical length & trit counters
    _update_counters(value, oldValue);
    _update_length(tritIndex, value);
}

void TritSet::_update_counters(Trit setValue, Trit oldValue) {
    if (setValue == Trit::False) {
        ++_falseCount;
    }
    else if (setValue == Trit::True) {
        ++_trueCount;
    }

    if (oldValue == Trit::False) {
        --_falseCount;
    }
    else if (oldValue == Trit::True) {
        --_trueCount;
    }
}

void TritSet::_update_length(size_type setIndex, Trit setValue) {
    if (setIndex + 1 > _length && setValue != Trit::Unknown) {
        _length = setIndex + 1;
    }
    else if (setIndex + 1 == _length && setValue == Trit::Unknown) {
        // the last non-unknown trit was reset; find the new length
        _length = _find_length();
    }
}

size_type TritSet::_find_length() const {
    /*
     * Called in two cases:
     * 1) When tritset's capacity after resize() is less than the previous logical length
     * 2) When the trit at position [_length - 1] was set to Unknown
     * In the first case we can start the search from the current capacity,
     * and in the second case - from the previous length,
     * since trits standing after the previous length are known to be Unknown
    */
    size_type length = std::min(_length, _capacity);
    for (; length > 0; --length) {
        if (_get_value_at(length - 1) != Trit::Unknown) {
            break;
        }
    }
    return length;
}
