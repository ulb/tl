#ifndef H_CLOPS_PACK
#define H_CLOPS_PACK

#include <cstring> // std::fill

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
	void unpack64(T* v,const SIZE length, uint64_t* word) {

		for (SIZE k = 0; k < length; ++k) {
			const auto i = k / 64 ;
			const auto j = k % 64 ;
			v[k] = ( word[i] >> j ) & 1 ;
		}

	}

	template <typename T,typename SIZE>
	void pack64_matrix(T** M, uint64_t** M_64,const SIZE nrows,const SIZE ncols,const SIZE ncols_64) {

		for (SIZE i = 0; i < nrows; ++i) pack64(M[i],ncols,M_64[i],ncols_64);

	}

}

#endif // H_CLOPS_PACK
