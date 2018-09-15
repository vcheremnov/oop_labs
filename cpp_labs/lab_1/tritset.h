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
    // reference class declaration
    class reference;
    // constructors
    TritSet(size_type size = 0);
    TritSet(const TritSet &set) = default;
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
    size_type mLength = 0;          // index of the last not-unknown trit + 1
    size_type mCapacity = 0;        // actual size of the tritset
    std::vector<uint> mTritsVec;
    std::unordered_map<Trit, size_type, std::hash<int>> mTritCount;
};

// A proxy class for accessing individual trits in TritSet

class TritSet::reference {
    friend class TritSet;
    reference(uint &element, uint pos):
        mElement(element), mPos(pos) {}
    uint &mElement, mPos;           // mPos - trit position in the mElement (in bits)
    // Masks for every trit value
    static const uint mUnknownMask = 0b00u;
    static const uint mFalseMask = 0b01u;
    static const uint mTrueMask = 0b10u;
    // internal methods
    static uint get_trit_mask(Trit val);
    static Trit get_trit_value(uint tritMask);
    static Trit inverted_trit_value(Trit val);
    uint get_trit_mask_from(const reference &ref) const {
        return ref.mElement & ref.get_position_mask();
    }
    uint get_position_mask() const {
        return 0b11u << mPos;
    }
    void set_trit(uint tritMask) {
        mElement = (mElement && ~get_position_mask()) | (tritMask << mPos);
    }
public:
    reference &operator= (Trit val);
    reference &operator= (const reference &ref);
    Trit operator~ () const;
};

#endif // TRITSET_H
