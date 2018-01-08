#ifndef H_SUBCL_IS_2SUM
#define H_SUBCL_IS_2SUM

#include <cstring>
#include "alloc.hpp"
#include "alloc_matrix.hpp"
#include "array/is_equal.hpp"
#include "linalg/transposeM.hpp"
#include "subcl/idx_in_collection.hpp"
#include "subcl/index_in_array.hpp"
#include "subcl/from_ith_row_on.hpp"
#include "subcl/extract_col.hpp"
//#include "subcl/determine_first_col_set.hpp"
#include "subcl/my_gcd.hpp"
#include "subcl/unique_row.hpp"
#include "subcl/unique_col.hpp"
#include "subcl/sum_array.hpp"
#include "subcl/generate_matrix_with_merged_rows.hpp"


namespace subcl {
	// vect2 / vect1
	template <typename T,typename SIZE>
	bool divide_monomials(T ** vect1, T ** vect2,T * vect1_mlt,T * vect2_mlt,const SIZE length_vect1, const SIZE length_vect2,const SIZE length, void *& mem_quot, T **& quot, T *& quot_mlt, SIZE & size_quot) {
		SIZE test_idx;
		size_quot = 0;
		alloc_matrix(mem_quot,quot,length_vect1,length);
		alloc(quot_mlt,length_vect1,T);
		if (length_vect1 > length_vect2) return false;
		for (SIZE i = 0; i < length_vect1; ++i) {
			test_idx = subcl::idx_in_collection(vect2,vect1[i],length_vect2,length);
			if (test_idx == length_vect2) return false;
			else if (vect2_mlt[test_idx] < vect1_mlt[i]) return false;
			else {
				if (vect1_mlt[i] < vect2_mlt[test_idx]) {
					std::memcpy(quot[size_quot],vect1[i],length * sizeof(T));
					quot_mlt[size_quot++] = vect2_mlt[test_idx] - vect1_mlt[i];
				}
			}
		}
	    return true;
	}

	template <typename T,typename SIZE>
	bool multiset_compare(T ** vect1, T ** vect2,T * vect1_mlt,T * vect2_mlt,const SIZE length_vect1, const SIZE length_vect2, const SIZE length) {
		if (length_vect1 != length_vect2) return false;
		SIZE test_idx;
		for (SIZE i = 0; i < length_vect1; ++i) {
			test_idx = subcl::idx_in_collection(vect2,vect1[i],length_vect2,length);
			if (test_idx == length_vect2) return false;
			else if (vect2_mlt[test_idx] != vect1_mlt[i]) return false;
		}
	    return true;
	}

	template <typename T,typename SIZE>
	bool is_col_partition(T ** col_bkts_i,T ** first_partition,T ** second_partition,T * col_bkts_i_mlt,T * first_partition_mlt,T * second_partition_mlt,const SIZE col_bkts_i_size,const SIZE size_first_partition,const SIZE size_second_partition,const SIZE length,bool & non_trivial_partition) {
		std::cout << "\ncurrent bucket of cols: " << std::endl;
		for (SIZE k = 0; k < col_bkts_i_size; ++k) {
			for (SIZE h = 0; h < length; ++h) std::cout << col_bkts_i[k][h];
			std::cout << "  :" << col_bkts_i_mlt[k] << std::endl;
		}

		if(subcl::multiset_compare(col_bkts_i,first_partition,col_bkts_i_mlt,first_partition_mlt,col_bkts_i_size,size_first_partition,length)) return true;
		if(subcl::multiset_compare(col_bkts_i,second_partition,col_bkts_i_mlt,second_partition_mlt,col_bkts_i_size,size_second_partition,length)) return true;
		void * mem_quot;
		T ** quot;
		T * quot_mlt;
		SIZE size_quot = 0;
		if (subcl::divide_monomials(first_partition,col_bkts_i,first_partition_mlt,col_bkts_i_mlt,size_first_partition,col_bkts_i_size,length,mem_quot,quot,quot_mlt,size_quot)) {
			if (subcl::multiset_compare(quot,second_partition,quot_mlt,second_partition_mlt,size_quot,size_second_partition,length)){
				free(mem_quot);
				free(quot_mlt);
				non_trivial_partition = true;
				return true;
			}
		}
		free(mem_quot);
		free(quot_mlt);
	    return false;
	}

