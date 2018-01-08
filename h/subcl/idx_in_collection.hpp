#ifndef H_SUBCL_IDX_IN_COLLECTION
#define H_SUBCL_IDX_IN_COLLECTION

#include "array/is_equal.hpp"

namespace subcl {
	template <typename T,typename SIZE>
	SIZE idx_in_collection(T ** collection, T * col,const SIZE collection_length, const SIZE length) {
		for (SIZE i = 0; i < collection_length; ++i) if (array::is_equal(collection[i],col,length)) return i;
		return collection_length;
	}
}
#endif // H_SUBCL_IDX_IN_COLLECTION