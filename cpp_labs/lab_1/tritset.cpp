#include <climits>
#include "tritset.h"
#include "reference.h"
#include "trit_handler.h"

namespace {
    using reference = TritSet::Reference;
    using size_type = TritSet::size_type;
    using uint = TritSet::uint;
}

// constructors

TritSet::TritSet(size_type size):
    mCapacity(size), mTritsVec(get_storage_length(size)) {}

// mutators

void TritSet::resize(size_type size) {
    mCapacity = get_storage_length(size);
    mTritsVec.resize(mCapacity);
    mTritsVec.shrink_to_fit();
}

void TritSet::shrink() {
    // search the last not-unknown trit's index from the end
    for (; mCapacity > 0; --mCapacity) {
        if ((*this)[mCapacity - 1] != Trit::Unknown) {
            break;
        }
    }
    resize(mCapacity);
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

reference TritSet::operator[] (size_type tritIndex) {
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