	template <typename T,typename SIZE>
	bool construct_col_partition(T *** col_bkts,const SIZE idx_min,T ** col_bkts_mlt,SIZE * col_bkts_sizes,const SIZE num_unique_row,const SIZE length) {
		T ** bkt_min_degree = col_bkts[idx_min];
		T * col_bkts_mlt_min_degree = col_bkts_mlt[idx_min];
		SIZE bkt_min_degree_size = col_bkts_sizes[idx_min];
		SIZE i;
		bool second_partition_met = false;
		bool free_mem_second_partition = false;
		void * mem_quot, * mem_second_partition;
		T ** quot, ** second_partition;
		T * quot_mlt, * second_partition_mlt;
		SIZE size_quot = 0;
		SIZE size_second_partition = 0;
		bool non_trivial_partition = false;

		for (SIZE k = 0; k < bkt_min_degree_size; ++k) {
			for (SIZE h = 0; h < length; ++h) std::cout << bkt_min_degree[k][h];
			std::cout << "  :" << col_bkts_mlt_min_degree[k] << std::endl;
		}

		for (i = 0; i < idx_min; ++i) {
			if (!second_partition_met) {
				if (subcl::divide_monomials(bkt_min_degree,col_bkts[i],col_bkts_mlt_min_degree,col_bkts_mlt[i],bkt_min_degree_size,col_bkts_sizes[i],length,mem_quot,quot,quot_mlt,size_quot)) {
					if (size_quot != 0) {
						size_second_partition = size_quot;
						alloc(second_partition_mlt,size_second_partition,T);
						std::memcpy(second_partition_mlt,quot_mlt,size_second_partition * sizeof(T));
						alloc_matrix(mem_second_partition,second_partition,size_second_partition,length);
						for (SIZE k = 0; k < size_second_partition; ++k) std::memcpy(second_partition[k],quot[k],length * sizeof(T));
						second_partition_met = true;
						free_mem_second_partition = true;
						non_trivial_partition = true;
					}
				}
				else {
					second_partition = col_bkts[i];
					second_partition_mlt = col_bkts_mlt[i];
					size_second_partition = col_bkts_sizes[i];
					second_partition_met = true;
				}
				std::cout << "second partition: " << std::endl;
				for (SIZE k = 0; k < size_second_partition; ++k) {
					for (SIZE h = 0; h < length; ++h) std::cout << second_partition[k][h];
					std::cout << "  :" << second_partition_mlt[k] << std::endl;
				}
				free(mem_quot);
				free(quot_mlt);
			}
			else {
				if (!is_col_partition(col_bkts[i],bkt_min_degree,second_partition,col_bkts_mlt[i],col_bkts_mlt_min_degree,second_partition_mlt,col_bkts_sizes[i],bkt_min_degree_size,size_second_partition,length,non_trivial_partition)) {
					if (free_mem_second_partition) {
						free(mem_second_partition);
						free(second_partition_mlt);
					}
					return false;
				}
			}
		}
		//////
		for (++i ; i < num_unique_row; ++i) {
			if (!second_partition_met) {
				if (subcl::divide_monomials(bkt_min_degree,col_bkts[i],col_bkts_mlt_min_degree,col_bkts_mlt[i],bkt_min_degree_size,col_bkts_sizes[i],length,mem_quot,quot,quot_mlt,size_quot)) {
					/** if divide_monomials is true and size_quot == 0
					 * then bkt_min_degree is equal to col_bkts[i] and we did not find the second partition yet
					 */
					if (size_quot != 0) { 
						size_second_partition = size_quot;
						alloc(second_partition_mlt,size_second_partition,T);
						std::memcpy(second_partition_mlt,quot_mlt,size_second_partition * sizeof(T));
						alloc_matrix(mem_second_partition,second_partition,size_second_partition,length);
						for (SIZE k = 0; k < size_second_partition; ++k) std::memcpy(second_partition[k],quot[k],length * sizeof(T));
						second_partition_met = true;
						free_mem_second_partition = true;
						non_trivial_partition = true;
					}
				}
				else {
					second_partition = col_bkts[i];
					second_partition_mlt = col_bkts_mlt[i];
					size_second_partition = col_bkts_sizes[i];
					second_partition_met = true;
				}
				std::cout << "second partition: " << std::endl;
				for (SIZE k = 0; k < size_second_partition; ++k) {
					for (SIZE h = 0; h < length; ++h) std::cout << second_partition[k][h];
					std::cout << "  :" << second_partition_mlt[k] << std::endl;
				}
				free(mem_quot);
				free(quot_mlt);
			}
			else {
				if (!is_col_partition(col_bkts[i],bkt_min_degree,second_partition,col_bkts_mlt[i],col_bkts_mlt_min_degree,second_partition_mlt,col_bkts_sizes[i],bkt_min_degree_size,size_second_partition,length,non_trivial_partition)) {
					if (free_mem_second_partition) {
						free(mem_second_partition);
						free(second_partition_mlt);
					}
					return false;
				}
			}
		}
		if (second_partition_met && free_mem_second_partition) {
			free(mem_second_partition);
			free(second_partition_mlt);
		}
		return true && non_trivial_partition;
	}

