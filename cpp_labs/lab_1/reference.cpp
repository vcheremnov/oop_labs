#include "reference.h"
#include "tritset_aux.h"

// get value of the reference

Trit Reference::value() const {
    return (mElemPtr == nullptr) ?
                Trit::Unknown : TritSetAux::get_value(*mElemPtr, mPos);
}

// constructor

Reference::Reference(TritSet &set, size_type tritIndex):
    mSet(set), mTritIndex(tritIndex) {

    mPos = TritSetAux::get_trit_position(tritIndex);
    mElemPtr = mSet.get_elem_address(tritIndex);
}

// assignment

Reference &Reference::operator= (Trit val) {
    if (mElemPtr == nullptr) {
        if (val == Trit::Unknown) {
            // nothing to set
            return *this;
        }
        // resize associated tritset to store not-unknown value
        mSet.resize(mTritIndex + 1);
        mElemPtr = mSet.get_elem_address(mTritIndex);
    }
    TritSetAux::set_value(val, *mElemPtr, mPos);

    return *this;
}
