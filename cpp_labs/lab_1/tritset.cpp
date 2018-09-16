#include <climits>
#include "tritset.h"
#include "reference.h"

// auxiliary stuff

namespace {
    using reference = TritSet::reference;
    using size_type = TritSet::size_type;
    using uint = TritSet::uint;

    const uint BITS_PER_TRIT = 2u;
    const uint TRITS_PER_INT = CHAR_BIT * sizeof(uint) / BITS_PER_TRIT;

    inline size_type div_ceil(size_type numerator, size_type denominator) {
        return numerator / denominator + (numerator % denominator != 0);
    }

    inline size_type get_vector_length(size_type tritsNum) {
        return div_ceil(tritsNum, TRITS_PER_INT);
    }

    inline size_type get_vector_index(size_type tritIndex) {
        return tritIndex / TRITS_PER_INT;
    }

    inline size_type get_trit_position(size_type tritIndex) {
        return tritIndex % TRITS_PER_INT;
    }
}

// TritSet implementation

// constructors

TritSet::TritSet(size_type size):
    mCapacity(size), mTritsVec(get_vector_length(size)) {
    // add initial counters for every trit value
    mTritCount.emplace(Trit::False, 0);
    mTritCount.emplace(Trit::Unknown, mCapacity);
    mTritCount.emplace(Trit::True, 0);
}

// mutators

void TritSet::resize(size_type size) {
    size_type vecSize = get_vector_length(size);
    mTritsVec.resize(vecSize);
}

void TritSet::shrink() {
    mCapacity = mLength;
    resize(mCapacity);
    mTritsVec.shrink_to_fit();
}

void TritSet::trim(size_type lastIndex) {       // remake!
    for (size_type ix = lastIndex; ix < mLength; ++ix) {
        mTritsVec[ix] = Trit::Unknown;
    }
    // find the new mLength value:
    // not implemented yet
}

// trit access

Trit TritSet::operator[] (size_type index) const {
    if (index < mCapacity) {
        // get vector's element index and trit's position in this element
        size_type vecIndex = get_vector_index(index);
        size_type tritPos = get_trit_position(index);
        // get trit value through the trit mask
        uint tritMask = reference::get_trit_mask(mTritsVec[vecIndex], tritPos * BITS_PER_TRIT);
        return reference::get_trit_value(tritMask);
    }
    return Trit::Unknown;
}

reference TritSet::operator[] (size_type index) {
    if (index < mCapacity) {
        // get vector's element index and trit's position in this element
        size_type vecIndex = get_vector_index(index);
        size_type tritPos = get_trit_position(index);
        // return reference to the trit
        return reference(&mTritsVec[vecIndex], tritPos * BITS_PER_TRIT);
    }
    // reference to the non-existing object
    return reference();
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
    for (size_type ix = 0; ix < mCapacity; ++ix) {
        resultSet[ix] |= set[ix];
    }
    return resultSet;
}

TritSet TritSet::operator& (const TritSet &set) const {
    TritSet resultSet(*this);
    if (resultSet.capacity() < set.capacity()) {
        resultSet.resize(set.capacity());
    }
    for (size_type ix = 0; ix < mCapacity; ++ix) {
        resultSet[ix] &= set[ix];
    }
    return resultSet;
}
