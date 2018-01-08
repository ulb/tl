#ifndef H_SUBCL_EXTRACT_COL
#define H_SUBCL_EXTRACT_COL

namespace subcl {
	 // take the i-th column of M without the j-th entry
	template <typename T,typename SIZE>
	void extract_col(T * M_transposed_i,T * col,const SIZE num_rows, const T j) {
		SIZE k;
		for (k = 0; k < j; ++k) col[k] = M_transposed_i[k];
		for (++k; k < num_rows; ++k) col[k-1] = M_transposed_i[k];
	}
}
#endif // H_SUBCL_EXTRACT_COL