#include <climits>
#include "tritset.h"
#include "reference.h"

namespace {
    using reference = TritSet::reference;
    using size_type = TritSet::size_type;
    using uint = TritSet::uint;
}

// constructors

TritSet::TritSet(size_type size):
    mCapacity(size), mTritsVec(get_storage_length(size)) {}

// mutators

void TritSet::resize(size_type size) {
    size_type vecSize = get_storage_length(size);
    mTritsVec.resize(vecSize);
}

void TritSet::shrink() {
    // get current tritset length in trits
    size_type setLength = capacity() * TRITS_PER_INT;
    // search the last not-unknown trit's index from the end
    for (; setLength > 0; --setLength) {
        if ((*this)[setLength - 1] != Trit::Unknown) {
            break;
        }
    }
    resize(get_storage_length(setLength));
    mTritsVec.shrink_to_fit();
}

// trit access

Trit TritSet::operator[] (size_type index) const {
    if (index < mCapacity) {
        // get vector's element index and trit's position in this element
        size_type vecIndex = get_element_index(index);
        size_type tritPos = get_trit_position(index);
        // get trit value through the trit mask
        uint tritMask = reference::get_trit_mask(mTritsVec[vecIndex], tritPos);
        return reference::get_trit_value(tritMask);
    }
    return Trit::Unknown;
}

reference TritSet::operator[] (size_type tritIndex) {
    return reference(this, tritIndex);
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
