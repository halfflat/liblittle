#include <algorithm>
#include <array>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>

#include "smallsort.h"

// SIMD vector types behave badly in templates for older gcc
#if __GNUC__ >= 5 || defined(__ICC)
#define RUN_VECTOR 1
#endif

const char *usage_str=
    "Usage: bench_smallsort [OPTION]...\n"
    "Benchmark smallsort routines over a series of small arrays or vectors.\n\n"
    "  -n N        Run N iterations of timing loop (default: 10000)\n"
    "  -c N        Sort vector of N small arrays/vectors at a time (default 1000).\n"
    "  -S          Compare with std::sort.\n"
    "  -h, --help  Display this help and exit.\n\n"
    "Tests are performed over a series of small arrays/vectors:\n"
    "  array:Nd   -- std::array<double,N>\n"
    "  array:Nf   -- std::array<float,N>\n"
    "  array:Ni   -- std::array<int,N>\n"
    "  vector:Nd  -- double __attribute__((vector_size(N*8)))\n"
    "  vector:Nf  -- float __attribute__((vector_size(N*4)))\n"
    "Vector versions may not be included, depending on platform.\n\n"
    "Reported times are the minima observed over the timing iterations.\n";

void usage(const std::string &msg="") {
    if (!msg.empty()) {
        std::cerr << "bench_smallsort: " << msg << "\n"
                  << "Try 'bench_smallort --help' for more information.\n";
        exit(2);
    }
    else {
        std::cout << usage_str;
        exit(0);
    }
}

template <typename V>
struct elem_traits {
    static constexpr size_t size() { return sizeof(V)/sizeof(std::declval<V>()[0]); }
    typedef typename std::decay<decltype(std::declval<V>()[0])>::type value_type;
};

template <typename V>
V random_vec() {
    static std::minstd_rand G;
    V v;
    for (size_t i=0;i<elem_traits<V>::size();++i) v[i]=G();
    return v;
}

template <typename V>
void assert_sorted(V x) {
    for (size_t i=1;i<elem_traits<V>::size();++i)
        if (x[i]<x[i-1]) throw std::logic_error("item not sorted");
}

struct run_options {
    size_t n_iter=10000;
    size_t n_item=1000;
    bool std_sort_cmp=false;
};

template <typename V>
void run(const char *name,const run_options &R) {
    constexpr double inf=std::numeric_limits<double>::infinity();
    constexpr int n=elem_traits<V>::size();

    std::vector<V> data(R.n_item);
    std::chrono::duration<double,std::micro> t_small(inf),t_std(inf);

    for (size_t i=0;i<R.n_iter;++i) {
        std::generate(data.begin(),data.end(),random_vec<V>);
        auto t0=std::chrono::high_resolution_clock::now();
        for (auto &x: data) x=small_sort<n>(x);
        auto t1=std::chrono::high_resolution_clock::now();
        auto dt=t1-t0;
        if (t_small>dt) t_small=dt;
        for (const auto &x: data) assert_sorted(x);

        if (!R.std_sort_cmp) continue;

        std::generate(data.begin(),data.end(),random_vec<V>);
        t0=std::chrono::high_resolution_clock::now();
        for (auto &x: data) std::sort(&x[0],&x[0]+n);
        t1=std::chrono::high_resolution_clock::now();
        dt=t1-t0;
        if (t_std>dt) t_std=dt;
        for (const auto &x: data) assert_sorted(x);
    }
    std::cout << std::setprecision(3) << std::fixed;
    std::cout << name << '\t' << t_small.count() << " µs";
    if (R.std_sort_cmp) std::cout << '\t' << t_std.count() << " µs";
    std::cout << '\n';
}

