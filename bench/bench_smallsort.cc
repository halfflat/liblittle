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
struct lib {
    using value_type = T;
    static constexpr unsigned width = N;
    void operator()(T* v) {
        std::sort(v, v+N);
    }
};

template <typename T, unsigned N>
struct small {
    using value_type = T;
    static constexpr unsigned width = N;
    void operator()(T* v) {
        hf::small_sort_inplace<N>(v);
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
    constexpr unsigned reps = 1000;

    constexpr unsigned N = Sorter::width;
    value_type values_unsorted[N*reps];
    value_type values[N*reps];
    Sorter sorter;

    std::generate(begin(values_unsorted),end(values_unsorted),[&]() {return random_val<value_type>(gen);});

    while (state.KeepRunning()) {
        state.PauseTiming();
        std::copy_n(begin(values_unsorted), N*reps, begin(values));

        state.ResumeTiming();
        value_type* value_ptr = values;
        for (unsigned i=0; i<reps; ++i) {
            sorter(value_ptr);
            value_ptr += N;
        }
        state.PauseTiming();

        value_ptr = values;
        for (unsigned i=0; i<reps; ++i) {
            if (!std::is_sorted(value_ptr, value_ptr+N)) throw std::runtime_error("small sort didn't sort");
            value_ptr += N;
        }
    }
}

#define by ,

BENCHMARK_TEMPLATE(bench, lib<int by 3>);
BENCHMARK_TEMPLATE(bench, lib<int by 4>);
BENCHMARK_TEMPLATE(bench, lib<int by 5>);
BENCHMARK_TEMPLATE(bench, lib<int by 6>);
BENCHMARK_TEMPLATE(bench, lib<int by 7>);
BENCHMARK_TEMPLATE(bench, lib<int by 8>);
BENCHMARK_TEMPLATE(bench, lib<int by 9>);
BENCHMARK_TEMPLATE(bench, lib<int by 10>);
BENCHMARK_TEMPLATE(bench, lib<int by 11>);
BENCHMARK_TEMPLATE(bench, lib<int by 12>);

BENCHMARK_TEMPLATE(bench, small<int by 3>);
BENCHMARK_TEMPLATE(bench, small<int by 4>);
BENCHMARK_TEMPLATE(bench, small<int by 5>);
BENCHMARK_TEMPLATE(bench, small<int by 6>);
BENCHMARK_TEMPLATE(bench, small<int by 7>);
BENCHMARK_TEMPLATE(bench, small<int by 8>);
BENCHMARK_TEMPLATE(bench, small<int by 9>);
BENCHMARK_TEMPLATE(bench, small<int by 10>);
BENCHMARK_TEMPLATE(bench, small<int by 11>);
BENCHMARK_TEMPLATE(bench, small<int by 12>);

BENCHMARK_TEMPLATE(bench, lib<double by 3>);
BENCHMARK_TEMPLATE(bench, lib<double by 4>);
BENCHMARK_TEMPLATE(bench, lib<double by 5>);
BENCHMARK_TEMPLATE(bench, lib<double by 6>);
BENCHMARK_TEMPLATE(bench, lib<double by 7>);
BENCHMARK_TEMPLATE(bench, lib<double by 8>);
BENCHMARK_TEMPLATE(bench, lib<double by 9>);
BENCHMARK_TEMPLATE(bench, lib<double by 10>);
BENCHMARK_TEMPLATE(bench, lib<double by 11>);
BENCHMARK_TEMPLATE(bench, lib<double by 12>);

BENCHMARK_TEMPLATE(bench, small<double by 3>);
BENCHMARK_TEMPLATE(bench, small<double by 4>);
BENCHMARK_TEMPLATE(bench, small<double by 5>);
BENCHMARK_TEMPLATE(bench, small<double by 6>);
BENCHMARK_TEMPLATE(bench, small<double by 7>);
BENCHMARK_TEMPLATE(bench, small<double by 8>);
BENCHMARK_TEMPLATE(bench, small<double by 9>);
BENCHMARK_TEMPLATE(bench, small<double by 10>);
BENCHMARK_TEMPLATE(bench, small<double by 11>);
BENCHMARK_TEMPLATE(bench, small<double by 12>);


BENCHMARK_MAIN();
