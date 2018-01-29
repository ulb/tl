#ifndef H_LINALG_MY_MATRIX_PROD
#define H_LINALG_MY_MATRIX_PROD

#include "linalg/my_inner_prod.hpp"

namespace linalg {
	// y = Mx
	template <typename T,typename SIZE>
	void my_matrix_prod(const T * const * const M, const T * const x, T * y, const SIZE rows, const SIZE cols) {
	    for (SIZE i = 0; i < rows; ++i) y[i] = my_inner_prod(M[i], x, cols);
	}
}

#endif // H_LINALG_MY_MATRIX_PROD
