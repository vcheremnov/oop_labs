#include <exception>
#include <iostream>
#include <sstream>
#include "../tritset.h"
#include "../tritset_aux.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

// string representation & output

std::ostream &operator<< (std::ostream &os, const TritSet &set) {
    for (size_type ix = 0; ix < set.capacity(); ++ix) {
        switch (set[ix]) {
        case Trit::False:
            os << '0';
            break;
        case Trit::True:
            os << '1';
            break;
        default:
            os << '?';
        }
    }
    return os;
}

std::string get_string_repr(const TritSet &set) {
    std::stringstream ss;
    ss << set;
    return ss.str();
}

// Tritwise operations

TEST(TritOperationsTest, TritNot) {
    EXPECT_EQ(~Trit::False,     Trit::True);
    EXPECT_EQ(~Trit::Unknown,   Trit::Unknown);
    EXPECT_EQ(~Trit::True,      Trit::False);
}

TEST(TritOperationsTest, TritAnd) {
    EXPECT_EQ(Trit::False & Trit::False,        Trit::False);
    EXPECT_EQ(Trit::False & Trit::Unknown,      Trit::False);
    EXPECT_EQ(Trit::False & Trit::True,         Trit::False);
    EXPECT_EQ(Trit::Unknown & Trit::False,      Trit::False);
    EXPECT_EQ(Trit::Unknown & Trit::Unknown,    Trit::Unknown);
    EXPECT_EQ(Trit::Unknown & Trit::True,       Trit::Unknown);
    EXPECT_EQ(Trit::True & Trit::False,         Trit::False);
    EXPECT_EQ(Trit::True & Trit::Unknown,       Trit::Unknown);
    EXPECT_EQ(Trit::True & Trit::True,          Trit::True);
}

TEST(TritOperationsTest, TritOr) {
    EXPECT_EQ(Trit::False | Trit::False,        Trit::False);
    EXPECT_EQ(Trit::False | Trit::Unknown,      Trit::Unknown);
    EXPECT_EQ(Trit::False | Trit::True,         Trit::True);
    EXPECT_EQ(Trit::Unknown | Trit::False,      Trit::Unknown);
    EXPECT_EQ(Trit::Unknown | Trit::Unknown,    Trit::Unknown);
    EXPECT_EQ(Trit::Unknown | Trit::True,       Trit::True);
    EXPECT_EQ(Trit::True | Trit::False,         Trit::True);
    EXPECT_EQ(Trit::True | Trit::Unknown,       Trit::True);
    EXPECT_EQ(Trit::True | Trit::True,          Trit::True);
}

// TritSetAux

TEST(TritSetAux, TritSetAux_general) {
    const size_type maxPos = TritSetAux::TRITS_PER_ELEM;

    uint element = 0;
    // set different values and check them by get_value(..)
    size_type pos1 = 0;
    TritSetAux::set_value(Trit::False, element, pos1);
    ASSERT_EQ(TritSetAux::get_value(element, pos1), Trit::False);

    size_type pos2 = maxPos / 2;
    TritSetAux::set_value(Trit::Unknown, element, pos2);
    ASSERT_EQ(TritSetAux::get_value(element, pos2), Trit::Unknown);

    size_type pos3 = maxPos * 3 / 4;
    TritSetAux::set_value(Trit::True, element, pos3);
    ASSERT_EQ(TritSetAux::get_value(element, pos3), Trit::True);

    // rewrite trit value at the same position & check
    TritSetAux::set_value(Trit::False, element, pos3);
    ASSERT_EQ(TritSetAux::get_value(element, pos3), Trit::False);

    // set several values at once & check
    size_type begPos = maxPos / 3;
    TritSetAux::set_value(Trit::True, element, begPos, maxPos);
    for (size_type pos = begPos; pos < maxPos; ++pos) {
        ASSERT_EQ(TritSetAux::get_value(element, pos), Trit::True);
    }
}

// TritSet::Reference

TEST(TritReferenceSet, Reference_value) {
    size_type size = 50;
    TritSet set(size);
    // get reference to the non-existing trit
    size *= 2;
    auto ref = set[size - 1];
    ASSERT_EQ(ref, Trit::Unknown);
    // assign Trit::True to the reference
    ref = Trit::True;
    ASSERT_EQ(set.capacity(), size);
    ASSERT_EQ(ref, Trit::True);
    auto anotherRef = set[size - 1];
    anotherRef = Trit::False;
    ASSERT_EQ(ref, Trit::False);
}

