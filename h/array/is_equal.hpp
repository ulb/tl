#ifndef H_ARRAY_IS_EQUAL
#define H_ARRAY_IS_EQUAL

namespace array {
	// check equality of T * arrays
	template <typename T,typename SIZE>
	bool is_equal(T * array1,T * array2, SIZE length) {
	    for (int i = 0; i < length; ++i) if (array1[i] != array2[i]) return false;
	    return true;
	}
}

#endif // H_ARRAY_IS_EQUAL