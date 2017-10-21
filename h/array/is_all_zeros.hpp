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
}

#endif // H_ARRAY_IS_ALL_ZEROS