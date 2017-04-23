#ifndef HF_COMPARATOR_ASM_H_
#define HF_COMPARATOR_ASM_H_

namespace hf {

// Definitionf of `template <typename V> struct comparator` in comparator.h

#ifdef __x86_64__
#ifdef __SSE__
template <>
struct comparator<float> {
    void operator()(float& a, float& b) const {
        float c = a;
        asm("minss %1, %0 \n\t"
            "maxss %2, %1 \n\t"
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
        asm("minsd %1, %0 \n\t"
            "maxsd %2, %1 \n\t"
            : "+&x" (c), "+x" (b)
            : "x" (a));
        a = c;
    }
};
#endif

template <>
struct comparator<std::uint16_t> {
    void operator()(std::uint16_t& a, std::uint16_t& b) const {
        auto c = a;
        asm("cmp %2, %1  \n\t"
            "cmovb %1, %0 \n\t"
            "cmovb %2, %1 \n\t"
            : "+&r" (c), "+r" (b) : "r" (a) : "cc");
        a = c;
    }
};

template <>
struct comparator<std::uint32_t> {
    void operator()(std::uint32_t& a, std::uint32_t& b) const {
        auto c = a;
        asm("cmp %2, %1  \n\t"
            "cmovbl %1, %0 \n\t"
            "cmovbl %2, %1 \n\t"
            : "+&r" (c), "+r" (b) : "r" (a) : "cc");
        a = c;
    }
};

template <>
struct comparator<std::int16_t> {
    void operator()(std::int16_t& a, std::int16_t& b) const {
        auto c = a;
        asm("cmp %2, %1  \n\t"
            "cmovl %1, %0 \n\t"
            "cmovl %2, %1 \n\t"
            : "+&r" (c), "+r" (b) : "r" (a) : "cc");
        a = c;
    }
};

template <>
struct comparator<std::int32_t> {
    void operator()(std::int32_t& a, std::int32_t& b) const {
        auto c = a;
        asm("cmp %2, %1  \n\t"
            "cmovll %1, %0 \n\t"
            "cmovll %2, %1 \n\t"
            : "+&r" (c), "+r" (b) : "r" (a) : "cc");
        a = c;
    }
};
#endif // def __x86_64__

} // namespace hf

#endif // ndef HF_COMPARATOR_ASM_H_

