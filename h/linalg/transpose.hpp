#ifndef H_LINALG_TRANSPOSE
#define H_LINALG_TRANSPOSE

namespace linalg {
	template <typename T,typename SIZE>
	void transpose(T ** M, T ** Mt, const SIZE m, const SIZE n) {
		for ( SIZE i = 0 ; i < m ; ++i )
			for ( SIZE j = 0 ; j < n ; ++j )
				Mt[j][i] = M[i][j];
	}
}

#endif // H_LINALG_TRANSPOSE
