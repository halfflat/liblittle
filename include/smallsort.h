#ifndef HF_SMALLSORT_H_
#define HF_SMALLSORT_H_ 1

/// Inline sorting of small, fixed-size
/// random-access sequences.

#include <utility>
#include <algorithm>
#include <type_traits>
#include <iostream>

namespace hf {

namespace impl {
    struct reorder {
        template <typename V>
        void operator()(V& a, V& b) const {
            (*this)(a, b, typename std::is_arithmetic<typename std::decay<V>::type>::type{});
        }

    protected:
        template <typename V>
        void operator()(V& a,V& b,std::true_type) const {
            V l = a<b? a: b;
            b = a<b? b: a;
            a = std::move(l);
        }

        template <typename V>
        void operator()(V& a,V& b,std::false_type) const {
            if (!(a<b)) std::swap(a,b);
        }
    };

    template <int m,int n,typename Reorder>
    struct S {
        template <typename A>
        [[gnu::always_inline]] static void run(A &a) {
            Reorder{}(a[m],a[n]);
        }
    };

    // merging networks

    // swap m/2 k-spaced pairs (m0,m0+k),(m0+2k,m0+3k),...
    template <int m,int m0,int k,typename R,typename A>
    struct pairwise_exchange {
        [[gnu::always_inline]] static void run(A &a) {
            S<m0,m0+k,R>::run(a);
            pairwise_exchange<m-2,m0+2*k,k,R,A>::run(a);
        }
    };

    template <int m0,int k,typename R,typename A>
    struct pairwise_exchange<1,m0,k,R,A> { static void run(A &a) {} };

    template <int m0,int k,typename R,typename A>
    struct pairwise_exchange<0,m0,k,R,A> { static void run(A &a) {} };

    // merge subsequences [m0,m0+k,m0+2k,...,m0+(m-1)k] amd
    // [n0,n0+k,n0+2k,...,n0+(n-1)k] of A.
    template <int m,int m0,int n,int n0,int k,typename R,typename A>
    struct merge_subsequences {
        [[gnu::always_inline]] static void run(A &a) {
            // merge 'odd' subsequences
            merge_subsequences<(m+1)/2,m0,(n+1)/2,n0,2*k,R,A>::run(a);
            // merge 'even' subsequences
            merge_subsequences<m/2,m0+k,n/2,n0+k,2*k,R,A>::run(a);
            // pair-wise merge sorted subsequences
            pairwise_exchange<m-1,m0+k,k,R,A>::run(a);

            constexpr int d=(m+1)%2;
            if (d) S<m0+(m-1)*k,n0,R>::run(a);
            pairwise_exchange<n-d,n0+d*k,k,R,A>::run(a);
        }
    };

    template <int m0,int n,int n0,int k,typename R,typename A>
    struct merge_subsequences<0,m0,n,n0,k,R,A> { static void run(A &a) {}; };

    template <int m,int m0,int n0,int k,typename R,typename A>
    struct merge_subsequences<m,m0,0,n0,k,R,A> { static void run(A &a) {}; };

    template <int m0,int n0,int k,typename R,typename A>
    struct merge_subsequences<1,m0,1,n0,k,R,A> {
        [[gnu::always_inline]] static void run(A &a) { S<m0,n0,R>::run(a); }
    };

    // minimal networks up to 6, then recusive merge

    template <int n,int n0,typename R,typename A>
    struct smallsort_inplace {
        [[gnu::always_inline]] static void run(A &a) {
            constexpr int r=n/2;
            smallsort_inplace<r,n0,R,A>::run(a);
            smallsort_inplace<n-r,n0+r,R,A>::run(a);
            merge_subsequences<r,n0,n-r,n0+r,1,R,A>::run(a);
        }
    };

    template <int n0,typename R,typename A>
    struct smallsort_inplace<0,n0,R,A> {
        static void run(A &a) {}
    };

    template <int n0,typename R,typename A>
    struct smallsort_inplace<1,n0,R,A> {
        static void run(A &a) {}
    };

    template <int n0,typename R,typename A>
    struct smallsort_inplace<2,n0,R,A> {
        [[gnu::always_inline]] static void run(A &a) { S<n0,n0+1,R>::run(a); }
    };

    template <int n0,typename R,typename A>
    struct smallsort_inplace<3,n0,R,A> {
        [[gnu::always_inline]] static void run(A &a) {
            S<n0  ,n0+1,R>::run(a);
            S<n0+1,n0+2,R>::run(a);
            S<n0  ,n0+1,R>::run(a);
        }
    };

    template <int n0,typename R,typename A>
    struct smallsort_inplace<4,n0,R,A> {
        [[gnu::always_inline]] static void run(A &a) {
            S<n0  ,n0+1,R>::run(a);
            S<n0+2,n0+3,R>::run(a);
            S<n0  ,n0+2,R>::run(a);
            S<n0+1,n0+3,R>::run(a);
            S<n0+1,n0+2,R>::run(a);
        }
    };

    template <int n0,typename R,typename A>
    struct smallsort_inplace<5,n0,R,A> {
        [[gnu::always_inline]] static void run(A &a) {
            S<n0  ,n0+1,R>::run(a);
            S<n0+2,n0+4,R>::run(a);
            S<n0  ,n0+3,R>::run(a);
            S<n0+1,n0+4,R>::run(a);
            S<n0+1,n0+2,R>::run(a);
            S<n0+3,n0+4,R>::run(a);
            S<n0  ,n0+1,R>::run(a);
            S<n0+2,n0+3,R>::run(a);
            S<n0+1,n0+2,R>::run(a);
        }
    };

    template <int n0,typename R,typename A>
    struct smallsort_inplace<6,n0,R,A> {
        [[gnu::always_inline]] static void run(A &a) {
            S<n0  ,n0+1,R>::run(a);
            S<n0+2,n0+3,R>::run(a);
            S<n0+4,n0+5,R>::run(a);
            S<n0  ,n0+2,R>::run(a);
            S<n0+1,n0+4,R>::run(a);
            S<n0+3,n0+5,R>::run(a);
            S<n0  ,n0+1,R>::run(a);
            S<n0+2,n0+3,R>::run(a);
            S<n0+4,n0+5,R>::run(a);
            S<n0+1,n0+2,R>::run(a);
            S<n0+3,n0+4,R>::run(a);
            S<n0+2,n0+3,R>::run(a);
        }
    };

} // namespace impl

using comparator = impl::reorder;

// note: custom Comparator is WIP; actual state of Comparator object
// is ignored atm.
template <int n,typename A,typename Comparator=comparator>
[[gnu::always_inline]] inline void smallsort_inplace(A &a, Comparator = Comparator{}) {
    impl::smallsort_inplace<n,0,Comparator,A>::run(a);
}

template <int n,typename A,typename Comparator=comparator>
[[gnu::always_inline]] inline A smallsort(A a, Comparator = Comparator{}) {
    impl::smallsort_inplace<n,0,Comparator,A>::run(a);
    return a;
}

} // namespace hf

#endif // ndef HF_SMALLSORT_H_
