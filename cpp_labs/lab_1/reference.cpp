#include "reference.h"

namespace {
    using reference = TritSet::reference;
    using size_type = TritSet::size_type;
    using uint = TritSet::uint;
}

// constructor

reference::reference(TritSet *setPtr, size_type tritIndex):
    mSetPtr(setPtr), mElemPtr(nullptr) {
    mElemIndex = get_element_index(tritIndex);
    mPos = get_trit_position(tritIndex);
    if (mElemIndex < mSetPtr->capacity()) {
        mElemPtr = &mSetPtr->mTritsVec[mElemIndex];
    }
}

// static methods

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

// assignment

reference &reference::operator= (Trit val) {
    if (mElemPtr == nullptr) {
        if (val == Trit::Unknown) {
            // nothing to set
            return *this;
        }
        // resize associated tritset to store not-unknown value
        mSetPtr->resize(mElemIndex + 1);
        mElemPtr = &mSetPtr->mTritsVec[mElemIndex];
    }
    // reset bits in the needed position
    *mElemPtr &= ~get_position_mask();
    // set bits corresponding to the given trit value
    *mElemPtr |= get_trit_mask(val) << mPos;

    return *this;
}

// trit operations

Trit operator& (Trit val1, Trit val2) {
    if (val1 == Trit::True) {
        return val2;
    }
    if (val1 == Trit::Unknown) {
        if (val2 == Trit::False) {
            return Trit::False;
        }
        return Trit::Unknown;
    }
    return Trit::False;
}

Trit operator| (Trit val1, Trit val2) {
    if (val1 == Trit::False) {
        return val2;
    }
    if (val1 == Trit::Unknown) {
        if (val2 == Trit::True) {
            return Trit::True;
        }
        return Trit::Unknown;
    }
    return Trit::True;
}

Trit operator~ (Trit val) {
    switch (val) {
    case Trit::False:
        return Trit::True;
    case Trit::True:
        return Trit::False;
    default:
        return Trit::Unknown;
    }
}