    template <typename T,typename SIZE>
	void free_all_2sum(T * unique_row,T * unique_row_mlt,void * mem_unique_cols,void * mem_first_partition,T * first_partition_mlt,void * mem_col_bkts_mlt) {
		free(unique_row);
		free(unique_row_mlt);
		free(mem_unique_cols);
		free(mem_first_partition);
		free(unique_row_mlt);
		free(first_partition_mlt);
		free(mem_col_bkts_mlt);
	}

	template <typename T,typename SIZE>
	bool is_1dim_2sum(T ** M,const SIZE num_rows,const SIZE num_cols,SIZE & j) {
	    SIZE k,h,l;

		for (k = 0; k <num_rows ; ++k) {
			for (h = 0; h < num_cols; ++h) std::cout << "\e[93m" << M[k][h];
			std::cout << "\e[0m\n";
		}

	    void * mem_M_transposed;
	    T ** M_transposed;
	    alloc_matrix(mem_M_transposed,M_transposed,num_cols,num_rows);
	    linalg::transposeM(M,M_transposed,num_rows,num_cols);

		for (j = 0; j < num_rows; ++j) {
			T * row = M[j];
			T * unique_row;
			T * unique_row_mlt;
			SIZE num_unique_row = 0;
			alloc(unique_row,num_cols,T);
			alloc(unique_row_mlt,num_cols,T);
			subcl::unique_row(row,unique_row,unique_row_mlt,num_cols,num_unique_row);

    		SIZE idx_min = std::distance(unique_row_mlt,std::min_element(unique_row_mlt, unique_row_mlt+num_unique_row));

			for (h = 0; h < num_cols; ++h) std::cout << row[h];
			std::cout << std::endl;
			for (h = 0; h < num_unique_row; ++h) std::cout << unique_row[h];
			std::cout << std::endl;
			for (h = 0; h < num_unique_row; ++h) std::cout << "\e[96m" << unique_row_mlt[h] << "\e[0m";
			std::cout << std::endl;
			// std::cout << idx_min << std::endl;
			std::cout << std::endl;

			void * mem_unique_cols;
			T ** unique_cols;
			SIZE num_unique_col = 0;
			alloc_matrix(mem_unique_cols,unique_cols,num_cols,num_rows-1);
			
			void * mem_col_bkts_mlt;
			T ** col_bkts_mlt;
		 	alloc_matrix(mem_col_bkts_mlt,col_bkts_mlt,num_unique_row,num_cols);

			subcl::unique_col(M_transposed,num_rows,num_cols,j,row,unique_row,num_unique_row,unique_cols,num_unique_col,col_bkts_mlt);

			for (k= 0; k < num_rows - 1; ++k) {
				for (l = 0; l < num_unique_col; ++l) std::cout << unique_cols[l][k]; 
				std::cout << std::endl;
			}
			std::cout << std::endl;
			for (l = 0; l < num_unique_row; ++l) {
				std::cout << "\e[95m" << unique_row[l] << "\e[0m\n";
				for (k = 0; k < num_unique_col; ++k) std::cout << col_bkts_mlt[l][k];
				std::cout << std::endl;
			}
			std::cout << std::endl;

			
			SIZE idx_first_partition;
			void * mem_first_partition;
			T ** first_partition;
			T * first_partition_mlt;
			SIZE first_partition_size;
			subcl::determine_first_col_set(col_bkts_mlt,num_unique_row,num_rows-1,mem_first_partition,first_partition,first_partition_mlt,first_partition_size,unique_row_mlt,idx_min,idx_first_partition);

			// if (subcl::construct_col_partition(col_bkts,idx_first_partition,col_bkts_mlt,col_bkts_sizes,num_unique_row,num_rows-1)) {
			// 	subcl::free_all_2sum(unique_row,unique_row_mlt,mem_unique_cols,mem_first_partition,first_partition_mlt,mem_col_bkts_mlt);
			// 	free(mem_M_transposed);
			// 	std::cout << "SAME MULTISET\n" << std::endl;
			// 	return true;
			// }
			// subcl::free_all_2sum(unique_row,unique_row_mlt,mem_unique_cols,mem_first_partition,first_partition_mlt,mem_col_bkts_mlt);
		}
		free(mem_M_transposed);
		return false;
	}

	// test if the 2-level polytope having slack-matrix S_new is a 1_sum
	template <typename T,typename SIZE>
	bool is_2sum(T ** S_new,SIZE num_rows_S_new,SIZE num_cols_S_new) {
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
			if (subcl::is_1dim_2sum(M1,num_rows_M-i,num_cols_M,j)) {
				if (i == 0) {
					free(mem_M1);
					free(mem_M);
					// std::cout << "2-SUM FOUND\n" << std::endl;
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

#endif // H_SUBCL_IS_2SUM