TEST(TritReferenceSet, Reference_AND_assignment) {
    size_type size = 50;
    TritSet set(size);
    // set values through the references (set[ix] returns a reference to the ix-th trit)
    set[0] = set[1] = set[2] = Trit::False;
    set[3] = set[4] = set[5] = Trit::Unknown;
    set[6] = set[7] = set[8] = Trit::True;
    // &= test
    set[0] &= Trit::False; set[1] &= Trit::Unknown;  set[2] &= Trit::True;
    set[3] &= Trit::False; set[4] &= Trit::Unknown;  set[5] &= Trit::True;
    set[6] &= Trit::False; set[7] &= Trit::Unknown;  set[8] &= Trit::True;
    // check the results
    ASSERT_EQ(set[0], Trit::False);
    ASSERT_EQ(set[1], Trit::False);
    ASSERT_EQ(set[2], Trit::False);
    ASSERT_EQ(set[3], Trit::False);
    ASSERT_EQ(set[4], Trit::Unknown);
    ASSERT_EQ(set[5], Trit::Unknown);
    ASSERT_EQ(set[6], Trit::False);
    ASSERT_EQ(set[7], Trit::Unknown);
    ASSERT_EQ(set[8], Trit::True);
}

TEST(TritReferenceSet, Reference_OR_assignment) {
    size_type size = 50;
    TritSet set(size);
    // set values through the references (set[ix] returns a reference to the ix-th trit)
    set[0] = set[1] = set[2] = Trit::False;
    set[3] = set[4] = set[5] = Trit::Unknown;
    set[6] = set[7] = set[8] = Trit::True;
    // &= test
    set[0] |= Trit::False; set[1] |= Trit::Unknown;  set[2] |= Trit::True;
    set[3] |= Trit::False; set[4] |= Trit::Unknown;  set[5] |= Trit::True;
    set[6] |= Trit::False; set[7] |= Trit::Unknown;  set[8] |= Trit::True;
    // check the results
    ASSERT_EQ(set[0], Trit::False);
    ASSERT_EQ(set[1], Trit::Unknown);
    ASSERT_EQ(set[2], Trit::True);
    ASSERT_EQ(set[3], Trit::Unknown);
    ASSERT_EQ(set[4], Trit::Unknown);
    ASSERT_EQ(set[5], Trit::True);
    ASSERT_EQ(set[6], Trit::True);
    ASSERT_EQ(set[7], Trit::True);
    ASSERT_EQ(set[8], Trit::True);
}

// TritSet public methods

TEST(TritSetTest, TritSet_subscript) {
    // make a set of size == 1000
    size_type size = 1000;
    TritSet set(size);
    ASSERT_EQ(set.capacity(), size);

    // doesn't change the capacity
    size_type bigIndex = 10000000u;
    size_type capacity = set.capacity();
    set[bigIndex] = Trit::Unknown;
    ASSERT_EQ(capacity, set.capacity());
    ASSERT_EQ(set[bigIndex], Trit::Unknown);

    // false, but no exception or memory allocation
    bigIndex *= 2;
    if (set[bigIndex] == Trit::True) {}
    ASSERT_EQ(set.capacity(), capacity);

    // memory allocation
    bigIndex = size * 10;
    set[bigIndex] = Trit::False;
    ASSERT_EQ(set[bigIndex], Trit::False);
    ASSERT_LT(capacity, set.capacity());

    // no memory operations
    capacity = set.capacity();
    set[bigIndex] = Trit::Unknown;
    set[bigIndex / 5] = Trit::True;
    ASSERT_EQ(set[bigIndex], Trit::Unknown);
    ASSERT_EQ(set[bigIndex / 5], Trit::True);
    ASSERT_EQ(capacity, set.capacity());
}

TEST(TritSetTest, TritSet_resize) {
    // make a set of size == 10
    size_type size = 10;
    TritSet set(size);

    // resize & check the capacity
    size = 100;
    set.resize(size);
    ASSERT_EQ(set.capacity(), size);

    // check whether the added trits are filled with "Unknown" values
    std::string str_repr(size, '?');
    ASSERT_EQ(get_string_repr(set), str_repr);

    // set some values
    for (size_type ix = 0; ix + 1 < size; ix += 2) {
        set[ix] = Trit::False;
        set[ix + 1] = Trit::True;
        str_repr[ix] = '0';
        str_repr[ix + 1] = '1';
    }
    // resize to remove half of them
    size /= 2;
    set.resize(size);
    str_repr.resize(size);
    ASSERT_EQ(set.capacity(), size);
    // resize back
    set.resize(size * 2);
    str_repr.resize(size * 2, '?');
    // check whether the first half of values is saved, and another is not
    ASSERT_EQ(get_string_repr(set), str_repr);

    // check the cleaning within the last storage element after resizing
    set.resize(TritSetAux::TRITS_PER_ELEM);
    str_repr = get_string_repr(set);
    for (size_type ix = 0; ix < set.capacity(); ++ix) {
        set[ix] = Trit::True;
        str_repr[ix] = '1';
    }
    set.resize(TritSetAux::TRITS_PER_ELEM / 2);
    str_repr.resize(set.capacity());
    set.resize(TritSetAux::TRITS_PER_ELEM);
    str_repr.resize(set.capacity(), '?');
    ASSERT_EQ(get_string_repr(set), str_repr);
}

