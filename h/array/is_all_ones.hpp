#ifndef H_ARRAY_IS_ALL_ONES
#define H_ARRAY_IS_ALL_ONES

namespace array {
	// check if an int* is all ones
	template <typename T,typename SIZE>
	bool is_all_ones(T * pt,SIZE length) {
	    T* end = pt + length;
	    while (pt != end) {
	        if (*pt == 0) return false;
	        ++pt;
	    }
	    return true;
	}
}

#endif // H_ARRAY_IS_ALL_ONES