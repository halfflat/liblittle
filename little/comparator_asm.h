#ifndef HF_COMPARATOR_ASM_H_
#define HF_COMPARATOR_ASM_H_

#include <cstdint>

namespace hf {

// Definition of `template <typename V> struct comparator` is in comparator.h

#ifdef __x86_64__
#ifdef __SSE__
template <>
struct comparator<float> {
    void operator()(float& a, float& b) const {
        float c = a;
        asm("minss   %1, %0\n\t"
            "maxss   %2, %1\n\t"
            : "+&x" (c), "+x" (b)
            : "x" (a));
        a = c;
    }
};
#endif


#ifdef __SSE2__
template <>
struct comparator<double> {
    void operator()(double& a, double& b) const {
        double c = a;
        asm("minsd   %1, %0\n\t"
            "maxsd   %2, %1\n\t"
            : "+&x" (c), "+x" (b)
            : "x" (a));
        a = c;
    }
};
#endif

namespace impl {
    template <typename T>
    void unsigned_comparator(T& a, T& b) {
        auto c = a;
        asm("cmp     %2, %1\n\t"
            "cmovb   %1, %0\n\t"
            "cmovb   %2, %1\n\t"
            : "+&r" (c), "+r" (b) : "r" (a) : "cc");
        a = c;
    }

    template <typename T>
    void signed_comparator(T& a, T& b) {
        auto c = a;
        asm("cmp     %2, %1\n\t"
            "cmovl   %1, %0\n\t"
            "cmovl   %2, %1\n\t"
            : "+&r" (c), "+r" (b) : "r" (a) : "cc");
        a = c;
    }
}

template <>
struct comparator<unsigned char> {
    void operator()(unsigned char& a, unsigned char& b) const {
        unsigned short as = a;
        unsigned short bs = b;
        impl::unsigned_comparator(as, bs);
        a = as;
        b = bs;
    }
};

template <>
struct comparator<unsigned short> {
    void operator()(unsigned short& a, unsigned short& b) const {
        impl::unsigned_comparator(a, b);
    }
};

template <>
struct comparator<unsigned> {
    void operator()(unsigned& a, unsigned& b) const {
        impl::unsigned_comparator(a, b);
    }
};

template <>
struct comparator<unsigned long> {
    void operator()(unsigned long& a, unsigned long& b) const {
        impl::unsigned_comparator(a, b);
    }
};

template <>
struct comparator<unsigned long long> {
    void operator()(unsigned long long& a, unsigned long long& b) const {
        impl::unsigned_comparator(a, b);
    }
};

template <>
struct comparator<signed char> {
    void operator()(signed char& a, signed char& b) const {
        short as = a;
        short bs = b;
        impl::signed_comparator(as, bs);
        a = as;
        b = bs;
    }
};

template <>
struct comparator<short> {
    void operator()(short& a, short& b) const {
        impl::signed_comparator(a, b);
    }
};

template <>
struct comparator<int> {
    void operator()(int& a, int& b) const {
        impl::signed_comparator(a, b);
    }
};

template <>
struct comparator<long> {
    void operator()(long& a, long& b) const {
        impl::signed_comparator(a, b);
    }
};

template <>
struct comparator<long long> {
    void operator()(long long& a, long long& b) const {
        impl::signed_comparator(a, b);
    }
};

template <typename T>
struct comparator<T*> {
    void operator()(T*& a, T*& b) const {
        impl::unsigned_comparator(a, b);
    }
};

template <>
struct comparator<char> {
    void operator()(char& a, char& b) {
        if (std::is_signed<char>::value)
            comparator<signed char>{}(reinterpret_cast<signed char&>(a), reinterpret_cast<signed char&>(b));
        else
            comparator<unsigned char>{}(reinterpret_cast<unsigned char&>(a), reinterpret_cast<unsigned char&>(b));
    }
};

#endif // def __x86_64__

} // namespace hf

#endif // ndef HF_COMPARATOR_ASM_H_