TEST(TritSetTest, TritSet_length) {
    size_type size = 100;
    TritSet set(size);

    // check if initial length equals 0
    ASSERT_EQ(set.length(), 0u);

    // change logical length
    size_type pos1 = size / 2;
    set[pos1] = Trit::True;
    ASSERT_EQ(set.length(), pos1 + 1);

    size_type pos2 = 3 * size / 4;
    set[pos2] = Trit::False;
    ASSERT_EQ(set.length(), pos2 + 1);

    // following assignments doesn't change the logical length
    size_type pos3 = pos1 / 2;
    set[pos3] = Trit::False;
    ASSERT_EQ(set.length(), pos2 + 1);

    set[pos2 + 1] = Trit::Unknown;
    ASSERT_EQ(set.length(), pos2 + 1);

    // reset the last non-unknown value => length changes
    set[pos2] = Trit::Unknown;
    ASSERT_EQ(set.length(), pos1 + 1);

    // resize to the size less than current length (and more than pos3 + 1)
    set.resize((pos1 + pos3) / 2);
    ASSERT_EQ(set.length(), pos3 + 1);
}

TEST(TritSetTest, TritSet_shrink) {
    // make a set of size == 1000
    size_type size = 1000;
    TritSet set(size);

    // change the logical length
    set[size / 2] = Trit::True;

    // shrink the set to the logical length
    set.shrink();
    ASSERT_EQ(set.capacity(), size / 2 + 1);
    ASSERT_EQ(set.length(), size / 2 + 1);

    // shrink to the empty size
    set[size / 2] = Trit::Unknown;
    set.shrink();
    ASSERT_EQ(set.capacity(), 0u);
    ASSERT_EQ(set.length(), 0u);
}

TEST(TritSetTest, TritSet_cardinality) {
    size_type size = 100;
    TritSet set(size);

    // check initial cardinalities
    ASSERT_EQ(set.cardinality(Trit::True), 0u);
    ASSERT_EQ(set.cardinality(Trit::False), 0u);
    ASSERT_EQ(set.cardinality(Trit::Unknown), 0u);
    auto cardMap = set.cardinality();
    ASSERT_EQ(cardMap[Trit::True], 0u);
    ASSERT_EQ(cardMap[Trit::False], 0u);
    ASSERT_EQ(cardMap[Trit::Unknown], 0u);

    // fill the set with values & check cardinalities
    size_type cnt = 0;
    for (size_type ix = 0; ix + 2 < set.capacity() / 2; ix += 3) {
        set[ix] = Trit::True;
        set[ix + 1] = Trit::Unknown;
        set[ix + 2] = Trit::False;
        ++cnt;
    }
    ASSERT_EQ(set.cardinality(Trit::True), cnt);
    ASSERT_EQ(set.cardinality(Trit::False), cnt);
    ASSERT_EQ(set.cardinality(Trit::Unknown), cnt);
    cardMap = set.cardinality();
    ASSERT_EQ(cardMap[Trit::True], cnt);
    ASSERT_EQ(cardMap[Trit::False], cnt);
    ASSERT_EQ(cardMap[Trit::Unknown], cnt);

    // change logical length & check cardinalities
    set = TritSet(size);
    size_type trueIndex = size / 2;
    set[trueIndex] = Trit::True;
    ASSERT_EQ(set.cardinality(Trit::True), 1u);
    ASSERT_EQ(set.cardinality(Trit::Unknown), trueIndex);

    set[trueIndex] = Trit::Unknown;
    ASSERT_EQ(set.cardinality(Trit::True), 0u);
    ASSERT_EQ(set.cardinality(Trit::Unknown), 0u);
}

TEST(TritSetTest, TritSet_trim) {
    size_type size = 100;
    TritSet set(size);
    std::string str_repr(size, '?');

    // fill the tritset
    for (size_type ix = 0; ix + 1 < set.capacity(); ix += 2) {
        set[ix] = Trit::True;
        set[ix + 1] = Trit::False;
        str_repr[ix] = '1';
        str_repr[ix + 1] = '0';
    }

    // trim & check values and logical length
    size_type lastIndex = size / 2;
    set.trim(lastIndex);
    std::fill(str_repr.begin() + lastIndex, str_repr.end(), '?');
    ASSERT_EQ(set.length(), lastIndex);
    ASSERT_EQ(get_string_repr(set), str_repr);
}

