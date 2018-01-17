#ifndef H_ARRAY_IS_ALL_ZEROS
#define H_ARRAY_IS_ALL_ZEROS

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
	bool is_all_zeros_64(uint64_t *pt, SIZE length) {
		const auto k(length/64);
		const auto l(length%64);
	    uint64_t* end = pt + k;
	    while (pt != end) {
	        if (*pt != uint64_t(0)) return false;
	        ++pt;
	    }
		if ( l > 0 && (*end & (uint64_t(0) << l)) != uint64_t(0)) return false;
	    return true;
	}
}

#endif // H_ARRAY_IS_ALL_ZEROS