run_options parse_args(int argc,char **argv) {
    run_options R;

    --argc;
    ++argv;
    while (argc>0) {
        if (!strcmp(argv[0],"-n")) {
            if (argc<1) usage("option '-n' requires an argument");
            R.n_iter=strtoul(argv[1],0,0);
            if (R.n_iter<1) usage("invalid iteration count: "+std::string(argv[1]));
            argc-=2;
            argv+=2;
            continue;
        }
        if (!strcmp(argv[0],"-c")) {
            if (argc<1) usage("option '-c' requires an argument");
            R.n_item=strtoul(argv[1],0,0);
            if (R.n_item<1) usage("invalid item count: "+std::string(argv[1]));
            argc-=2;
            argv+=2;
            continue;
        }
        if (!strcmp(argv[0],"-S")) {
            R.std_sort_cmp=true;
            --argc;
            ++argv;
            continue;
        }
        if (!strcmp(argv[0],"-h") || !strcmp(argv[0],"--help")) usage();

        // unrecognized option
        usage("unrecognized option '"+std::string(argv[0])+"'");
    }

    return R;
}

#if RUN_VECTOR
typedef double __attribute__((vector_size(4*sizeof(double)))) vec4d;
typedef float __attribute__((vector_size(8*sizeof(float)))) vec8f;
#endif

int main(int argc,char **argv) {
    run_options R=parse_args(argc,argv);

#define RUN_DBL_ARRAY(n) run<std::array<double,n>>("array:" #n "d",R)
#define RUN_FLT_ARRAY(n) run<std::array<float,n>>("array:" #n "f",R)
#define RUN_INT_ARRAY(n) run<std::array<int,n>>("array:" #n "i",R)
    RUN_DBL_ARRAY(3);
    RUN_DBL_ARRAY(4);
    RUN_DBL_ARRAY(5);
    RUN_DBL_ARRAY(6);
    RUN_DBL_ARRAY(7);
    RUN_DBL_ARRAY(8);
    RUN_DBL_ARRAY(9);
    RUN_DBL_ARRAY(10);
    RUN_DBL_ARRAY(11);
    RUN_DBL_ARRAY(12);
    RUN_DBL_ARRAY(13);
    RUN_DBL_ARRAY(14);
    RUN_DBL_ARRAY(15);
    RUN_DBL_ARRAY(16);
//    RUN_DBL_ARRAY(17);
//    RUN_DBL_ARRAY(18);
//    RUN_DBL_ARRAY(19);
//    RUN_DBL_ARRAY(20);
//    RUN_DBL_ARRAY(21);
//    RUN_DBL_ARRAY(22);
//    RUN_DBL_ARRAY(23);
//    RUN_DBL_ARRAY(24);
//    RUN_DBL_ARRAY(25);
//
//    RUN_FLT_ARRAY(4);
//    RUN_FLT_ARRAY(8);
//    RUN_FLT_ARRAY(12);
//    RUN_FLT_ARRAY(16);
//    RUN_FLT_ARRAY(20);
//    RUN_FLT_ARRAY(24);
//
    RUN_FLT_ARRAY(3);
    RUN_FLT_ARRAY(4);
    RUN_FLT_ARRAY(5);
    RUN_FLT_ARRAY(6);
    RUN_FLT_ARRAY(7);
    RUN_FLT_ARRAY(8);
    RUN_FLT_ARRAY(9);
    RUN_FLT_ARRAY(10);
    RUN_FLT_ARRAY(11);
    RUN_FLT_ARRAY(12);
    RUN_FLT_ARRAY(13);
    RUN_FLT_ARRAY(14);
    RUN_FLT_ARRAY(15);
    RUN_FLT_ARRAY(16);
    RUN_INT_ARRAY(3);
    RUN_INT_ARRAY(4);
    RUN_INT_ARRAY(5);
    RUN_INT_ARRAY(6);
    RUN_INT_ARRAY(7);
    RUN_INT_ARRAY(8);
    RUN_INT_ARRAY(9);
    RUN_INT_ARRAY(10);
    RUN_INT_ARRAY(11);
    RUN_INT_ARRAY(12);
    RUN_INT_ARRAY(13);
    RUN_INT_ARRAY(14);
    RUN_INT_ARRAY(15);
    RUN_INT_ARRAY(16);
//    RUN_INT_ARRAY(17);
//    RUN_INT_ARRAY(18);
//    RUN_INT_ARRAY(19);
//    RUN_INT_ARRAY(20);

#if RUN_VECTOR
    run<vec4d>("vector:4d",R);
    run<vec8f>("vector:8f",R);
#endif

    return 0;
}
