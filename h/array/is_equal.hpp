#ifndef H_ARRAY_IS_EQUAL
#define H_ARRAY_IS_EQUAL

namespace array {
	// check equality of T * arrays
	template <typename T,typename SIZE>
	bool is_equal(const T * const a, const T * const b, const SIZE n) {
	    for (SIZE i = 0; i < n; ++i) if (a[i] != b[i]) return false;
	    return true;
	}
}

#endif // H_ARRAY_IS_EQUAL
