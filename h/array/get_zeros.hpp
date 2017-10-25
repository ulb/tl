#ifndef H_ARRAY_GET_ZEROS
#define H_ARRAY_GET_ZEROS

#include "alloc.hpp"

namespace array {
	template <typename T,typename SIZE>
	SIZE get_zeros(T * v,const SIZE length,T *& zero_indices_v) {
		alloc(zero_indices_v,length,T);
		SIZE num_zero_indices_v = 0;
		for (SIZE i = 0; i < length; ++i) if (v[i] == 0) zero_indices_v[num_zero_indices_v++] = i;
		return num_zero_indices_v;
	}
}

#endif // H_ARRAY_GET_ZEROS