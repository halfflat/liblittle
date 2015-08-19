#ifndef SMALLSORT_HPP
#define SMALLSORT_HPP 1

/// Inline sorting of small, fixed-size
/// random-access sequences.

#include <utility>
#include <algorithm>
#include <iostream>

namespace impl {

    static void emit(int m,int n) {
        std::cout << "swap " << (m+1) << ", " << (n+1) << "\n";
    }

    template <typename T>
    inline void cmp_swap(T &a,T &b) {
        if (a>b) std::swap(a,b);
    }

    // convenience template for below:

    template <int m,int n>
    struct S {
        template <typename A>
        static void run(A &a) { cmp_swap(a[m],a[n]); }
    };

    // merging networks

    // swap m/2 adjacent pairs (m0,m0+1),(m0+2,m0+3),...
    template <int m,int m0,int k,typename A>
    struct pairwise_exchange {
        static void run(A &a) {
            S<m0,m0+k>::run(a);
            pairwise_exchange<m-2,m0+2*k,k,A>::run(a);
        }
    };

    template <int m0,int k,typename A>
    struct pairwise_exchange<1,m0,k,A> { static void run(A &a) {} };

    template <int m0,int k,typename A>
    struct pairwise_exchange<0,m0,k,A> { static void run(A &a) {} };

    // merge subsequences [m0,m0+k,m0+2k,...,m0+(m-1)k] amd
    // [n0,n0+k,n0+2k,...,n0+(n-1)k] of A.
    template <int m,int m0,int n,int n0,int k,typename A>
    struct merge_subsequences {
        static void run(A &a) {
            // merge 'odd' subsequences
            merge_subsequences<(m+1)/2,m0,(n+1)/2,n0,2*k,A>::run(a);
            // merge 'even' subsequences
            merge_subsequences<m/2,m0+k,n/2,n0+k,2*k,A>::run(a);
            // pair-wise merge sorted subsequences
            pairwise_exchange<m-1,m0+k,k,A>::run(a);
            
            constexpr int d=(m+1)%2;
            if (d) S<m0+(m-1)*k,n0>::run(a);
            pairwise_exchange<n-d,n0+d*k,k,A>::run(a);
        }
    };
    

    template <int m0,int n,int n0,int k,typename A>
    struct merge_subsequences<0,m0,n,n0,k,A> { static void run(A &a) {}; };

    template <int m,int m0,int n0,int k,typename A>
    struct merge_subsequences<m,m0,0,n0,k,A> { static void run(A &a) {}; };

    template <int m0,int n0,int k,typename A>
    struct merge_subsequences<1,m0,1,n0,k,A> {
        static void run(A &a) { S<m0,n0>::run(a); }
    };

    // minimal networks up to 5, then recusive merge

    template <int n,int n0,typename A>
    struct small_sort_inplace {
        static void run(A &a) {
            constexpr int r=n/2;
            small_sort_inplace<r,n0,A>::run(a);
            small_sort_inplace<n-r,n0+r,A>::run(a);
            merge_subsequences<r,n0,n-r,n0+r,1,A>::run(a);
        }
    };

    template <int n0,typename A>
    struct small_sort_inplace<0,n0,A> {
        static void run(A &a) {}
    };

    template <int n0,typename A>
    struct small_sort_inplace<1,n0,A> {
        static void run(A &a) {}
    };

    template <int n0,typename A>
    struct small_sort_inplace<2,n0,A> {
        static void run(A &a) { S<n0,n0+1>::run(a); }
    };

    template <int n0,typename A>
    struct small_sort_inplace<3,n0,A> {
        static void run(A &a) {
            S<n0  ,n0+1>::run(a);
            S<n0+1,n0+2>::run(a);
            S<n0  ,n0+1>::run(a);
        }
    };

    template <int n0,typename A>
    struct small_sort_inplace<4,n0,A> {
        static void run(A &a) {
            S<n0  ,n0+1>::run(a);
            S<n0+2,n0+3>::run(a);
            S<n0  ,n0+2>::run(a);
            S<n0+1,n0+3>::run(a);
            S<n0+1,n0+2>::run(a);
        }
    };

    template <int n0,typename A>
    struct small_sort_inplace<5,n0,A> {
        static void run(A &a) {
            S<n0  ,n0+1>::run(a);
            S<n0+2,n0+4>::run(a);
            S<n0  ,n0+3>::run(a);
            S<n0+1,n0+4>::run(a);
            S<n0+1,n0+2>::run(a);
            S<n0+3,n0+4>::run(a);
            S<n0  ,n0+1>::run(a);
            S<n0+2,n0+3>::run(a);
            S<n0+1,n0+2>::run(a);
        }
    };
} // namespace impl

template <int n,typename A>
void small_sort_inplace(A &a) {
    impl::small_sort_inplace<n,0,A>::run(a);
}

template <int n,typename A>
A small_sort(A a) {
    impl::small_sort_inplace<n,0,A>::run(a);
    return a;
}

#endif // ndef SMALLSORT_HPP


