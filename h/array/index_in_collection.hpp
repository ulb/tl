#ifndef H_ARRAY_INDEX_IN_COLLECTION
#define H_ARRAY_INDEX_IN_COLLECTION

#include <algorithm> // lower bound
#include "array/is_equal.hpp"
#include "base/construct_ground_H.hpp"

namespace array {
	// returns the index of some vector in a collection of SORTED vectors
	template <typename T,typename SIZE>
	int index_in_collection(T**index, T* indices, T* point, SIZE length,SIZE dimension) {
		base::compare_ground_H<T> comp(dimension);
		T** end = index+length;
		T** lo = std::lower_bound(index, end, point, comp);
		if ( lo == end || !array::is_equal(*lo, point, dimension) ) return length;
		return indices[lo-index];
	}
}

#endif // H_ARRAY_INDEX_IN_COLLECTION
