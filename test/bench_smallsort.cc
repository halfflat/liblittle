#include <cstdlib>
#include <cstring>
#include <string>
#include <array>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <stdexcept>

#include "smallsort.h"

const char *usage_str=
    "Usage: bench_smallsort [OPTION]...\n"
    "Benchmark smallsort routines over a series of small arrays or vectors.\n\n"
    "  -n N        Run N iterations of timing loop (default: 10000)\n"
    "  -c N        Sort vector of N small arrays/vectors at a time (default 2000).\n"
    "  -h, --help  Display this help and exit.\n\n"
    "Tests are performed over the following small arrays/vectors:\n"
    "  array3d   -- std::array<double,3>\n"
    "  array4d   -- std::array<double,3>\n"
    "  array25f  -- std::array<float,25>\n"
    "  vec4d     -- double __attribute__((vector_size(32)))\n"
    "  vec8f     -- float __attribute__((vector_size(32)))\n";

void usage(const std::string &msg="") {
    if (msg) {
        std::cerr << "bench_smallsort: " << msg << "\n";
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
    size_t n_item=2000;
};

template <typename V>
void run(const char *name,const run_options &R) {
    std::vector<V> data(R.n_item);
    std::chrono::duration<double,std::micro> t(0);

    for (size_t i=0;i<R.n_iter;++i) {
        std::generate(data.begin(),data.end(),random_vec<V>);
        auto t0=std::chrono::high_resolution_clock::now();
        for (auto &x: data) x=small_sort<elem_traits<V>::size()>(x);
        auto t1=std::chrono::high_resolution_clock::now();
        t+=t1-t0;
        for (const auto &x: data) assert_sorted(x);
    }
    std::cout << std::setw(10) << name << t.count() << " µs\n";
}

run_options parse_args(int argc,char **argv) {
    run_options R;

    while (argc>0) {
        if (!strcmp(argv[0],"-n")) {
            if (argc<1) usage("option '-n' requires an argument");
            R.n_iter=strtoul(argv[1],0,0);
            if (R.n_iter<1) usage("invalid iteration count: "+string(argv[1]));
            argc-=2;
            argv+=2;
            continue;
        }
        if (!strcmp(argv[0],"-c")) {
            if (argc<1) usage("option '-c' requires an argument");
            R.n_item=strtoul(argv[1],0,0);
            if (R.n_item<1) usage("invalid item count: "+string(argv[1]));
            argc-=2;
            argv+=2;
            continue;
        }
        if (!strcmp(argv[0],"-h") || !strcmp(argv[0],"--help")) usage();

        // unrecognized option
        usage("unrecognized option '"+std::string(argv[0])+"'");
    }

    return R;
}

typedef std::array<double,3> array3d;
typedef std::array<double,4> array4d;
typedef std::array<float,25> array25f;
typedef double __attribute__((vector_size(4*sizeof(double)))) vec4d;
typedef float __attribute__((vector_size(8*sizeof(float)))) vec8f;

int main(int argc,char **argv) {
    if (argc!=2 && argc!=3) usage();

    run_options R=parse_args(argc,argv);

    run<array3d>("array3d",R);
    run<array4d>("array4d",R);
    run<array25f>("array25f",R);
    run<vec4d>("vec4d",R);
    run<vec8f>("vec8f",R);

    return 0;
}
