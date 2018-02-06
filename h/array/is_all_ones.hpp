#ifndef H_ARRAY_IS_ALL_ONES
#define H_ARRAY_IS_ALL_ONES



namespace array {
	// check if an int* is all ones
	template <typename T,typename SIZE>
	bool is_all_ones(const T * pt, const SIZE length) {
	    const T * const end(pt + length);
	    while (pt != end) {
	        if (*pt == 0) return false;
	        ++pt;
	    }
	    return true;
	}

	template<typename SIZE>
	bool is_all_ones_64(const uint64_t * pt, const SIZE length) {
		const auto k(length/64);
		const auto l(length%64);
	    const uint64_t * const end(pt + k);
	    while (pt != end) {
	        if (*pt != ~uint64_t(0)) return false;
	        ++pt;
	    }
		if ( l > 0 && (*end | (~uint64_t(0) << l)) != ~uint64_t(0)) return false;
	    return true;
	}
}

#endif // H_ARRAY_IS_ALL_ONES