TEST(TritSetTest, TritSet_const_subscript) {
    size_type size = 50;
    TritSet set(size);
    for (size_type ix = 0; ix + 2 < size; ix += 3) {
        set[ix] = Trit::False;
        set[ix + 1] = Trit::Unknown;
        set[ix + 2] = Trit::True;
    }

    // make const TritSet
    const TritSet constSet(set);

    // check the identity of set & constSet using the const subcript operator
    for (size_type ix = 0; ix < size; ++ix) {
        ASSERT_EQ(set[ix], constSet[ix]);
    }
}

TEST(Test, TritSet_get_str_repr) {
    size_type size = 10;
    TritSet set(size);
    std::string str_repr(10, '?');

    // check initial string representation
    ASSERT_EQ(get_string_repr(set), str_repr);

    // change trits && check string representation
    set[1] = set[3] = set[5] = set[7] = Trit::False;
    set[2] = set[4] = set[6] = set[8] = Trit::True;
    str_repr = "?01010101?";
    ASSERT_EQ(get_string_repr(set), str_repr);

    // resize & check the preservation of the previous characters
    size *= 2;
    set.resize(size);
    str_repr.resize(size, '?');
    ASSERT_EQ(get_string_repr(set), str_repr);
    size /= 4;
    set.resize(size);
    str_repr.resize(size);
    ASSERT_EQ(get_string_repr(set), str_repr);

    // string representation of the empty set
    set.resize(0);
    ASSERT_EQ(get_string_repr(set), "");
}

// TritSet public operators

TEST(TritSetTest, TritSet_NOT) {
    size_type size = 50;
    TritSet set(size);
    std::string str_repr = get_string_repr(set);

    // invert and check string representations
    ASSERT_EQ(get_string_repr(~set), str_repr);

    // set values & invert
    for (size_type ix = 0; ix + 1 < size; ix += 3) {
        set[ix] = Trit::False;
        set[ix + 1] = Trit::True;
        // set opposite values
        str_repr[ix] = '1';
        str_repr[ix + 1] = '0';
    }
    ASSERT_EQ(get_string_repr(~set), str_repr);
}

TEST(TritSetTest, TritSet_AND) {
    size_type size1 = 50, size2 = 100;
    TritSet set1(size1), set2(size2);

    // check capacity of the result
    ASSERT_EQ((set1 & set2).capacity(), std::max(size1, size2));
    ASSERT_EQ((set2 & set1).capacity(), std::max(size1, size2));

    // check the whole truth table
    set1[0] = set1[1] = set1[2] = Trit::False;
    set1[3] = set1[4] = set1[5] = Trit::Unknown;
    set1[6] = set1[7] = set1[8] = Trit::True;

    set2[0] = set2[3] = set2[6] = Trit::False;
    set2[1] = set2[4] = set2[7] = Trit::Unknown;
    set2[2] = set2[5] = set2[8] = Trit::True;

    std::string str_repr("0000??0?1");
    str_repr.resize(std::max(size1, size2), '?');
    ASSERT_EQ(get_string_repr(set1 & set2), str_repr);
    ASSERT_EQ(get_string_repr(set2 & set1), str_repr);

    // test combined assignment
    set1 &= set2;
    ASSERT_EQ(get_string_repr(set1), str_repr);
}

TEST(TritSetTest, TritSet_OR) {
    size_type size1 = 50, size2 = 100;
    TritSet set1(size1), set2(size2);

    // check capacity of the result
    ASSERT_EQ((set1 & set2).capacity(), std::max(size1, size2));
    ASSERT_EQ((set2 & set1).capacity(), std::max(size1, size2));

    // check the whole truth table
    set1[0] = set1[1] = set1[2] = Trit::False;
    set1[3] = set1[4] = set1[5] = Trit::Unknown;
    set1[6] = set1[7] = set1[8] = Trit::True;

    set2[0] = set2[3] = set2[6] = Trit::False;
    set2[1] = set2[4] = set2[7] = Trit::Unknown;
    set2[2] = set2[5] = set2[8] = Trit::True;

    std::string str_repr("0?1??1111");
    str_repr.resize(std::max(size1, size2), '?');
    ASSERT_EQ(get_string_repr(set1 | set2), str_repr);
    ASSERT_EQ(get_string_repr(set2 | set1), str_repr);

    // test combined assignment
    set1 |= set2;
    ASSERT_EQ(get_string_repr(set1), str_repr);
}

