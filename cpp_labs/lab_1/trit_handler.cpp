#include "trit_handler.h"

namespace {
    using TritHandler = TritSet::TritHandler;
    using size_type = TritSet::size_type;
    using uint = TritSet::uint;
}

Trit TritHandler::get_value(const uint &element, size_type tritPos) {
    uint tritMask = element & get_position_mask(tritPos);
    tritMask >>= tritPos * BITS_PER_TRIT;
    return get_trit_value(tritMask);
}

void TritHandler::set_value(Trit value, uint &element, size_type tritPos) {
    // disable bits in trit's position
    element &= ~get_position_mask(tritPos);
    // set bits representing requested trit value
    element |= get_trit_mask(value) << (tritPos * BITS_PER_TRIT);
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
