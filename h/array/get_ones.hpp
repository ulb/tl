#ifndef H_ARRAY_GET_ONES
#define H_ARRAY_GET_ONES

#include "mem/alloc.hpp"

namespace array {
	template <typename T,typename SIZE>
	SIZE get_ones(const T * const bits, const SIZE length, T*& index) {
		mem::alloc(index, length);
		SIZE n = 0;
		for (SIZE i = 0; i < length; ++i) {
			index[n] = i;
			n += bits[i];
		}
		return n;
	}

	template <typename SIZE,typename T>
	SIZE get_ones_64(const uint64_t * word, const SIZE length, T*& index) {
		mem::alloc(index, length);
		SIZE n = 0;
		SIZE x = 0;
		auto k = length / 64;
		auto l = length % 64;
		while (k-- != 0) {
			uint64_t w(*(word++));
			for (SIZE j = 0; j < 64; ++j) {
				index[n] = x++;
				n += w & 1;
				w >>= 1;
			}
		}
		if ( l != 0 ) {
			uint64_t w(*word);
			do {
				index[n] = x++;
				n += w & 1;
				w >>= 1;
			} while ( --l != 0 ) ;
		}
		return n;
	}
}

#endif // H_ARRAY_GET_ONES
