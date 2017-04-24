#include "compat.h"

#include <algorithm>
#include <cstring>
#include <random>
#include <stdexcept>
#include <type_traits>

#include "benchmark/benchmark.h"
#include "smallsort.h"

template <typename T, typename G>
inline T random_val(G& gen) {
    using distribution = typename std::conditional<
        std::is_integral<T>::value,
        std::uniform_int_distribution<T>,
        std::uniform_real_distribution<T>
    >::type;

    static distribution dist;
    return dist(gen);
}

template <typename T, unsigned N>
struct run_stdsort {
    using value_type = T;
    static constexpr unsigned width = N;
    void operator()(T* v) {
        std::sort(v, v+N);
    }
};

template <typename T, unsigned N>
struct run_smallsort {
    using value_type = T;
    static constexpr unsigned width = N;
    void operator()(T* v) {
        hf::smallsort_inplace<N>(v);
    }
};

template <typename Sorter>
void bench(benchmark::State& state) {
    using value_type = typename Sorter::value_type;
    using std::begin;
    using std::end;

    std::minstd_rand gen;

    // reps should be large enough to both reduce the
    // effect of timing overhead, and also flummox
    // any branch prediction.
    constexpr unsigned reps = 10000;

    constexpr unsigned N = Sorter::width;
    value_type values_unsorted[N*reps];
    value_type values[N*reps];
    Sorter sorter;

    std::generate(begin(values_unsorted),end(values_unsorted),[&]() {return random_val<value_type>(gen);});

    while (state.KeepRunning()) {
        state.PauseTiming();
        std::copy_n(begin(values_unsorted), N*reps, begin(values));
        benchmark::ClobberMemory();

        state.ResumeTiming();
        value_type* value_ptr = values;
        for (unsigned i=0; i<reps; ++i) {
            sorter(value_ptr);
            value_ptr += N;
        }
        benchmark::ClobberMemory();
    }

    auto value_ptr = values;
    for (unsigned i=0; i<reps; ++i) {
        if (!std::is_sorted(value_ptr, value_ptr+N)) throw std::runtime_error("small sort didn't sort");
        value_ptr += N;
    }
}

// Type list chicanery... 

template <typename V, V...>
struct foreach_value {
    template <template <V, typename...> class X, typename... Xparam, typename... Args>
    static void run(Args&&... args) {}
};

template <typename V, V a, V... bs>
struct foreach_value<V, a, bs...> {
    template <template <V, typename...> class X, typename... Xparam, typename... Args>
    static void run(Args&&... args) {
        X<a, Xparam...> _(args...);
        foreach_value<V, bs...>::template run<X, Xparam...>(args...);
    }
};

template <typename...>
struct foreach_type {
    template <template <typename...> class X, typename... Xparam, typename... Args>
    static void run(Args&&... args) {}
};

template <typename H, typename... Tail>
struct foreach_type<H, Tail...> {
    template <template <typename...> class X, typename... Xparam, typename... Args>
    static void run(Args&&... args) {
        X<H, Xparam...> _(args...);
        foreach_type<Tail...>::template run<X, Xparam...>(args...);
    }
};

// Benchmark registration...

std::string type_name(short) { return "short"; }
std::string type_name(unsigned short) { return "ushort"; }
std::string type_name(int) { return "int"; }
std::string type_name(unsigned) { return "uint"; }
std::string type_name(float) { return "float"; }
std::string type_name(double) { return "double"; }

template <int n, typename V>
struct register_smallsort_bench {
    register_smallsort_bench() {
        auto name = "smallsort/"+type_name(V{})+"/"+std::to_string(n);
        benchmark::RegisterBenchmark(name.c_str(), bench<run_smallsort<V, n>>);
    }
};

template <int n, typename V>
struct register_stdsort_bench {
    register_stdsort_bench() {
        auto name = "stdsort/"+type_name(V{})+"/"+std::to_string(n);
        benchmark::RegisterBenchmark(name.c_str(), bench<run_stdsort<V, n>>);
    }
};

template <int...> struct intlist {};
template <typename, typename> struct register_benches;

template <typename V, int... sizes>
struct register_benches<V, intlist<sizes...>> {
    register_benches() {
        foreach_value<int, sizes...>::template run<register_smallsort_bench, V>();
        foreach_value<int, sizes...>::template run<register_stdsort_bench, V>();
    }
};

int main(int argc,char** argv) {
    using sizes = intlist<3,4,5,6,7,8,9,10,11,12,13,14,15,20,25,30,35,40,45>;
    foreach_type<short, unsigned short, int, unsigned, float, double>::run<register_benches, sizes>();

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
