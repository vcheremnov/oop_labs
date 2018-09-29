#ifndef REFERENCE_H
#define REFERENCE_H

#include "tritset.h"
#include "trits.h"

// A proxy class for accessing individual trits in TritSet

class TritSet::Reference {
    friend class TritSet;
public:
    // get value of the reference
    Trit value() const;
    // conversion to Trit
    operator Trit() const {
        return value();
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
    Reference(TritSet &set, size_type tritIndex);
    // internal data
    TritSet &mSet;                              // reference to the associated tritset
    uint *mElemPtr;                             // pointer to the element storing given trit
    size_type mTritIndex;                       // trit's index in the tritset
    size_type mPos;                             // trit's position within the tritset's storage element
};

#endif // REFERENCE_H
