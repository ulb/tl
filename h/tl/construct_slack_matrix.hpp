#ifndef H_TL_CONSTRUCT_SLACK_MATRIX
#define H_TL_CONSTRUCT_SLACK_MATRIX

#include "mem/alloc.hpp"
#include "mem/alloc_matrix.hpp"
#include "array/get_ones.hpp"
#include "array/is_equal.hpp"
#include "linalg/innerprod.hpp"
#include "st/is_subseteq.hpp"

namespace tl {
	// Check if the indices of zeros in all_rows[i] is a subset of all_rows[j]s
	template <typename T,typename SIZE>
	bool is_subset(const T * const A, const T * const B, const SIZE n) {
		return st::is_subseteq(B, A, n); // note that we work on the complement
	}

	// check maximality of zero sets of rows
	template <typename T,typename SIZE>
	bool is_maximal(T** rows, T** ith, T** end, const SIZE ncols) {
		T* rowi = *ith;
		while ( rows != ith ) if (is_subset(rowi,*(rows++),ncols)) return false;
		++rows;
		while ( rows != end ) if (is_subset(rowi,*(rows++),ncols)) return false;
		return true;
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
		mem::alloc_matrix(mem_all_rows,all_rows,2*num_slabs,num_cols_S_new);

		T * temp_row;
		mem::alloc(temp_row,num_cols_S_new);
		SIZE num_all_rows = 0;
		for (SIZE i = 0; i < num_B_indices; ++i) {
			T num_ones = 0;
			T B_i = B_indices[i];
			const T s1 = linalg::innerprod(ground_H[0],slabs[B_i],D);
			temp_row[0] = s1;
			num_ones += s1;
			for (SIZE j = 0; j < num_cols_S; ++j) {
				const T s2 = linalg::innerprod(base_H[j],slabs[B_i],D);
				temp_row[1+j] = s2;
				num_ones += s2;
			}
			for (SIZE j = 1; j < num_A_indices; ++j) {
				const T s2 = linalg::innerprod(ground_H[A_indices[j]],slabs[B_i],D);
				temp_row[num_cols_S+j] = s2;
				num_ones += s2;
			}

			/**
			 * we test that the number of zeros in the candidate row (and its complement) are <= #vertices of P_0 = num_cols_S
			 * othewise we reject the matrix because it will be tested when the facet corresponding to that row is taken
			 * as basis
			 * If this is true for all the maximal rows, this test is true for all the rows
			 */

			if ((num_cols_S_new - num_ones) >= D) {
				if ((num_cols_S_new - num_ones) > num_cols_S) {
					free(A_indices);
					free(B_indices);
					free(temp_row);
					free(mem_all_rows);
					return false;
				}
				std::memcpy(all_rows[num_all_rows],temp_row,num_cols_S_new * sizeof(T));
				++num_all_rows;
			}
			if (num_ones >= D) {
				if (num_ones > num_cols_S){
					free(A_indices);
					free(B_indices);
					free(temp_row);
					free(mem_all_rows);
					return false;
				}
				for (SIZE j = 0; j < num_cols_S_new; ++j) all_rows[num_all_rows][j] = 1-temp_row[j];
				++num_all_rows;
			}
		}
		free(A_indices);
		free(B_indices);

		num_rows_S_new = 0;
		mem::alloc_matrix(mem_S_new,S_new,num_all_rows,num_cols_S_new);
		// check maximality of rows
		for (SIZE i = 0; i < num_all_rows; i++) {
			if (is_maximal(all_rows, all_rows+i, all_rows+num_all_rows, num_cols_S_new)) {
				std::memcpy(S_new[num_rows_S_new], all_rows[i], num_cols_S_new * sizeof(T));
				++num_rows_S_new;
			}
		}

		free(mem_all_rows);

		// rearranging rows of S_new
		T n_row = 0;
		for (SIZE i = n_row+1; (i < num_rows_S_new) && (n_row < D); ++i) {
			if (array::is_equal(S_new[i]+1,S[n_row],num_cols_S)) {
				std::memcpy(temp_row,S_new[i],num_cols_S_new * sizeof(T));
				std::memcpy(S_new[i],S_new[n_row+1],num_cols_S_new * sizeof(T));
				std::memcpy(S_new[n_row+1],temp_row,num_cols_S_new * sizeof(T));
				++n_row;
			}
		}

		free(temp_row);

		return true;
	}

}

#endif // H_TL_CONSTRUCT_SLACK_MATRIX
