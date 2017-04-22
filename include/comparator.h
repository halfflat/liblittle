#ifndef HF_COMPARATOR_H_
#define HF_COMPARATOR_H_

namespace hf {


template <typename V>
struct comparator {
    void operator()(V& a, V& b) const {
        if (std::is_arithmetic<typename std::decay<V>::type>::value)  {
            auto l = a<b? a: b;
            b = a<b? b: a;
            a = std::move(l);
        }
        else {
        }
    }
};

struct reorder {
    template <typename V>
    void operator()(V& a, V& b) const {
        (*this)(a, b, typename std::is_arithmetic<typename std::decay<V>::type>::type{});
    }

    protected:
        template <typename V>
        void operator()(V& a,V& b,std::true_type) const {
        }

        template <typename V>
        void operator()(V& a,V& b,std::false_type) const {
            if (!(a<b)) std::swap(a,b);
        }
    };

    template <int m,int n,typename Reorder>
    struct S {
        template <typename A>
        [[gnu::always_inline]] static void run(A &a) {
            Reorder{}(a[m],a[n]);
        }
    };


#endif // ndef HF_COMPARATOR_H_
