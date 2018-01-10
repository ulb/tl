#ifndef H_CLOPS_FAST_DISCRETECONVEXHULL_CL
#define H_CLOPS_FAST_DISCRETECONVEXHULL_CL

#include <cstring> // std::fill
#include "alloc.hpp"
#include "array/get_ones.hpp"

namespace clops {

	template <typename T, typename W, typename SIZE, typename SP>
	void max_col_sat(T* P_ones, const SIZE P_nones, W* Q, const SIZE n, SP M) {

		std::fill(Q,Q+n,~W(0)); // fill Q with all 1's

		for (SIZE i = 0; i < P_nones; ++i) {
			auto P_indices_i = P_ones[i];
			auto M_i = M[P_indices_i];
			for (SIZE j = 0; j < n; ++j) Q[j] &= M_i[j];
		}

	}


	// Compute the discrete convex hull of a point set A
	template <typename T, typename W,typename SIZE, typename SP>
	void fast_discreteconvexhull_cl(T* A, W* B, W* C, SP sp, SP sp_t, const SIZE n_rows,const SIZE n_cols,const SIZE n_rows_64,const SIZE n_cols_64) {
		SIZE *A_ones;
		const SIZE A_nones = array::get_ones(A,n_rows,A_ones);
		clops::max_col_sat(A_ones,A_nones,B,n_cols_64,sp);
		free(A_ones);
		SIZE *B_ones;
		const SIZE B_nones = array::get_ones_64(B,n_cols_64,B_ones,n_cols);
		clops::max_col_sat(B_ones,B_nones,C,n_rows_64,sp_t);
		free(B_ones);
	}
}

#endif // H_CLOPS_FAST_DISCRETECONVEXHULL_CL
