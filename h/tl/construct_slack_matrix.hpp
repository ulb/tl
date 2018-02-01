#ifndef H_TL_CONSTRUCT_SLACK_MATRIX
#define H_TL_CONSTRUCT_SLACK_MATRIX

#include "mem/alloc.hpp"
#include "mem/alloc_matrix.hpp"
#include "array/get_ones.hpp"
#include "linalg/my_inner_prod.hpp"

namespace tl {
	// Check if the indices of zeros in all_rows[i] is a subset of all_rows[j]s
	template <typename T,typename SIZE>
	bool is_subset(T * all_rows_i,T * all_rows_j,const SIZE num_cols_S_new) {
		for (SIZE k = 0; k < num_cols_S_new; ++k) if ((all_rows_i[k] == 0) && (all_rows_j[k] != 0)) return false;
		return true;
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

	// test function
	template <typename T,typename SIZE>
	bool accept(T* Sn, T* S,const SIZE d) {
		for (SIZE j = 0; j < d; ++j) if (Sn[j] != S[j]) return false;
		return true;
	}

	template <typename T>//,typename SIZE>
	void free_all(T * A_indices,T * B_indices,T * temp_row,void * mem_all_rows) {//, const SIZE num_all_rows) {
		free(A_indices);
		free(B_indices);
		free(temp_row);
		free(mem_all_rows);
	}

	// slack matrix construction
	template <typename T,typename SIZE>
	bool construct_slack_matrix(T ** base_H,T ** ground_H,T * A,T * B,T ** slabs,T ** S,void *& mem_S_new,T **& S_new,const SIZE size_ground_H, const SIZE num_slabs,const SIZE num_cols_S, SIZE & num_rows_S_new, SIZE & num_cols_S_new, const T D) {
		SIZE i, j;

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
		T num_ones, B_i,s;
		for (i = 0; i < num_B_indices; ++i) {
			num_ones = 0;
			B_i = B_indices[i];
			s = linalg::my_inner_prod(ground_H[0],slabs[B_i],D);
			temp_row[0] = s;
			num_ones += s;
			for (j = 0; j < num_cols_S; ++j) {
				s = linalg::my_inner_prod(base_H[j],slabs[B_i],D);
				temp_row[1+j] = s;
				num_ones += s;
			}
			for (j = 1; j < num_A_indices; ++j) {
				s = linalg::my_inner_prod(ground_H[A_indices[j]],slabs[B_i],D);
				temp_row[num_cols_S+j] = s;
				num_ones += s;
			}

			/**
			 * we test that the number of zeros in the candidate row (and its complement) are <= #vertices of P_0 = num_cols_S
			 * othewise we reject the matrix because it will be tested when the facet corresponding to that row is taken
			 * as basis
			 * If this is true for all the maximal rows, this test is true for all the rows
			 */

			if ((num_cols_S_new - num_ones) >= D) {
				if ((num_cols_S_new - num_ones) > num_cols_S) {
					free_all(A_indices,B_indices,temp_row,all_rows);
					return false;
				}
				std::memcpy(all_rows[num_all_rows],temp_row,num_cols_S_new * sizeof(T));
				++num_all_rows;
			}
			if (num_ones >= D) {
				if (num_ones > num_cols_S){
					free_all(A_indices,B_indices,temp_row,all_rows);
					return false;
				}
				for (j = 0; j < num_cols_S_new; ++j) all_rows[num_all_rows][j] = 1-temp_row[j];
				++num_all_rows;
			}
		}
		free(A_indices);
		free(B_indices);

		num_rows_S_new = 0;
		mem::alloc_matrix(mem_S_new,S_new,num_all_rows,num_cols_S_new);
		// check maximality of rows
		for (i = 0; i < num_all_rows; i++) {
			if (is_maximal(all_rows, all_rows+i, all_rows+num_all_rows, num_cols_S_new)) {
				std::memcpy(S_new[num_rows_S_new],all_rows[i],num_cols_S_new * sizeof(T));
				++num_rows_S_new;
			}
		}

		free(mem_all_rows);

		// rearranging rows of S_new
		T n_row = 0;
		for (i = n_row+1; (i < num_rows_S_new) && (n_row < D); ++i) {
			if (accept(S_new[i]+1,S[n_row],num_cols_S)) {
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
