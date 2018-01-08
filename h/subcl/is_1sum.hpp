#ifndef H_SUBCL_IS_1SUM
#define H_SUBCL_IS_1SUM

#include <algorithm>
#include <cstring>
#include "alloc.hpp"
#include "alloc_matrix.hpp"
#include "array/is_equal.hpp"
#include "subcl/idx_in_collection.hpp"
#include "subcl/index_in_array.hpp"
#include "subcl/from_ith_row_on.hpp"
#include "subcl/extract_col.hpp"
#include "subcl/my_gcd.hpp"
#include "subcl/unique_row.hpp"
#include "subcl/free_all.hpp"
#include "subcl/generate_matrix_with_merged_rows.hpp"

namespace subcl {
	template <typename T,typename SIZE>
	bool multiset_compare_w_mlt(T ** vect1, T ** vect2,T * vect1_mlt,T * vect2_mlt,const SIZE length_vect1, const SIZE length_vect2,const T mlt1,const T mlt2, const SIZE length) {
		if (length_vect1 != length_vect2) return false;
		SIZE test_idx;
		// std::cout << "\e[96m" << mlt1 << ' ' << mlt2 << "\e[0m\n";
		for (SIZE i = 0; i < length_vect1; ++i) {
			test_idx = subcl::idx_in_collection(vect2,vect1[i],length_vect2,length);
			if (test_idx == length_vect2) return false;
			else if (vect2_mlt[test_idx]/mlt2 != vect1_mlt[i]/mlt1) return false;
		}
	    return true;
	}

	template <typename T,typename SIZE>
	bool are_same_multisets(T *** col_bkts,T * red_unique_row_mlt,T ** col_bkts_mlt,SIZE * col_bkts_sizes,const SIZE num_unique,const SIZE length) {
		T ** bkt_0 = col_bkts[0];
		T * col_bkts_mlt_0 = col_bkts_mlt[0];
		SIZE bkt_0_size = col_bkts_sizes[0];
		for (SIZE i = 1; i < num_unique; ++i)
			if (!subcl::multiset_compare_w_mlt(bkt_0,col_bkts[i],col_bkts_mlt_0,col_bkts_mlt[i],bkt_0_size,col_bkts_sizes[i],red_unique_row_mlt[0],red_unique_row_mlt[i],length))
				return false;
		return true;
	}

	template <typename T,typename SIZE>
	void reduced_mlt(T * unique_row_mlt,const SIZE num_unique,T * red_unique_row_mlt) {
		T gcd_arr = subcl::my_gcd(unique_row_mlt,num_unique);
		// std::cout << "\e[96m" << gcd_arr << "\e[0m\n";
		for (SIZE i = 0; i < num_unique; ++i) red_unique_row_mlt[i] = unique_row_mlt[i]/gcd_arr;
	}

