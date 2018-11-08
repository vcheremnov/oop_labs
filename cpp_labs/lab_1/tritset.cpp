#include <climits>
#include <sstream>
#include <algorithm>
#include "tritset.h"
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

std::unordered_map<Trit, size_type> TritSet::cardinality() const {
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
    _forget_trit_values(size);
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
    if (_capacity < _length) {
        _length = _find_length(_capacity);
    }
}

void TritSet::trim(size_type lastIndex) {
    if (lastIndex < _length) {
        _forget_trit_values(lastIndex);
        // index of the element following the element containing lastIndex trit
        size_type begElemIndex = TritSetAux::get_element_index(lastIndex) + 1;
        // index of the element following the element with the last set trit
        size_type endElemIndex = TritSetAux::get_element_index(_length - 1) + 1;
        // fill the range [begElemIndex, endElemIndex) with zeros
        std::fill(_storage.begin() + begElemIndex, _storage.begin() + endElemIndex, 0u);
        // reset residual trits within the element containing lastIndex trit
        size_type begPos = TritSetAux::get_trit_position(lastIndex),
                  endPos = TritSetAux::TRITS_PER_ELEM;
        TritSetAux::set_value(Trit::Unknown, _storage[begElemIndex - 1], begPos, endPos);
        // change the logical length
        _length = _find_length(lastIndex);
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
    if (this != &set) {
        // resize if doesn't fit the result's capacity
        if (capacity() < set.capacity()) {
            resize(set.capacity());
        }
        // find maximum logical length
        size_type maxLength = std::max(length(), set.length());
        // get result of the operation
        for (size_type ix = 0; ix < maxLength; ++ix) {
            _set_value_at(ix, _get_value_at(ix) & set._get_value_at(ix));
        }
    }
    return *this;
}

TritSet &TritSet::operator|= (const TritSet &set) {
    if (this != &set) {
        // resize if doesn't fit the result's capacity
        if (capacity() < set.capacity()) {
            resize(set.capacity());
        }
        // find maximum logical length
        size_type maxLength = std::max(length(), set.length());
        // get result of the operation
        for (size_type ix = 0; ix < maxLength; ++ix) {
            _set_value_at(ix, _get_value_at(ix) | set._get_value_at(ix));
        }
    }
    return *this;
}

TritSet operator~ (const TritSet &set) {
    TritSet resultSet = set;
    for (size_type ix = 0; ix < set.length(); ++ix) {
        resultSet._set_value_at(ix, ~resultSet._get_value_at(ix));
    }
    return resultSet;
}

TritSet operator| (const TritSet &set1, const TritSet &set2) {
    return (set1.capacity() > set2.capacity()) ?
                TritSet(set1) |= set2 : TritSet(set2) |= set1;
}

TritSet operator& (const TritSet &set1, const TritSet &set2) {
    return (set1.capacity() > set2.capacity()) ?
                TritSet(set1) &= set2 : TritSet(set2) &= set1;
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
    _update_length(value, tritIndex);
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

void TritSet::_forget_trit_values(size_type startIndex) {
    if (startIndex < _length) {
        size_type startElemIndex = TritSetAux::get_element_index(startIndex);
        size_type startPos = TritSetAux::get_trit_position(startIndex);
        _trueCount = TritSetAux::count_trits(Trit::True, _storage[startElemIndex], 0, startPos);
        _falseCount = TritSetAux::count_trits(Trit::False, _storage[startElemIndex], 0, startPos);
        for (size_type elemIndex = 0; elemIndex < startElemIndex; ++elemIndex) {
            _trueCount += TritSetAux::count_trits(Trit::True, _storage[elemIndex], 0, TritSetAux::TRITS_PER_ELEM);
            _falseCount += TritSetAux::count_trits(Trit::False, _storage[elemIndex], 0, TritSetAux::TRITS_PER_ELEM);
        }
    }
}

void TritSet::_update_length(Trit setValue, size_type setIndex) {
    if (setIndex + 1 > _length && setValue != Trit::Unknown) {
        _length = setIndex + 1;
    }
    else if (setIndex + 1 == _length && setValue == Trit::Unknown) {
        // the last non-unknown trit was reset; find the new length
        _length = _find_length(_length);
    }
}

size_type TritSet::_find_length(size_type lenUpperEstimate) const {
    // search starting from the upper estimate of the length
    size_type length = lenUpperEstimate;
    for (; length > 0; --length) {
        if (_get_value_at(length - 1) != Trit::Unknown) {
            break;
        }
    }
    return length;
}
