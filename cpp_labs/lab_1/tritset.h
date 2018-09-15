#ifndef TRITSET_H
#define TRITSET_H

#include <unordered_map>
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
    using size_type = std::vector<uint>::size_type;
    // trit reference
    class reference;
    // constructors
    TritSet(size_type size = 0);
    TritSet(const TritSet &set) = default;
    // accessors
    std::unordered_map<Trit, size_type, std::hash<int>> cardinality() const;
    size_type cardinality(Trit value) const;
    size_type capacity() const { return mCapacity; }
    size_type length() const;
    // mutators
    void shrink();
    void trim(size_type lastIndex);
    // assignment
    TritSet &operator= (const TritSet &set) = default;
    TritSet &operator&= (const TritSet &set);
    TritSet &operator|= (const TritSet &set);
    // trit access
    Trit operator[] (size_type index) const;
    reference operator[] (size_type index);
    // ternary operations
    TritSet operator~ () const;
    TritSet operator| (const TritSet &set) const;
    TritSet operator& (const TritSet &set) const;
private:
    // member variables
    size_type mCapacity = 0;
    std::vector<uint> mTrits;
    // Masks for every trit value
    static const uint mUnknownMask = 0u;
    static const uint mFalseMask = 1u;
    static const uint mTrueMask = 2u;
    std::unordered_map<Trit, size_type, std::hash<int>> mTritCount;
};

// A proxy class for accessing individual trits in TritSet

class TritSet::reference {
    friend class TritSet;
    reference(uint &element, uint pos):
        mElement(element), mPos(pos) {}
    uint &mElement, mPos;
public:
    reference &operator[] (Trit val);
    reference &operator[] (const reference &val);
    Trit operator~ ();
};

#endif // TRITSET_H
