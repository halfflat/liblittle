#include "little/compat.h"

#include <type_traits>
#include <cstdint>
#include <string>
#include <gtest/gtest.h>

#include "little/comparator.h"

template <typename T>
struct comparator: public ::testing::Test {
    using type = T;
    static constexpr bool is_signed = std::is_signed<T>::value;

    static type a() { return is_signed?-1:0; }
    static type b() { return is_signed?0:1; }
    static type c() { return is_signed?1:2; }

    using comparator_type = hf::comparator<type>;
};

template <>
struct comparator<std::string>: public ::testing::Test {
    using type = std::string;

    static type a() { return ""; }
    static type b() { return "abc"; }
    static type c() { return "abcd"; }

    using comparator_type = hf::comparator<type>;
};

typedef ::testing::Types<
    unsigned char, unsigned short, unsigned, unsigned long, unsigned long long,
    signed char, short, int, long, long long,
    char, float, double, std::string
> test_comp_types;

TYPED_TEST_CASE(comparator, test_comp_types);

TYPED_TEST(comparator, test) {
    typename TestFixture::comparator_type comp;

    // know a < b < c
    auto a = TestFixture::a();
    auto b = TestFixture::b();
    auto c = TestFixture::c();

    auto x = a;
    auto y = b;
    comp(x, y);
    EXPECT_EQ(a, x);
    EXPECT_EQ(b, y);

    x = b;
    y = a;
    comp(x, y);
    EXPECT_EQ(a, x);
    EXPECT_EQ(b, y);

    x = b;
    y = c;
    comp(x, y);
    EXPECT_EQ(b, x);
    EXPECT_EQ(c, y);

    x = c;
    y = b;
    comp(x, y);
    EXPECT_EQ(b, x);
    EXPECT_EQ(c, y);

    x = a;
    y = c;
    comp(x, y);
    EXPECT_EQ(a, x);
    EXPECT_EQ(c, y);

    x = c;
    y = a;
    comp(x, y);
    EXPECT_EQ(a, x);
    EXPECT_EQ(c, y);
};

