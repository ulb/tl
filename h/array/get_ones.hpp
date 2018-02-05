#ifndef H_ARRAY_GET_ONES
#define H_ARRAY_GET_ONES

#include "mem/alloc.hpp"

namespace array {
	template <typename T,typename SIZE>
	SIZE get_ones(T* bits, const SIZE length, T*& index) {
		mem::alloc(index, length);
		SIZE n = 0;
		for (SIZE i = 0; i < length; ++i) {
			index[n] = i;
			n += bits[i];
		}
		return n;
	}

	template <typename SIZE,typename T>
	SIZE get_ones_64(uint64_t* word, const SIZE length, T*& index) {
		mem::alloc(index,length);
		SIZE n = 0;
		SIZE x = 0;
		const auto k = length / 64;
		const auto l = length % 64;
		for (SIZE i = 0; i < k; ++i) {
			uint64_t w(word[i]);
			for (SIZE j = 0; j < 64; ++j) {
				index[n] = x++;
				n += w & 1;
				w >>= 1;
			}
		}
		for (SIZE j = 0; j < l; ++j) {
			index[n] = x++;
			n += (word[k] >> j) & 1;
		}
		return n;
	}
}

#endif // H_ARRAY_GET_ONES
