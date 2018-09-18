#ifndef REFERENCE_H
#define REFERENCE_H

#include "tritset.h"
#include "trit_handler.h"
#include "tritwise_operations.h"

// A proxy class for accessing individual trits in TritSet

class TritSet::Reference {
public:
    friend class TritSet;
    Trit value() const {                                    // get value of the reference
        return TritHandler::get_value(*mElemPtr, mPos);
    }
    // assignment operators
    Reference &operator= (Trit val);
    Reference &operator= (const Reference &ref) {
        return *this = ref.value();
    }
    Reference &operator&= (const Reference &ref) {
        return *this = (*this & ref);
    }
    Reference &operator|= (Trit val) {
        return *this = (*this | val);
    }
    Reference &operator&= (Trit val) {
        return *this = (*this & val);
    }
    Reference &operator|= (const Reference &ref) {
        return *this = (*this | ref);
    }
    // operations on trits
    Trit operator& (Trit val) const {
        return value() & val;
    }
    Trit operator| (Trit val) const {
        return value() | val;
    }
    Trit operator& (const Reference &ref) const {
        return value() & ref.value();
    }
    Trit operator| (const Reference &ref) const {
        return value() | ref.value();
    }
    Trit operator~ () const {
        return ~value();
    }
    // relational operators
    bool operator== (Trit val) const {
        return value() == val;
    }
    bool operator!= (Trit val) const {
        return value() != val;
    }
    bool operator== (const Reference &ref) const {
        return value() == ref.value();
    }
    bool operator!= (const Reference &ref) const {
        return value() != ref.value();
    }
private:
    Reference(TritSet *setPtr, size_type tritIndex);
    // internal data
    TritSet *mSetPtr;                           // pointer to the associated tritset
    uint *mElemPtr;                             // pointer to the element storing given trit
    size_type mTritIndex, mElemIndex, mPos;
};

#endif // REFERENCE_H
