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
    size_type capacity() const;
    size_type length() const;
    size_type cardinality(Trit value) const;
    std::unordered_map<Trit, size_type, TritHash> cardinality() const;
    // modifiers
    void shrink();
    void resize(size_type size);
    void trim(size_type lastIndex);
    // assignment
    TritSet &operator&= (const TritSet &set);
    TritSet &operator|= (const TritSet &set);
    // trit access
    Trit operator[] (size_type tritIndex) const;
    Reference operator[] (size_type tritIndex);
    // tritwise operations
    TritSet operator~ () const;
    TritSet operator| (const TritSet &set) const;
    TritSet operator& (const TritSet &set) const;
private:
    // private methods
    void _update_counters(Trit setValue, Trit oldValue);            // update trit counters after change of a trit value
    void _update_length(Trit setValue, size_type setIndex);         // update logical length after change of a trit value
    size_type _find_length(size_type evalLength) const;             // find current logical length
    Trit _get_value_at(size_type tritIndex) const;                  // get trit value directly from a storage element
    void _set_value_at(size_type tritIndex, Trit value);            // set trit value directly in a storage element
    // internal data
    size_type _length = 0;                                          // logical length of the storage
    size_type _capacity = 0;                                        // actual size of the tritset
    std::vector<uint> _storage;                                     // storage for trits
    size_type _falseCount = 0, _trueCount = 0;                      // number of false/true trits
};

class TritSet::Reference {
    friend class TritSet;
public:
    operator Trit() const;
    Reference &operator= (Trit value);
    Reference &operator= (const Reference &ref);
    Reference &operator|= (Trit value);
    Reference &operator&= (Trit value);
private:
    Reference(TritSet &set, size_type tritIndex);
    Trit _value() const;                            // value of the reference
    // internal data
    TritSet &_set;                                  // reference to the associated tritset
    size_type _tritIndex;                           // trit's index in the tritset
};

#endif // TRITSET_H
