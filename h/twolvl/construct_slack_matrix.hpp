#ifndef H_TWOLVL_CONSTRUCT_SLACK_MATRIX
#define H_TWOLVL_CONSTRUCT_SLACK_MATRIX

#include "alloc.hpp"
#include "array/get_ones.hpp"
#include "linalg/my_inner_prod.hpp"

namespace twolvl {
	// Check if the indices of zeros in all_rows[i] is a subset of all_rows[j]s
    template <typename T,typename SIZE>
    bool is_subset(T * all_rows_i,T * all_rows_j,const SIZE num_cols_S_new) {
        for (SIZE k = 0; k < num_cols_S_new; ++k) if ((all_rows_i[k] == 0) && (all_rows_j[k] != 0)) return false;
        return true;
    }

    // check maximality of zero sets of rows
    template <typename T,typename SIZE>
    bool is_maximal(T ** all_rows,const SIZE num_all_rows,const SIZE num_cols_S_new,const T i) {
		SIZE j;
		for (j = 0; j < i; ++j) if (is_subset(all_rows[i],all_rows[j],num_cols_S_new)) return false;
        for (++j; j < num_all_rows; ++j) if (is_subset(all_rows[i],all_rows[j],num_cols_S_new)) return false;
        return true;
	}

	// test function
	template <typename T,typename SIZE>
	bool accept(T ** S_new,T ** S,const SIZE num_cols_S,const T i, const T n_row) {
        for (SIZE j = 0; j < num_cols_S; ++j) if (S_new[i][j+1] != S[n_row][j]) return false;
        return true;
	}

	// slack matrix construction
	template <typename T,typename SIZE>
	void construct_slack_matrix(T ** base_H,T ** ground_H,T * A,T * B,T ** slabs,T ** S,T **& S_new,const SIZE size_ground_H, const SIZE num_slabs,const SIZE num_cols_S, SIZE & num_rows_S_new, SIZE & num_cols_S_new,const T D) {
	    SIZE i, j, k;
	    T ** all_rows;
	    alloc(all_rows,2*num_slabs,T*);

        T * A_indices, * B_indices;       
        T num_A_indices = array::get_ones(A,size_ground_H,A_indices);
        T num_B_indices = array::get_ones(B,num_slabs,B_indices);
	    num_cols_S_new = num_cols_S + num_A_indices;

	    T * temp_row;
	    alloc(temp_row,num_cols_S_new,T);
	    
	    k = 0;
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
            
            if ((num_cols_S_new - num_ones) >= D) {
                alloc(all_rows[k],num_cols_S_new,T);
                std::memcpy(all_rows[k++],temp_row,num_cols_S_new * sizeof(T));
            }
            if (num_ones >= D) {
                alloc(all_rows[k],num_cols_S_new,T);
                for (j = 0; j < num_cols_S_new; ++j) all_rows[k][j] = 1-temp_row[j];
                ++k;
            }
	    }
	    free(A_indices);
	    free(B_indices);
	    free(temp_row);
	    SIZE num_all_rows = k;
	    	    
	    num_rows_S_new = 0;
	    alloc(S_new,2*num_B_indices,T *);
	    // check maximality of rows
	    for (i = 0; i < num_all_rows; i++) {
	        if (is_maximal(all_rows,num_all_rows,num_cols_S_new,i)) {
	        	alloc(S_new[num_rows_S_new],num_cols_S_new,T);
	            std::memcpy(S_new[num_rows_S_new],all_rows[i],num_cols_S_new * sizeof(T));
	            num_rows_S_new++;
	        }
	    }
	    
	    for (i = 0; i < num_all_rows; ++i) free(all_rows[i]);
	    free(all_rows);
	    
	    // rearranging rows of S_new
	    T n_row = 0;
	    alloc(temp_row,num_cols_S_new,T);
	    for (i = n_row+1; (i < num_rows_S_new) && (n_row < D); ++i) {
	        if (accept(S_new,S,num_cols_S,i,n_row)) {
	            std::memcpy(temp_row,S_new[i],num_cols_S_new * sizeof(T));
	            std::memcpy(S_new[i],S_new[n_row+1],num_cols_S_new * sizeof(T));
	            std::memcpy(S_new[n_row+1],temp_row,num_cols_S_new * sizeof(T));
	            n_row++;
	        }
	    }
	    free(temp_row);
	}
}

#endif // H_TWOLVL_CONSTRUCT_SLACK_MATRIX