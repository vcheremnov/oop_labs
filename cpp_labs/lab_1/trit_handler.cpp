#include "trit_handler.h"

namespace {
    using TritHandler = TritSet::TritHandler;
    using size_type = TritSet::size_type;
    using uint = TritSet::uint;
}

Trit TritHandler::get_value(const uint &element, size_type pos) {
    uint tritMask = element & get_position_mask(pos);
    tritMask = shift_right(tritMask, pos);
    return get_trit_value(tritMask);
}

void TritHandler::set_value(Trit value, uint &element, size_type pos) {
    // disable bits in trit's position
    element &= ~get_position_mask(pos);
    // set bits representing requested trit value
    uint tritMask = get_trit_mask(value);
    element |= shift_left(tritMask, pos);
}

void TritHandler::set_value(Trit value, uint &element, size_type begPos, size_type endPos) {
    uint posMask = shift_left(mPosMask, begPos);
    uint tritMask = shift_left(get_trit_mask(value), begPos);
    for (size_type pos = begPos; pos < endPos; ++pos) {
        element &= ~posMask;
        element |= tritMask;
        posMask = shift_left(posMask, 1);
        tritMask = shift_left(tritMask, 1);
    }
}

uint TritHandler::get_trit_mask(Trit val) {
    switch (val) {
    case Trit::False:
        return mFalseMask;
    case Trit::True:
        return mTrueMask;
    default:
        return mUnknownMask;
    }
}

Trit TritHandler::get_trit_value(uint tritMask) {
    switch (tritMask) {
    case mFalseMask:
        return Trit::False;
    case mTrueMask:
        return Trit::True;
    default:
        return Trit::Unknown;
    }
}
