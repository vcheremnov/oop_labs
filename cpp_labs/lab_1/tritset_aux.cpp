#include "tritset_aux.h"

namespace {

const uint mUnknownMask = 0b00u;
const uint mFalseMask = 0b01u;
const uint mTrueMask = 0b10u;
const uint mPosMask = 0b11u;

uint get_trit_mask(Trit val) {
    switch (val) {
    case Trit::False:
        return mFalseMask;
    case Trit::True:
        return mTrueMask;
    default:
        return mUnknownMask;
    }
}

Trit get_trit_value(uint tritMask) {
    switch (tritMask) {
    case mFalseMask:
        return Trit::False;
    case mTrueMask:
        return Trit::True;
    default:
        return Trit::Unknown;
    }
}

inline uint get_position_mask(size_type pos) {
    return mPosMask << (pos * TritSetAux::BITS_PER_TRIT);
}
inline uint shift_left(uint mask, size_type posShift) {
    return mask << (posShift * TritSetAux::BITS_PER_TRIT);
}
inline uint shift_right(uint mask, size_type posShift) {
    return mask >> (posShift * TritSetAux::BITS_PER_TRIT);
}

}   // anonymous namespace

namespace TritSetAux {

const uint BITS_PER_TRIT = 2u;
const uint TRITS_PER_ELEM = CHAR_BIT * sizeof(uint) / BITS_PER_TRIT;

Trit get_value(const uint &element, size_type pos) {
    // get bits in the "pos" position
    uint tritMask = element & get_position_mask(pos);
    // shift to the low-order bits
    tritMask = shift_right(tritMask, pos);
    // return the trit value represented by these bits
    return get_trit_value(tritMask);
}

void set_value(Trit value, uint &element, size_type pos) {
    // reset bits in the "pos" position
    element &= ~get_position_mask(pos);
    // set bits representing requested trit value
    uint tritMask = get_trit_mask(value);
    element |= shift_left(tritMask, pos);
}

void set_value(Trit value, uint &element, size_type begPos, size_type endPos) {
    // initialize bit masks
    uint posMask = shift_left(mPosMask, begPos);
    uint tritMask = shift_left(get_trit_mask(value), begPos);
    for (size_type pos = begPos; pos < endPos; ++pos) {
        // set given trit value in the pos
        element &= ~posMask;
        element |= tritMask;
        // shift masks
        posMask = shift_left(posMask, 1);
        tritMask = shift_left(tritMask, 1);
    }
}

}   // namespace TritSetAux


