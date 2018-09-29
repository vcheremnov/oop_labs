#include "trits.h"

Trit operator& (Trit val1, Trit val2) {
    if (val1 == Trit::True) {
        return val2;
    }
    if (val1 == Trit::Unknown) {
        if (val2 == Trit::False) {
            return Trit::False;
        }
        return Trit::Unknown;
    }
    return Trit::False;
}

Trit operator| (Trit val1, Trit val2) {
    if (val1 == Trit::False) {
        return val2;
    }
    if (val1 == Trit::Unknown) {
        if (val2 == Trit::True) {
            return Trit::True;
        }
        return Trit::Unknown;
    }
    return Trit::True;
}

Trit operator~ (Trit val) {
    switch (val) {
    case Trit::False:
        return Trit::True;
    case Trit::True:
        return Trit::False;
    default:
        return Trit::Unknown;
    }
}
