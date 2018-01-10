#ifndef H_LINALG_TRASPOSEM
#define H_LINALG_TRASPOSEM

namespace linalg {
	template <typename T,typename SIZE>
	void transposeM(T ** M, T ** transposedM, const SIZE m, const SIZE n) {
		for ( SIZE i = 0 ; i < m ; ++i ) 
			for ( SIZE j = 0 ; j < n ; ++j ) transposedM[j][i] = M[i][j];
	}
}

#endif // H_LINALG_TRASPOSEM
