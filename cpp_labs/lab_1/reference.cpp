#include "reference.h"

namespace {
    using Reference = TritSet::Reference;
    using TritHandler = TritSet::TritHandler;
    using size_type = TritSet::size_type;
    using uint = TritSet::uint;
}

// constructor

Reference::Reference(TritSet *setPtr, size_type tritIndex):
    mSetPtr(setPtr), mElemPtr(nullptr), mTritIndex(tritIndex) {

    mElemIndex = get_element_index(tritIndex);
    mPos = get_trit_position(tritIndex);
    if (mElemIndex < mSetPtr->capacity()) {
        mElemPtr = &mSetPtr->get_elem_for(tritIndex);
    }
}

// assignment

Reference &Reference::operator= (Trit val) {
    if (mElemPtr == nullptr) {
        if (val == Trit::Unknown) {
            // nothing to set
            return *this;
        }
        // resize associated tritset to store not-unknown value
        mSetPtr->resize(mTritIndex);
        mElemPtr = &mSetPtr->get_elem_for(mTritIndex);
    }
    TritHandler::set_value(val, *mElemPtr, mPos);

    return *this;
}
