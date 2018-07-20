#ifndef H_ARRAY_IS_ALL_ZEROS
#define H_ARRAY_IS_ALL_ZEROS

// #include "array/dump.hpp"
// #include "nt/pow.hpp"

namespace array {
	// check if an int* is all ones
	template <typename T,typename SIZE>
	bool is_all_zeros(T * pt,SIZE length) {
	    T * end = pt + length;
	    while (pt != end) {
	        if (*pt == 1) return false;
	        ++pt;
	    }
	    return true;
	}

	template <typename SIZE>
	bool is_all_zeros_64(const uint64_t *pt,const SIZE length) {
		const auto k(length / 64);
		const auto l(length % 64);
	    const uint64_t * const end(pt + k);
	    while (pt != end) {
	        if (*(pt++) != uint64_t(0)) return false;
	    }
		if ( l > 0 && (*end & (~uint64_t(0) >> (64-l))) != uint64_t(0)) return false;
	    return true;
	}
}

#endif // H_ARRAY_IS_ALL_ZEROS
