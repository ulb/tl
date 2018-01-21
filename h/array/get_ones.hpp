#ifndef H_ARRAY_GET_ONES
#define H_ARRAY_GET_ONES

#include "mem/alloc.hpp"

namespace array {
	template <typename T,typename SIZE>
	SIZE get_ones(T* char_v,const SIZE length,T*& one_indices) {
		mem::alloc(one_indices,length);
		SIZE num_one_indices = 0;
		for (SIZE i = 0; i < length; ++i) {
			one_indices[num_one_indices] = i;
			num_one_indices += char_v[i];
		}
		return num_one_indices;
	}

	template <typename SIZE,typename T>
	SIZE get_ones_64(uint64_t* word,const SIZE length,T*& one_indices) {
		mem::alloc(one_indices,length);
		SIZE num_one_indices = 0;
		for (SIZE k = 0; k < length; ++k) {
			const auto i = k / 64 ;
			const auto j = k % 64 ;
			one_indices[num_one_indices] = k;
			num_one_indices += ( ( word[i] >> j ) & 1 );
		}
		return num_one_indices;
	}
}

#endif // H_ARRAY_GET_ONES
