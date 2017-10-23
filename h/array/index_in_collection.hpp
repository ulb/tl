#ifndef H_ARRAY_INDEX_IN_COLLECTION
#define H_ARRAY_INDEX_IN_COLLECTION

#include "array/is_equal.hpp"

namespace array {
	// returns the index of some vector in a collection of vectors
	template <typename T,typename SIZE>
	int index_in_collection(T ** collection,T * vect,SIZE length_collection,SIZE length_vect) {
	    for (int i = 0; i < length_collection; ++i) if (array::is_equal(collection[i],vect,length_vect)) return i;
	    return length_collection;
	}
}

#endif // H_ARRAY_INDEX_IN_COLLECTION