#ifndef REFERENCE_H
#define REFERENCE_H

#include "tritset.h"

// operations on trits

Trit operator& (Trit val1, Trit val2);
Trit operator| (Trit val1, Trit val2);
Trit operator~ (Trit val);

// A proxy class for accessing individual trits in TritSet

class TritSet::reference {
    friend class TritSet;
    // constructor
    reference(TritSet *setPtr, size_type tritIndex);
    // internal data
    TritSet *mSetPtr;                           // pointer to the associated tritset
    uint *mElemPtr;                             // pointer to the element storing given trit
    size_type mElemIndex = 0, mPos = 0;         // index of the storage element / trit position in it
    // bitmasks for every trit value
    static const uint mUnknownMask = 0b00u;
    static const uint mFalseMask = 0b01u;
    static const uint mTrueMask = 0b10u;
    static const uint mPosMask = 0b11u;
    // static methods
    static Trit get_trit_value(uint tritMask);
    static uint get_trit_mask(Trit val);
    static uint get_trit_mask(uint elem, size_type pos) {
        return (elem & get_position_mask(pos)) >> pos;
    }
    static uint get_position_mask(size_type pos) {
        return mPosMask << pos;
    }
    // internal methods
    uint get_position_mask() const {
        return get_position_mask(mPos);
    }
    uint get_trit_mask() const {
        return (mElemPtr == nullptr) ? mUnknownMask : get_trit_mask(*mElemPtr, mPos);
    }
    Trit get_trit_value() const {
        return get_trit_value(get_trit_mask());
    }
public:
    // assignment operators
    reference &operator= (Trit val);
    reference &operator= (const reference &ref) {
        return *this = ref.get_trit_value();
    }
    reference &operator&= (const reference &ref) {
        return *this = (*this & ref);
    }
    reference &operator|= (Trit val) {
        return *this = (*this | val);
    }
    reference &operator&= (Trit val) {
        return *this = (*this & val);
    }
    reference &operator|= (const reference &ref) {
        return *this = (*this | ref);
    }
    // operations on trits
    Trit operator& (Trit val) const {
        return get_trit_value() & val;
    }
    Trit operator| (Trit val) const {
        return get_trit_value() | val;
    }
    Trit operator& (const reference &ref) const {
        return get_trit_value() & ref.get_trit_value();
    }
    Trit operator| (const reference &ref) const {
        return get_trit_value() | ref.get_trit_value();
    }
    Trit operator~ () const {
        return ~get_trit_value();
    }
    // relational operators
    bool operator== (Trit val) const {
        return get_trit_value() == val;
    }
    bool operator!= (Trit val) const {
        return get_trit_value() != val;
    }
    bool operator== (const reference &ref) const {
        return get_trit_value() == ref.get_trit_value();
    }
    bool operator!= (const reference &ref) const {
        return get_trit_value() != ref.get_trit_value();
    }
};

#endif // REFERENCE_H
