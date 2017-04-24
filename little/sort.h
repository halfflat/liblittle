#ifndef HF_SMALLSORT_H_
#define HF_SMALLSORT_H_ 1

/// Inline sorting of small, fixed-size
/// random-access sequences.

#include <utility>
#include <type_traits>

#include "comparator.h"

namespace hf {
namespace tiny {

namespace impl {
    template <int m,int n>
    struct S {
        template <typename A, typename C>
        [[gnu::always_inline]] static void run(A &a,C&& c) {
            c(a[m],a[n]);
        }
    };

    // merging networks

    // swap m/2 k-spaced pairs (m0,m0+k),(m0+2k,m0+3k),...
    template <int m,int m0,int k>
    struct pairwise_exchange {
        template <typename A, typename C>
        [[gnu::always_inline]] static void run(A &a,C&& c) {
            S<m0,m0+k>::run(a,c);
            pairwise_exchange<m-2,m0+2*k,k>::run(a,c);
        }
    };

    template <int m0,int k>
    struct pairwise_exchange<1,m0,k> { template <typename A,typename C> static void run(A&,C&&) {} };

    template <int m0,int k>
    struct pairwise_exchange<0,m0,k> { template <typename A,typename C> static void run(A&,C&&) {} };


    // merge subsequences [m0,m0+k,m0+2k,...,m0+(m-1)k] amd
    // [n0,n0+k,n0+2k,...,n0+(n-1)k] of A.
    template <int m,int m0,int n,int n0,int k>
    struct merge_subsequences {
        template <typename A, typename C>
        [[gnu::always_inline]] static void run(A &a,C&& c) {
            // merge 'odd' subsequences
            merge_subsequences<(m+1)/2,m0,(n+1)/2,n0,2*k>::run(a,c);
            // merge 'even' subsequences
            merge_subsequences<m/2,m0+k,n/2,n0+k,2*k>::run(a,c);
            // pair-wise merge sorted subsequences
            pairwise_exchange<m-1,m0+k,k>::run(a,c);

            constexpr int d=(m+1)%2;
            if (d) S<m0+(m-1)*k,n0>::run(a,c);
            pairwise_exchange<n-d,n0+d*k,k>::run(a,c);
        }
    };

    template <int m0,int n,int n0,int k>
    struct merge_subsequences<0,m0,n,n0,k> { template <typename A,typename C> static void run(A &,C&&) {}; };

    template <int m,int m0,int n0,int k>
    struct merge_subsequences<m,m0,0,n0,k> { template <typename A,typename C> static void run(A &,C&&) {}; };

    template <int m0,int n0,int k>
    struct merge_subsequences<1,m0,1,n0,k> {
        template <typename A,typename C>
        [[gnu::always_inline]] static void run(A &a,C&& c) { S<m0,n0>::run(a,c); }
    };

    // minimal networks up to 6, then recusive merge

    template <int n,int n0>
    struct sort {
        template <typename A,typename C>
        [[gnu::always_inline]] static void run(A &a,C&& c) {
            constexpr int r=n/2;
            sort<r,n0>::run(a,c);
            sort<n-r,n0+r>::run(a,c);
            merge_subsequences<r,n0,n-r,n0+r,1>::run(a,c);
        }
    };

    template <int n0>
    struct sort<0,n0> {
        template <typename A,typename C>
        static void run(A &a,C&& c) {}
    };

    template <int n0>
    struct sort<1,n0> {
        template <typename A,typename C>
        static void run(A &a,C&& c) {}
    };

    template <int n0>
    struct sort<2,n0> {
        template <typename A,typename C>
        [[gnu::always_inline]] static void run(A &a,C&& c) { S<n0,n0+1>::run(a,c); }
    };

    template <int n0>
    struct sort<3,n0> {
        template <typename A,typename C>
        [[gnu::always_inline]] static void run(A &a,C&& c) {
            S<n0  ,n0+1>::run(a,c);
            S<n0+1,n0+2>::run(a,c);
            S<n0  ,n0+1>::run(a,c);
        }
    };

    template <int n0>
    struct sort<4,n0> {
        template <typename A,typename C>
        [[gnu::always_inline]] static void run(A &a,C&& c) {
            S<n0  ,n0+1>::run(a,c);
            S<n0+2,n0+3>::run(a,c);
            S<n0  ,n0+2>::run(a,c);
            S<n0+1,n0+3>::run(a,c);
            S<n0+1,n0+2>::run(a,c);
        }
    };

    template <int n0>
    struct sort<5,n0> {
        template <typename A,typename C>
        [[gnu::always_inline]] static void run(A &a,C&& c) {
            S<n0  ,n0+1>::run(a,c);
            S<n0+2,n0+4>::run(a,c);
            S<n0  ,n0+3>::run(a,c);
            S<n0+1,n0+4>::run(a,c);
            S<n0+1,n0+2>::run(a,c);
            S<n0+3,n0+4>::run(a,c);
            S<n0  ,n0+1>::run(a,c);
            S<n0+2,n0+3>::run(a,c);
            S<n0+1,n0+2>::run(a,c);
        }
    };

    template <int n0>
    struct sort<6,n0> {
        template <typename A,typename C>
        [[gnu::always_inline]] static void run(A &a,C&& c) {
            S<n0  ,n0+1>::run(a,c);
            S<n0+2,n0+3>::run(a,c);
            S<n0+4,n0+5>::run(a,c);
            S<n0  ,n0+2>::run(a,c);
            S<n0+1,n0+4>::run(a,c);
            S<n0+3,n0+5>::run(a,c);
            S<n0  ,n0+1>::run(a,c);
            S<n0+2,n0+3>::run(a,c);
            S<n0+4,n0+5>::run(a,c);
            S<n0+1,n0+2>::run(a,c);
            S<n0+3,n0+4>::run(a,c);
            S<n0+2,n0+3>::run(a,c);
        }
    };

    template <typename A>
    using value_type_t = typename std::decay<decltype(std::declval<A>()[0])>::type;
} // namespace impl

template <int n,typename A,typename Comparator=::hf::comparator<impl::value_type_t<A>>>
[[gnu::always_inline]] inline void sort(A &a, Comparator&& comparator = Comparator{}) {
    impl::sort<n,0>::run(a, comparator);
}

} // namespace tiny
} // namespace hf

#endif // ndef HF_SMALLSORT_H_
