#include <climits>
#include <sstream>
#include "tritset.h"
#include "reference.h"
#include "trit_handler.h"

namespace {
    using Reference = TritSet::Reference;
    using size_type = TritSet::size_type;
    using uint = TritSet::uint;
}

// constructors

TritSet::TritSet(size_type size):
    mCapacity(size), mTritsVec(get_storage_length(size)) {}

// mutators

void TritSet::resize(size_type size) {
    mCapacity = size;
    size_type storageSize = get_storage_length(size);
    mTritsVec.resize(storageSize);
    mTritsVec.shrink_to_fit();
    // clear the residual trits within the last element of the storage
    if (storageSize > 0) {
        size_type begPos = get_trit_position(mCapacity), endPos = TRITS_PER_INT;
        TritHandler::set_value(Trit::Unknown, mTritsVec.back(), begPos, endPos);
    }
}

void TritSet::shrink() {
    // search the last not-unknown trit's index from the end
    size_type newCapacity = capacity();
    for (; newCapacity > 0; --newCapacity) {
        if ((*this)[newCapacity - 1] != Trit::Unknown) {
            break;
        }
    }
    resize(newCapacity);
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

Trit TritSet::operator[] (size_type index) const {
    if (index < mCapacity) {
        // get vector's element index and trit's position in this element
        size_type vecIndex = get_element_index(index);
        size_type tritPos = get_trit_position(index);
        // get trit value
        return TritHandler::get_value(mTritsVec[vecIndex], tritPos);
    }
    return Trit::Unknown;
}

Reference TritSet::operator[] (size_type tritIndex) {
    return Reference(this, tritIndex);
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
