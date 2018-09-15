#include <climits>
#include "tritset.h"

namespace {
    using reference = TritSet::reference;
    using size_type = TritSet::size_type;
    using uint = TritSet::uint;
    const uint BITS_PER_TRIT = 2u;

    size_type div_ceil(size_type numerator, size_type denominator) {
        return numerator / denominator + (numerator % denominator != 0);
    }

    size_type get_vector_length(TritSet::size_type tritsNum) {
        return div_ceil(BITS_PER_TRIT * tritsNum, CHAR_BIT * sizeof(uint));
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

void TritSet::trim(size_type lastIndex) {

}

// assignment

TritSet &TritSet::operator&= (const TritSet &set) {

}

TritSet &TritSet::operator|= (const TritSet &set) {

}

// trit access

Trit TritSet::operator[] (size_type index) const {

}

reference TritSet::operator[] (size_type index) {

}

// ternary operations

TritSet TritSet::operator~ () const {

}

TritSet TritSet::operator| (const TritSet &set) const {

}

TritSet TritSet::operator& (const TritSet &set) const {

}

// TritSet::reference implementation

uint reference::get_trit_mask(Trit val) {
    switch (val) {
    case Trit::False:
        return mFalseMask;
    case Trit::True:
        return mTrueMask;
    default:
        return mUnknownMask;
    }
}

Trit reference::get_trit_value(uint tritMask) {
    switch (tritMask) {
    case mFalseMask:
        return Trit::False;
    case mTrueMask:
        return Trit::True;
    default:
        return Trit::Unknown;
    }
}

Trit reference::inverted_trit_value(Trit val) {
    switch (val) {
    case Trit::False:
        return Trit::True;
    case Trit::True:
        return Trit::False;
    default:
        return Trit::Unknown;
    }
}

reference &reference::operator= (Trit val) {
    // get a bitmask for the given trit value
    uint tritMask = get_trit_mask(val);
    // refresh element's value
    set_trit(tritMask);

    return *this;
}

reference &reference::operator= (const reference &ref) {
    // retrieve trit mask from the given object
    uint tritMask = get_trit_mask_from(ref);
    // refresh element's value
    set_trit(tritMask);

    return *this;
}

Trit reference::operator~ () const {
    // retrieve trit mask from the current object
    uint tritMask = get_trit_mask_from(*this);
    // get a trit's value from its bitmask
    Trit tritValue = get_trit_value(tritMask >> mPos);

    return inverted_trit_value(tritValue);
}
