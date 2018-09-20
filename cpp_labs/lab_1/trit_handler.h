#ifndef TRITHANDLER_H
#define TRITHANDLER_H

#include "tritset.h"

// a class working with trits within a single element of type "uint"

class TritSet::TritHandler {
public:
    static Trit get_value(const uint &element, size_type pos);
    static void set_value(Trit value, uint &element, size_type pos);
    static void set_value(Trit value, uint &element, size_type begPos, size_type endPos);
private:
    // bitmasks for every trit value
    static const uint mUnknownMask = 0b00u;
    static const uint mFalseMask = 0b01u;
    static const uint mTrueMask = 0b10u;
    static const uint mPosMask = 0b11u;
    // static methods
    static Trit get_trit_value(uint tritMask);
    static uint get_trit_mask(Trit val);
    static uint get_position_mask(size_type pos) {
        return mPosMask << (pos * BITS_PER_TRIT);
    }
    static uint shift_left(uint mask, size_type posShift) {
        return mask << (posShift * BITS_PER_TRIT);
    }
    static uint shift_right(uint mask, size_type posShift) {
        return mask >> (posShift * BITS_PER_TRIT);
    }
};

#endif // TRITHANDLER_H
