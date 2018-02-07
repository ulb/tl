#ifndef H_TL_CONSTRUCT_SLACK_MATRIX
#define H_TL_CONSTRUCT_SLACK_MATRIX

#include <cstring>
#include "mem/alloc.hpp"
#include "mem/alloc_matrix.hpp"
#include "array/get_ones.hpp"
#include "array/is_equal.hpp"
#include "linalg/mulmv.hpp"
#include "st/is_subseteq.hpp"

namespace tl {

	// check maximality of zero sets of rows
	template <typename T,typename SIZE>
	bool is_maximal(T** rows, T** ith, T** end, const SIZE ncols) {
		T* rowi(*ith); // note that we work on the complement of the sets
		while ( rows != ith ) if (st::is_subseteq(*(rows++),rowi,ncols)) return false;
		++rows;
		while ( rows != end ) if (st::is_subseteq(*(rows++),rowi,ncols)) return false;
		return true;
	}

	template<typename T, typename SIZE>
	void rearrange(const SIZE D, T** S, const SIZE num_cols_S, T** S_new, const SIZE num_rows_S_new, const SIZE num_cols_S_new) {
		// rearranging rows of S_new
		// we know we will find S[0] first then S[1] then S[2] etc
		// because of how the way S_new is constructed
		T * tmp;
		mem::alloc(tmp,num_cols_S_new);
		T n_row = 0;
		for (SIZE i = 1; (i < num_rows_S_new) && (n_row < D); ++i) {
			if (array::is_equal(S_new[i]+1,S[n_row],num_cols_S)) {
				std::memcpy(tmp,S_new[i],num_cols_S_new * sizeof(T));
				std::memcpy(S_new[i],S_new[n_row+1],num_cols_S_new * sizeof(T));
				std::memcpy(S_new[n_row+1],tmp,num_cols_S_new * sizeof(T));
				++n_row;
			}
		}
		free(tmp);
	}

	template<typename T, typename SIZE>
	SIZE getmaximalrows(T** all_rows, const SIZE num_all_rows, T** S_new, const SIZE num_cols_S_new) {
		// check maximality of rows
		SIZE num_rows_S_new = 0;
		for (SIZE i = 0; i < num_all_rows; i++) {
			if (is_maximal(all_rows, all_rows+i, all_rows+num_all_rows, num_cols_S_new)) {
				std::memcpy(S_new[num_rows_S_new], all_rows[i], num_cols_S_new * sizeof(T));
				++num_rows_S_new;
			}
		}
		return num_rows_S_new;
	}

	// slack matrix construction
	template <typename T,typename SIZE>
	bool construct_slack_matrix(T ** base_H,T ** ground_H,T * A,T * B,T ** slabs,T ** S,void *& mem_S_new,T **& S_new,const SIZE size_ground_H, const SIZE num_slabs,const SIZE num_cols_S, SIZE & num_rows_S_new, SIZE & num_cols_S_new, const T D) {

		T * A_indices, * B_indices;
		T num_A_indices = array::get_ones(A,size_ground_H,A_indices);
		T num_B_indices = array::get_ones(B,num_slabs,B_indices);
		num_cols_S_new = num_cols_S + num_A_indices;

		void * mem_all_rows;
		T ** all_rows;
		mem::alloc_matrix(mem_all_rows, all_rows, 2*num_B_indices, num_cols_S_new);

		void * mem_ground;
		T ** ground;
		mem::alloc_matrix(mem_ground,ground,num_cols_S_new,D);

		std::memcpy(ground[0],ground_H[0],D);
		for (SIZE j = 0; j < num_cols_S; ++j) {
			std::memcpy(ground[1+j],base_H[j],D);
		}
		for (SIZE j = 1; j < num_A_indices; ++j) {
			std::memcpy(ground[num_cols_S+j],ground_H[A_indices[j]],D);
		}
		free(A_indices);

		SIZE num_all_rows = 0;
		for (SIZE i = 0; i < num_B_indices; ++i) {

			T *row(all_rows[num_all_rows]);

			linalg::mulmv(ground,slabs[B_indices[i]],row,num_cols_S_new,D);

			T num_ones = 0;
			for (SIZE j = 0; j < num_cols_S_new; ++j) num_ones += row[j];

			/**
			 * we test that the number of zeros in the candidate row (and its complement) are <= #vertices of P_0 = num_cols_S
			 * othewise we reject the matrix because it will be tested when the facet corresponding to that row is taken
			 * as basis
			 * If this is true for all the maximal rows, this test is true for all the rows
			 */

			if ((num_cols_S_new - num_ones) >= D) {
				if ((num_cols_S_new - num_ones) > num_cols_S) {
					free(B_indices);
					free(mem_ground);
					free(mem_all_rows);
					return false;
				}
				++num_all_rows;
			}
			if (num_ones >= D) {
				if (num_ones > num_cols_S){
					free(B_indices);
					free(mem_ground);
					free(mem_all_rows);
					return false;
				}
				T *row2(all_rows[num_all_rows]);
				for (SIZE j = 0; j < num_cols_S_new; ++j) row2[j] = 1-row[j];
				++num_all_rows;
			}
		}
		free(B_indices);
		free(mem_ground);

		mem::alloc_matrix(mem_S_new, S_new, num_all_rows, num_cols_S_new);
		num_rows_S_new = getmaximalrows(all_rows, num_all_rows, S_new, num_cols_S_new);

		free(mem_all_rows);

		rearrange(D, S, num_cols_S, S_new, num_rows_S_new, num_cols_S_new);

		return true;
	}

}

#endif // H_TL_CONSTRUCT_SLACK_MATRIX
