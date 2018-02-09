#ifndef H_ARRAY_IS_EQUAL
#define H_ARRAY_IS_EQUAL

#include <algorithm>

namespace array {
	// check equality of T * arrays
	template <typename T,typename SIZE>
	bool is_equal(const T * const a, const T * const b, const SIZE n) {
		return std::equal(a,a+n,b);
	}
}

#endif // H_ARRAY_IS_EQUAL
