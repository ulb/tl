#ifndef H_ARRAY_DUMP
#define H_ARRAY_DUMP

#include <iostream>
#include <bitset>

namespace array {
	template <typename T, typename SIZE>
	void dump(T* v, const SIZE length) {
		for (SIZE i = 0; i < length; ++i) std::cerr << v[i] << ' ' ;
		std::cerr << std::endl ;
	}

	template <typename W, typename SIZE>
	void dump_64(W* w, const SIZE nwords) {
		for (SIZE i = 0; i < nwords; ++i) std::cerr << std::bitset<64>(w[i]) << ' ' ;
		std::cerr << std::endl ;
	}

	template <typename T, typename SIZE>
	void dump_matrix(T** M, const SIZE nrows, const SIZE ncols) {
		for (SIZE i = 0; i < nrows; ++i) dump(M[i], ncols);
	}

	template <typename W, typename SIZE>
	void dump_matrix_64(W** M, const SIZE nrows, const SIZE nwords) {
		for (SIZE i = 0; i < nrows; ++i) dump_64(M[i], nwords);
	}
}

#endif // H_ARRAY_DUMP