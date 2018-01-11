#ifndef H_CLOPS_PACK
#define H_CLOPS_PACK

namespace array {
	template <typename T,typename SIZE>
	void pack64(T* v,const SIZE length, uint64_t* word, const SIZE nwords) {
		for (SIZE i = 0; i < nwords; ++i) {
			word[i] = uint64_t(0);
			for (SIZE j = 0; j < 64; ++j) {
				auto k = i*64+j;
				if (k==length) break;
				word[i] |= ( uint64_t(v[k]) << j ) ;
			}
		}
	}

	template <typename T,typename SIZE>
	void unpack64(T* v,const SIZE length, uint64_t* word, const SIZE nwords) {
		for (SIZE i = 0; i < nwords; ++i) {
			for (SIZE j = 0; j < 64; ++j) {
				auto k = i*64+j;
				if (k==length) break;
				v[k] = ( word[i] >> j ) & 1 ;
			}
		}
	}

	template <typename T,typename SIZE>
	void pack64_matrix(T** M, uint64_t** M_64,const SIZE nrows,const SIZE ncols,const SIZE ncols_64) {
		for (SIZE i = 0; i < nrows; ++i) pack64(M[i],ncols,M_64[i],ncols_64);
	}

}

#endif // H_CLOPS_PACK