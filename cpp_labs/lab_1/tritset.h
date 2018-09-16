#ifndef TRITSET_H
#define TRITSET_H

#include <unordered_map>
#include <cstddef>
#include <climits>
#include <vector>

enum class Trit {
    Unknown,
    False,
    True
};

class TritSet {
public:
    // class types
    using uint = unsigned int;
    using size_type = std::size_t;
    // reference to the trit
    class reference;
    friend class reference;
    // constructors
    TritSet(size_type size = 0);
    // accessors
    size_type capacity() const {
        return mCapacity;
    }
    // mutators
    void resize(size_type size);
    void shrink();
    // assignment
    TritSet &operator&= (const TritSet &set) {
        return *this = (*this & set);
    }
    TritSet &operator|= (const TritSet &set) {
        return *this = (*this | set);
    }
    // trit access
    Trit operator[] (size_type tritIndex) const;
    reference operator[] (size_type tritIndex);
    // tritwise operations
    TritSet operator~ () const;
    TritSet operator| (const TritSet &set) const;
    TritSet operator& (const TritSet &set) const;
private:
    size_type mCapacity = 0;                                        // actual size of the tritset
    std::vector<uint> mTritsVec;                                    // storage for the trits
    // static variables
    static const uint BITS_PER_TRIT = 2u;
    static const uint TRITS_PER_INT = CHAR_BIT * sizeof(uint) / BITS_PER_TRIT;
    // private static methods
    static size_type get_storage_length(size_type tritsNum) {
        return tritsNum / TRITS_PER_INT + (tritsNum % TRITS_PER_INT != 0);
    }
    static size_type get_element_index(size_type tritIndex) {
        return tritIndex / TRITS_PER_INT;
    }
    static size_type get_trit_position(size_type tritIndex) {
        return tritIndex % TRITS_PER_INT * BITS_PER_TRIT;
    }
};

#endif // TRITSET_H
