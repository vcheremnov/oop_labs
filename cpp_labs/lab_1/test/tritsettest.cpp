#include <exception>
#include <iostream>
#include <sstream>
#include "tritsettest.h"
#include "../tritset.h"
#include "../reference.h"
#include "../tritwise_operations.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

namespace {
    using reference = TritSet::Reference;
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

// TritHandler

// TritSet public methods

TEST(TritSetTest, TritSet_capacity) {
    // make a set of size == 1000
    size_type size = 1000;
    TritSet set(size);
    ASSERT_EQ(set.capacity(), size);
    // doesn't change the capacity
    size_type bigIndex = 10000000u;
    size_type capacity = set.capacity();
    set[bigIndex] = Trit::Unknown;
    ASSERT_EQ(capacity, set.capacity());
    // false, but no exception or memory allocation
    bigIndex *= 2;
    if (set[bigIndex] == Trit::True) {}
    ASSERT_EQ(set.capacity(), capacity);
    // memory allocation
    bigIndex = size * 10;
    set[bigIndex] = Trit::False;
    ASSERT_LT(capacity, set.capacity());
    // no memory operations
    capacity = set.capacity();
    set[bigIndex] = Trit::Unknown;
    set[bigIndex / 5] = Trit::True;
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
