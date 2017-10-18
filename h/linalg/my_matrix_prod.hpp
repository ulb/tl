#ifndef H_LINALG_MY_MATRIX_PROD
#define H_LINALG_MY_MATRIX_PROD

namespace linalg {
	// y = Mx
	template <typename T,typename SIZE>
	void my_matrix_prod(T ** M, T * x, T * y, SIZE num_rows, SIZE num_cols) {
	    int i, j;
	    for (i = 0; i < num_rows; ++i) {
	        y[i] = 0;
	        for (j = 0; j < num_cols; ++j) y[i] += M[i][j]*x[j];
	    }
	}
}

#endif // H_LINALG_MY_MATRIX_PROD