	template <typename T,typename SIZE>
	bool is_1dim_1sum(T ** M,const SIZE num_rows,const SIZE num_cols,SIZE & j) {
	    SIZE i,k,h;

		// for (k = 0; k <num_rows ; ++k) {
		// 	for (h = 0; h < num_cols; ++h) std::cout << "\e[92m" << M[k][h];
		// 	std::cout << "\e[0m\n";
		// }

	    void * mem_M_transposed;
	    T ** M_transposed;
	    alloc_matrix(mem_M_transposed,M_transposed,num_cols,num_rows);
	    for (k = 0; k < num_cols; ++k)
	        for (h = 0; h < num_rows; ++h) M_transposed[k][h] = M[h][k];

		for (j = 0; j < num_rows; ++j) {
			T * row = M[j];
			// std::cout << "j = " << j << " - ";

			T * unique_row;
			T * unique_row_mlt;
			SIZE num_unique = 0;
			alloc(unique_row,num_cols,T);
			alloc(unique_row_mlt,num_cols,T);
			subcl::unique_row(row,unique_row,unique_row_mlt,num_cols,num_unique);
			// for (h = 0; h < num_cols; ++h) std::cout << row[h];
			// std::cout << "   ";
			// for (h = 0; h < num_unique; ++h) std::cout << unique_row[h] << ' ';
			// std::cout << std::endl;
			// for (h = 0; h < num_unique; ++h) std::cout << "\e[96m" << unique_row_mlt[h] << "\e[0m\n";
			// std::cout << std::endl;

			T * red_unique_row_mlt;
			alloc(red_unique_row_mlt,num_cols,T);
			subcl::reduced_mlt(unique_row_mlt,num_unique,red_unique_row_mlt);
			free(unique_row_mlt);

			// for (h = 0; h < num_unique; ++h) std::cout << "\e[96m" << red_unique_row_mlt[h] << "\e[0m\n";
			// std::cout << std::endl;

			void ** mem_col_bkts;
			T *** col_bkts;
			void * mem_col_bkts_mlt;
			T ** col_bkts_mlt;
			SIZE * col_bkts_sizes;
		 	alloc(mem_col_bkts,num_unique,void*);
		 	alloc_matrix(mem_col_bkts_mlt,col_bkts_mlt,num_unique,num_cols);
			alloc(col_bkts,num_unique,T **);
			alloc(col_bkts_sizes,num_unique,T);
			std::memset(col_bkts_sizes,0,num_unique * sizeof(T));
			for (h = 0; h < num_unique; ++h) alloc_matrix(mem_col_bkts[h],col_bkts[h],num_cols,num_rows-1);
			SIZE idx, test_idx, col_bkts_sizes_j;
			T * col;
			alloc(col,num_rows-1,T);
			for (i = 0; i < num_cols; ++i) {
				subcl::extract_col(M_transposed[i],col,num_rows,j); // extract column i (all rows but the j-th)
				// for (SIZE h = 0; h < num_rows-1; ++h) std::cout << col[h];
				// std::cout << std::endl;
				idx = subcl::index_in_array(unique_row,row[i],num_unique);
				
				col_bkts_sizes_j = col_bkts_sizes[idx];
				test_idx = subcl::idx_in_collection(col_bkts[idx],col,col_bkts_sizes_j,num_rows-1);
				if (test_idx == col_bkts_sizes_j) {
					col_bkts_mlt[idx][col_bkts_sizes_j] = 1;
					std::memcpy(col_bkts[idx][col_bkts_sizes_j],col,(num_rows-1) * sizeof(T));
					col_bkts_sizes[idx]++;
				}
				else col_bkts_mlt[idx][test_idx]++;
			}
			free(col);
			// std::cout << std::endl;
			// for (SIZE l = 0; l < num_unique; ++l) {
			// 	std::cout << unique_row[l] << std::endl;
			// 	for (SIZE k = 0; k < col_bkts_sizes[l]; ++k) {
			// 		for (SIZE h = 0; h < num_rows-1; ++h) std::cout << col_bkts[l][k][h];
			// 		std::cout << "  :" << col_bkts_mlt[l][k] << std::endl;
			// 	}
			// 	std::cout << std::endl;
			// }
			if (subcl::are_same_multisets(col_bkts,red_unique_row_mlt,col_bkts_mlt,col_bkts_sizes,num_unique,num_rows-1)) {
				subcl::free_all(unique_row,red_unique_row_mlt,col_bkts,col_bkts_sizes,mem_col_bkts,num_unique,mem_col_bkts_mlt);
				free(mem_M_transposed);
				// std::cout << "SAME MULTISET\n" << std::endl;
				return true;
			}
			subcl::free_all(unique_row,red_unique_row_mlt,col_bkts,col_bkts_sizes,mem_col_bkts,num_unique,mem_col_bkts_mlt);
		}
		free(mem_M_transposed);
		return false;
	}

	// test if the 2-level polytope having slack-matrix S_new is a 1_sum
	template <typename T,typename SIZE>
	bool is_1sum(T ** S_new,SIZE num_rows_S_new,SIZE num_cols_S_new) {
		SIZE i,j;
		void * mem_M;
		T ** M;
		alloc_matrix(mem_M,M,num_rows_S_new,num_cols_S_new);
		// std::cout << "---------------------------------------" << std::endl;
		for (i = 0; i < num_rows_S_new; ++i) std::memcpy(M[i],S_new[i],num_cols_S_new * sizeof(T));
		SIZE num_rows_M = num_rows_S_new;
		SIZE num_cols_M = num_cols_S_new;
		i = 0;
		while (i < (num_rows_M - 2)) {
			void * mem_M1;
			T ** M1;
			alloc_matrix(mem_M1,M1,num_rows_M-i,num_cols_M);
			subcl::from_ith_row_on(M1,M,num_rows_M,num_cols_M,i);
			if (subcl::is_1dim_1sum(M1,num_rows_M-i,num_cols_M,j)) {
				if (i == 0) {
					free(mem_M1);
					free(mem_M);
					// std::cout << "1-SUM FOUND\n" << std::endl;
					return true;
				}
				else {
					void * mem_M_out;
					T ** M_out;
					alloc_matrix(mem_M_out,M_out,num_rows_M-1,num_cols_M);
					// std::cout << "\e[95m" << i-1 << ' ' << i+j << "\e[0m\n";
					subcl::generate_matrix_with_merged_rows(M_out,M,num_rows_M,num_cols_M,i-1,i+j);
					free(mem_M);
			        mem_M = mem_M_out;
			        M = M_out;
					--num_rows_M;
				}
				i = 0;
			}
			else ++i;
			free(mem_M1);
		}
		free(mem_M);
		return false;
	}
}

#endif // H_SUBCL_IS_1SUM