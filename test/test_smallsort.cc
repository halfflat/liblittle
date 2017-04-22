#include <type_traits>
#include <cstdint>
#include <numeric>
#include <gtest/gtest.h>

#include "smallsort.h"

template <typename T>
struct smallsort: public ::testing::Test {
    static constexpr int n=T::value;

    static_assert(n<32,"n too large to test");
};

typedef ::testing::Types<
    std::integral_constant<int,0>,
    std::integral_constant<int,1>,
    std::integral_constant<int,2>,
    std::integral_constant<int,3>,
    std::integral_constant<int,4>,
    std::integral_constant<int,5>,
    std::integral_constant<int,6>,
    std::integral_constant<int,7>,
    std::integral_constant<int,8>,
    std::integral_constant<int,9>,
    std::integral_constant<int,10>,
    std::integral_constant<int,11>,
    std::integral_constant<int,12>,
    std::integral_constant<int,13>,
    std::integral_constant<int,14>,
    std::integral_constant<int,15>,
    std::integral_constant<int,16>,
    std::integral_constant<int,17>,
    std::integral_constant<int,18>,
    std::integral_constant<int,19>,
    std::integral_constant<int,20>,
    std::integral_constant<int,21>,
    std::integral_constant<int,22>,
    std::integral_constant<int,23>,
    std::integral_constant<int,24>,
    std::integral_constant<int,25>
> test_ints_0_25;

TYPED_TEST_CASE(smallsort,test_ints_0_25);

TYPED_TEST(smallsort,binary_sort) {
    constexpr int n=TestFixture::n;
    uint_fast32_t xmax=(1ul<<n)-1;
    std::array<int,n> a;

    for (uint_fast32_t x=0;x<=xmax;++x) {
        uint_fast32_t mask=1u;
        int c=0;
        for (int i=0;i<n;++i) {
            a[i]=!!(x&mask);
            c+=a[i];
            mask<<=1;
        }

        hf::smallsort_inplace<n>(a);
        auto a0=a.begin();
        auto a_end=a.end();
        ASSERT_EQ(0,std::accumulate(a0,a0+(n-c),0));
        ASSERT_EQ(c,std::accumulate(a0+(n-c),a_end,0));
    }
};

