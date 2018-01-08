#ifndef H_SUBCL_INDEX_IN_ARRAY
#define H_SUBCL_INDEX_IN_ARRAY

#include "array/is_equal.hpp"

namespace subcl {
	template <typename T,typename SIZE>
	SIZE index_in_array(T * array,T val,SIZE length) {
		for (SIZE i = 0; i < length; ++i) if (array[i] == val) return i;
		return length;
	}
}
#endif // H_SUBCL_INDEX_IN_ARRAY