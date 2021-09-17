#include <algorithm>
#include <cstring>
#include <random>
#include <stdexcept>
#include <type_traits>

#include "benchmark/benchmark.h"
#include "little/multiset.h"

using namespace hf;

template <typename T>
struct random_val {
    template <typename Gen>
    T operator()(Gen& gen) const {
        using distribution = typename std::conditional<
            std::is_integral<T>::value,
            std::uniform_int_distribution<T>,
            std::uniform_real_distribution<T>
        >::type;

        distribution dist;
        return dist(gen);
    }
};

template <typename MSet>
void bench_count(benchmark::State& state) {
    using value_type = typename MSet::value_type;
    using std::begin;
    using std::end;

    using Gen = std::minstd_rand;
    Gen gen;

    // generate 2K keys; place N keys from
    // the first half in the map

    std::size_t N = state.range(0);
    std::size_t K = N+N/2;

    std::vector<value_type> keys(2*K);
    std::generate(keys.begin(), keys.end(), [&gen]() { return random_val<value_type>{}(gen); });

    MSet mset;
    std::multiset<value_type> check;

    std::uniform_int_distribution<std::size_t> index(0, K);
    for (std::size_t j = 0; j<N; ++j) {
        auto v = keys[index(gen)];
        mset.insert(v);
        check.insert(v);
    }
    std::shuffle(keys.begin(), keys.end(), gen);

    while (state.KeepRunning()) {
        for (const auto& k: keys) {
            benchmark::DoNotOptimize(mset.count(k));
        }
    }

    for (const auto& k: keys) {
        if (mset.count(k)!=check.count(k)) throw std::runtime_error("multiset counts do not match");
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

std::string type_name(char) { return "char"; }
std::string type_name(short) { return "short"; }
std::string type_name(unsigned short) { return "ushort"; }
std::string type_name(int) { return "int"; }
std::string type_name(unsigned) { return "uint"; }
std::string type_name(long long) { return "longlong"; }
std::string type_name(unsigned long long) { return "ulonglong"; }
std::string type_name(float) { return "float"; }
std::string type_name(double) { return "double"; }

template <int n, typename V>
struct register_tinymultiset_bench {
    register_tinymultiset_bench() {
        auto name = "tinymultiset.count/"+type_name(V{});
        using mset = tiny::multiset<V,n>;
        benchmark::RegisterBenchmark(name.c_str(), bench_count<mset>)->Arg(n);
    }
};

template <int n, typename V>
struct register_stdmultiset_bench {
    register_stdmultiset_bench() {
        auto name = "stdmultiset.count/"+type_name(V{});
        using mset = std::multiset<V>;
        benchmark::RegisterBenchmark(name.c_str(), bench_count<mset>)->Arg(n);
    }
};

template <int...> struct intlist {};
template <typename, typename> struct register_benches;

template <typename V, int... sizes>
struct register_benches<V, intlist<sizes...>> {
    register_benches() {
        foreach_value<int, sizes...>::template run<register_tinymultiset_bench, V>();
        foreach_value<int, sizes...>::template run<register_stdmultiset_bench, V>();
    }
};

int main(int argc,char** argv) {
    using sizes = intlist<2,3,4,5,6,7,8,9,10>;
    foreach_type<int, double>::run<register_benches, sizes>();

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
