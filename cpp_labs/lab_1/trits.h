#ifndef TRITS_H
#define TRITS_H

#include <cstddef>
#include <functional>

enum class Trit {
    Unknown,
    False,
    True
};

namespace std {

template<> struct hash<Trit> {
    std::size_t operator() (Trit value) const {
        return static_cast<std::size_t>(value);
    }
};

}   // namespace std

Trit operator& (Trit val1, Trit val2);
Trit operator| (Trit val1, Trit val2);
Trit operator~ (Trit val);

#endif // TRITS_H
