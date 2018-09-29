#ifndef TRITSET_H
#define TRITSET_H

#include <cstddef>
#include <climits>
#include <vector>
#include <iostream>
#include <unordered_map>
#include "trits.h"

class TritSet {
public:
    // class types
    using uint = unsigned int;
    using size_type = std::size_t;
    // reference to the trit
    class Reference;
    // constructors
    TritSet(size_type size = 0);
    // accessors
    size_type capacity() const {
        return mCapacity;
    }
    size_type length() const;
    size_type cardinality(Trit value) const;
    std::unordered_map<Trit, size_type, TritHash> cardinality() const;
    // string representation
    std::string get_string_repr() const;
    // modifiers
    void shrink() {
        resize(length());
    }
    void resize(size_type size);
    void trim(size_type lastIndex);
    // assignment
    TritSet &operator&= (const TritSet &set) {
        return *this = (*this & set);
    }
    TritSet &operator|= (const TritSet &set) {
        return *this = (*this | set);
    }
    // trit access
    Trit operator[] (size_type tritIndex) const {
        return get_value_at(tritIndex);
    }
    Reference operator[] (size_type tritIndex);
    // tritwise operations
    TritSet operator~ () const;
    TritSet operator| (const TritSet &set) const;
    TritSet operator& (const TritSet &set) const;
private:
    size_type mCapacity = 0;                                        // actual size of the tritset
    std::vector<uint> mStorage;                                     // storage for the trits
    // private methods
    uint *get_elem_address(size_type tritIndex);
    const uint *get_elem_address(size_type tritIndex) const;
    Trit get_value_at(size_type tritIndex) const;
};

std::ostream &operator<< (std::ostream &os, const TritSet &set);

#endif // TRITSET_H
