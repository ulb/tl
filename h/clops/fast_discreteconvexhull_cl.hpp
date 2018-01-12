#ifndef H_CLOPS_FAST_DISCRETECONVEXHULL_CL
#define H_CLOPS_FAST_DISCRETECONVEXHULL_CL

#include <cstring> // std::fill
#include "array/get_ones.hpp"

namespace clops {

	template <typename T, typename W, typename SIZE, typename R>
	void max_col_sat(T* rows, W* Q, R* M, const SIZE m, const SIZE n) {

		const auto Qn = Q + n;
		std::fill(Q,Qn,~W(0)); // fill Q with all 1's

		for ( SIZE i = 0 ; i < m ; ++i ) {
			auto Mij = M[rows[i]];
			auto Qj = Q;
			while (Qj != Qn) *(Qj++) &= *(Mij++);
		}

	}


	// Compute the discrete convex hull of a point set A
	template <typename T, typename W,typename SIZE, typename SP>
	void fast_discreteconvexhull_cl(T* A, W* B, W* C, SP sp, SP sp_t, const SIZE n_rows,const SIZE n_cols,const SIZE n_rows_64,const SIZE n_cols_64) {

		SIZE *A_ones;
		const SIZE A_nones = array::get_ones(A,n_rows,A_ones);
		clops::max_col_sat(A_ones,B,sp,A_nones,n_cols_64);
		free(A_ones);

		SIZE *B_ones;
		const SIZE B_nones = array::get_ones_64(B,n_cols,B_ones);
		clops::max_col_sat(B_ones,C,sp_t,B_nones,n_rows_64);
		free(B_ones);

	}
}

#endif // H_CLOPS_FAST_DISCRETECONVEXHULL_CL
