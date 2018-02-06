#ifndef H_CLOPS_PACK
#define H_CLOPS_PACK

#include <cstring> // std::fill
#include "linalg/div_ceil.hpp"

namespace array {

	template <typename T,typename SIZE>
	void pack64(T* v,const SIZE length, uint64_t* word, const SIZE nwords) {

		std::fill(word,word+nwords,uint64_t(0));

		for (SIZE k = 0; k < length; ++k) {
			const auto i = k / 64 ;
			const auto j = k % 64 ;
			word[i] |= ( uint64_t(v[k]) << j ) ;
		}

	}

	template <typename T,typename SIZE>
	void unpack64(T* v, const SIZE length, uint64_t* word) {

		const auto k = length / 64;
		const auto l = length % 64;

		for (SIZE i = 0; i < k; ++i) {
			auto w(word[i]);
			for ( SIZE j = 0 ; j < 64 ; ++j) {
				*(v++) = w & 1 ;
				w >>= 1;
			}
		}

		for (SIZE j = 0 ; j < l ; ++j) {
			*(v++) = ( word[k] >> j ) & 1 ;
		}

	}

	template <typename T,typename SIZE>
	void pack64_matrix(T** M, uint64_t** M_64,const SIZE nrows,const SIZE ncols,const SIZE ncols_64) {

		for (SIZE i = 0; i < nrows; ++i) pack64(M[i],ncols,M_64[i],ncols_64);

	}

	template <typename T,typename SIZE>
	void pack64_matrix_triangular(T** M, uint64_t** M_64,const SIZE nrows) {

		for (SIZE i = 1; i < nrows; ++i) pack64(M[i],i,M_64[i],linalg::div_ceil(i,64));

	}

}

#endif // H_CLOPS_PACK
