#ifndef REFERENCE_H
#define REFERENCE_H

#include "tritset.h"
#include "trit_handler.h"
#include "tritwise_operations.h"

// A proxy class for accessing individual trits in TritSet

class TritSet::Reference {
public:
    friend class TritSet;
    // conversion to Trit
    operator Trit() const { return value(); }
    // get Trit value of the reference
    Trit value() const {
        return (mElemPtr == nullptr) ?
                    Trit::Unknown : TritHandler::get_value(*mElemPtr, mPos);
    }
    // assignment operators
    Reference &operator= (Trit val);
    Reference &operator= (const Reference &ref) {
        return *this = ref.value();
    }
    Reference &operator|= (Trit val) {
        return *this = (*this | val);
    }
    Reference &operator&= (Trit val) {
        return *this = (*this & val);
    }
private:
    Reference(TritSet *setPtr, size_type tritIndex);
    // internal data
    TritSet *mSetPtr;                           // pointer to the associated tritset
    uint *mElemPtr;                             // pointer to the element storing given trit
    size_type mTritIndex;                       // trit's index in the tritset
    size_type mPos;                             // trit's position in the tritset's storage element
};

#endif // REFERENCE_H
