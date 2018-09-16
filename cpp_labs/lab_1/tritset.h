#ifndef TRITSET_H
#define TRITSET_H

#include <unordered_map>
#include <cstddef>
#include <vector>

enum Trit {
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
    std::unordered_map<Trit, size_type, std::hash<int>> cardinality() const {
        return mTritCount;
    }
    size_type cardinality(Trit value) const {
        return mTritCount.at(value);
    }
    size_type capacity() const {
        return mCapacity;
    }
    size_type length() const {
        return mLength;
    }
    // mutators
    void resize(size_type size);
    void shrink();
    void trim(size_type lastIndex);
    // assignment
    TritSet &operator&= (const TritSet &set) {
        return *this = (*this & set);
    }
    TritSet &operator|= (const TritSet &set) {
        return *this = (*this | set);
    }
    // trit access
    Trit operator[] (size_type index) const;
    reference operator[] (size_type index);
    // tritwise operations
    TritSet operator~ () const;
    TritSet operator| (const TritSet &set) const;
    TritSet operator& (const TritSet &set) const;
private:
    // member variables
    size_type mLength = 0;                                          // index of the last not-unknown trit + 1
    size_type mCapacity = 0;                                        // actual size of the tritset
    std::vector<uint> mTritsVec;                                    // storage for the trits
    std::unordered_map<Trit, size_type, std::hash<int>> mTritCount; // trits occurence counter
};

#endif // TRITSET_H
