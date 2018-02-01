#ifndef H_ST_MIN_A_IDX
#define H_ST_MIN_A_IDX

#include "array/is_equal.hpp"

namespace st {
	template <typename T,typename SIZE>
	T min_A_idx(T *A, const SIZE length) {
	    for (SIZE i = 0; i < length; ++i) if (A[i] == 1) return i;
	    return length;
	}
}

#endif // H_ST_MIN_A_IDX
