// workarounds for old gcc standard libraries

#include <type_traits>
#include <string>
#include <cstdio>

#if defined(__GNUC__) && __GNUC__<5
namespace std {
template <typename T>
using is_trivially_copyable = std::has_trivial_copy_constructor<T>;

std::string to_string(long long x) {
    char buf[100];
    snprintf(buf, sizeof(buf), "%lld", x);
    return std::string(buf);
}

}
#endif

