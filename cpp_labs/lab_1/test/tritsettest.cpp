#include <exception>
#include <iostream>
#include <sstream>
#include "../tritset.h"
#include "../trit_handler.h"
#include "../reference.h"
#include "../tritwise_operations.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

namespace {
    using TritHandler = TritSet::TritHandler;
    using Reference = TritSet::Reference;
    using size_type = TritSet::size_type;
    using uint = TritSet::uint;
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

// TritSet::TritHandler

TEST(TritHandlerTest, TritHandler_general) {
    // implementation dependent test
    // assumes that trits are stored in TritSet::uint elements, each trit is 2-bit long
    const uint BITS_PER_TRIT = 2u;
    const uint TRITS_PER_INT = CHAR_BIT * sizeof(uint) / BITS_PER_TRIT;
    const size_type maxPos = TRITS_PER_INT;

    uint element = 0;
    // set different values and check them by get_value(..)
    size_type pos1 = 0;
    TritHandler::set_value(Trit::False, element, pos1);
    ASSERT_EQ(TritHandler::get_value(element, pos1), Trit::False);

    size_type pos2 = maxPos / 2;
    TritHandler::set_value(Trit::Unknown, element, pos2);
    ASSERT_EQ(TritHandler::get_value(element, pos2), Trit::Unknown);

    size_type pos3 = maxPos * 3 / 4;
    TritHandler::set_value(Trit::True, element, pos3);
    ASSERT_EQ(TritHandler::get_value(element, pos3), Trit::True);

    // set several values at once & check
    size_type begPos = maxPos / 3, endPos = maxPos;
    TritHandler::set_value(Trit::True, element, begPos, endPos);
    for (size_type pos = begPos; pos < endPos; ++pos ) {
        ASSERT_EQ(TritHandler::get_value(element, pos), Trit::True);
    }
}

// TritSet::Reference

TEST(TritReferenceSet, Reference_value) {
    size_type size = 50;
    TritSet set(size);
    // get reference to the non-existing trit
    size *= 2;
    auto ref = set[size - 1];
    ASSERT_EQ(ref.value(), Trit::Unknown);
    // assign Trit::True to the reference
    ref = Trit::True;
    ASSERT_EQ(set.capacity(), size);
    ASSERT_EQ(ref.value(), Trit::True);
    auto anotherRef = set[size - 1];
    anotherRef = Trit::False;
    ASSERT_EQ(ref.value(), Trit::False);
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
    ASSERT_EQ(set.get_string_repr(), str_repr);
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
    ASSERT_EQ(set.get_string_repr(), str_repr);
}

TEST(TritSetTest, TritSet_shrink) {
    // make a set of size == 1000
    size_type size = 1000;
    TritSet set(size);
    set[size / 2] = Trit::True;
    // shrink the set
    set.shrink();
    ASSERT_EQ(set.capacity(), size / 2 + 1);
    // shrink to the empty size
    set[size / 2] = Trit::Unknown;
    set.shrink();
    ASSERT_EQ(set.capacity(), 0u);
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

TEST(TritSetTest, TritSet_get_str_repr) {
    size_type size = 10;
    TritSet set(size);
    std::string str_repr(10, '?');
    // check initial string representation
    ASSERT_EQ(set.get_string_repr(), str_repr);
    // change trits && check string representation
    set[1] = set[3] = set[5] = set[7] = Trit::False;
    set[2] = set[4] = set[6] = set[8] = Trit::True;
    str_repr = "?01010101?";
    ASSERT_EQ(set.get_string_repr(), str_repr);
    // resize & check the preservation of the previous characters
    size *= 2;
    set.resize(size);
    str_repr.resize(size, '?');
    ASSERT_EQ(set.get_string_repr(), str_repr);
    size /= 4;
    set.resize(size);
    str_repr.resize(size);
    ASSERT_EQ(set.get_string_repr(), str_repr);
    // string representation of the empty set
    set.resize(0);
    ASSERT_EQ(set.get_string_repr(), "");
}

// TritSet public operators

TEST(TritSetTest, TritSet_NOT) {
    size_type size = 50;
    TritSet set(size);
    std::string str_repr = set.get_string_repr();
    // invert and check string representations
    ASSERT_EQ((~set).get_string_repr(), str_repr);
    // set values & invert
    for (size_type ix = 0; ix + 1 < size; ix += 3) {
        set[ix] = Trit::False;
        set[ix + 1] = Trit::True;
        // set opposite values
        str_repr[ix] = '1';
        str_repr[ix + 1] = '0';
    }
    ASSERT_EQ((~set).get_string_repr(), str_repr);
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
    ASSERT_EQ((set1 & set2).get_string_repr(), str_repr);
    ASSERT_EQ((set2 & set1).get_string_repr(), str_repr);
    // test combined assignment
    set1 &= set2;
    ASSERT_EQ(set1.get_string_repr(), str_repr);
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
    ASSERT_EQ((set1 | set2).get_string_repr(), str_repr);
    ASSERT_EQ((set2 | set1).get_string_repr(), str_repr);
    // test combined assignment
    set1 |= set2;
    ASSERT_EQ(set1.get_string_repr(), str_repr);
}

