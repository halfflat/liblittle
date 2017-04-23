#ifndef HF_COMPARATOR_H_
#define HF_COMPARATOR_H_

namespace hf {

template <typename V>
struct comparator {
    void operator()(V& a, V& b) const {
        if (std::is_arithmetic<typename std::decay<V>::type>::value)  {
            auto l = std::min(a,b);
            auto u = std::max(a,b);
            a = l;
            b = u;
        }
        else {
            if (!(a<b)) std::swap(a,b);
        }
    }
};

} // namespace hf

#ifdef HF_USE_ASM_KERNELS
#include "comparator_asm.h"
#endif

#endif // ndef HF_COMPARATOR_H_
