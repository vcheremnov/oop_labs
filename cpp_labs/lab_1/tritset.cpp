#include <climits>
#include <sstream>
#include "tritset.h"
#include "reference.h"
#include "tritset_aux.h"

// constructors

TritSet::TritSet(size_type size):
    mCapacity(size), mStorage(TritSetAux::get_storage_length(size)) {}

// accessors

size_type TritSet::cardinality(Trit value) const {
    size_type count = 0;
    for (size_type ix = 0; ix < length(); ++ix) {
        if (get_value_at(ix) == value) {
            ++count;
        }
    }
    return count;
}

std::unordered_map<Trit, size_type, TritHash> TritSet::cardinality() const {
    std::unordered_map<Trit, size_type, TritHash> tritCounter;
    for (size_type ix = 0; ix < length(); ++ix) {
        ++tritCounter[get_value_at(ix)];
    }
    return tritCounter;
}

// modifiers

size_type TritSet::length() const {
    // search the last not-unknown trit's index from the end
    size_type len = capacity();
    for (; len > 0; --len) {
        if (get_value_at(len - 1) != Trit::Unknown) {
            break;
        }
    }
    return len;
}

void TritSet::resize(size_type size) {
    // set capacity equal to the given size
    mCapacity = size;
    // shrink the set to the minimum possible size
    size_type storageSize = TritSetAux::get_storage_length(size);
    mStorage.resize(storageSize);
    mStorage.shrink_to_fit();
    // clear residual trits within the last element of the storage
    if (storageSize > 0) {
        size_type begPos = TritSetAux::get_trit_position(mCapacity), endPos = TritSetAux::TRITS_PER_ELEM;
        TritSetAux::set_value(Trit::Unknown, mStorage.back(), begPos, endPos);
    }
}

void TritSet::trim(size_type lastIndex){
    if (lastIndex < mCapacity) {
        // set all storage elements after lastIndex to 0
        size_type elemIndex = TritSetAux::get_element_index(lastIndex) + 1;
        std::fill(mStorage.begin() + elemIndex, mStorage.end(), 0u);
        // reset residual trits within the element containing lastIndex trit
        size_type begPos = TritSetAux::get_trit_position(lastIndex), endPos = TritSetAux::TRITS_PER_ELEM;
        TritSetAux::set_value(Trit::Unknown, mStorage[elemIndex], begPos, endPos);
    }
}

// string representation & output

std::ostream &operator<< (std::ostream &os, const TritSet &set) {
    for (size_type ix = 0; ix < set.capacity(); ++ix) {
        switch (set[ix]) {
        case Trit::False:
            os << '0';
            break;
        case Trit::True:
            os << '1';
            break;
        default:
            os << '?';
        }
    }
    return os;
}

std::string TritSet::get_string_repr() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

// trit access

Reference TritSet::operator[] (size_type tritIndex) {
    return Reference(*this, tritIndex);
}

// tritwise operations

TritSet TritSet::operator~ () const {
    TritSet resultSet(*this);
    for (size_type ix = 0; ix < capacity(); ++ix) {
        resultSet[ix] = ~resultSet[ix];
    }
    return resultSet;
}

TritSet TritSet::operator| (const TritSet &set) const {
    TritSet resultSet(*this);
    if (resultSet.capacity() < set.capacity()) {
        resultSet.resize(set.capacity());
    }
    for (size_type ix = 0; ix < resultSet.capacity(); ++ix) {
        resultSet[ix] |= set[ix];
    }
    return resultSet;
}

TritSet TritSet::operator& (const TritSet &set) const {
    TritSet resultSet(*this);
    if (resultSet.capacity() < set.capacity()) {
        resultSet.resize(set.capacity());
    }
    for (size_type ix = 0; ix < resultSet.capacity(); ++ix) {
        resultSet[ix] &= set[ix];
    }
    return resultSet;
}

// private methods

Trit TritSet::get_value_at(size_type tritIndex) const {
    if (tritIndex < mCapacity) {
        // get vector's element index and trit's position in this element
        size_type vecIndex = TritSetAux::get_element_index(tritIndex);
        size_type tritPos = TritSetAux::get_trit_position(tritIndex);
        // get trit value
        return TritSetAux::get_value(mStorage[vecIndex], tritPos);
    }
    return Trit::Unknown;
}

const uint *TritSet::get_elem_address(size_type tritIndex) const {
    return (tritIndex < mCapacity) ?
                &mStorage.at(TritSetAux::get_element_index(tritIndex)) : nullptr;
}

uint *TritSet::get_elem_address(size_type tritIndex) {
    return const_cast<uint*>(const_cast<const TritSet*>(this)->get_elem_address(tritIndex));
}


