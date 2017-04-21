/* wrap one instance of smallsort for the purpose of inspecting the generated assembly */

#include "smallsort.h"

#ifndef TYPE
#define TYPE int
#endif

#ifndef N
#define N 7
#endif

#define wrapper_bis(T,C) sort_##T##_##C
#define wrapper(T,C) wrapper_bis(T,C)

void wrapper(TYPE,N)(TYPE* data) {
    small_sort_inplace<N>(data);
}
