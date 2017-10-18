#ifndef H_TWOLVL_CONSTRUCT_SLACK_MATRIX
#define H_TWOLVL_CONSTRUCT_SLACK_MATRIX

#include "../alloc.hpp"
#include "../linalg/my_inner_prod.hpp"

namespace twolvl {
	// slack matrix construction
	template <typename T,typename SIZE>
	void construct_slack_matrix(T **& base_H,T **& ground_set_H,T *& A,T *& B,T **& slabs,T ** S,T **& S_new,const SIZE size_ground_H, const SIZE num_slabs,const SIZE length_A,const SIZE length_B,const SIZE num_rows_S,const SIZE num_cols_S, SIZE & num_rows_S_new, SIZE & num_cols_S_new,const T D) {
	    int i, j, k, h;
	    T ** all_rows;
	    alloc(all_rows,2*num_slabs,T*);
	    T s;
	    num_cols_S_new = num_cols_S + length_A;
	    T * temp_row;
	    alloc(temp_row,num_cols_S_new,T);
	    bool accept;
	    
	    k = 0;
	    for (i = 0; i < num_slabs; ++i) {
	        if (B[i] == 1) {
	            linalg::my_inner_prod(ground_set_H[0],slabs[i],s,D);
	            if (s != 0) temp_row[0] = 1;
	            else temp_row[0] = 0;
	            
	            for (j = 0; j < num_cols_S; ++j) {
	                linalg::my_inner_prod(base_H[j],slabs[i],s,D);
	                if (s != 0) temp_row[1+j] = 1;
	                else temp_row[1+j] = 0;
	            }
	            h = 1;
	            for (j = 1; j < size_ground_H; ++j) {
	                if (A[j] == 1) {
	                    linalg::my_inner_prod(ground_set_H[j],slabs[i],s,D);
	                    if (s != 0) temp_row[num_cols_S+h] = 1;
	                    else temp_row[num_cols_S+h] = 0;
	                    ++h;
	                }
	            }
	            
	            int num_vertices = 0;
	            for (j = 0; j < num_cols_S_new; ++j)
	                if (temp_row[j] == 0) num_vertices += 1;
	            
	            if (num_vertices >= D) {
	                alloc(all_rows[k],num_cols_S_new,T);
	                for (j = 0; j < num_cols_S_new; ++j) all_rows[k][j] = temp_row[j];
	                ++k;
	            }
	            
	            if ((num_cols_S_new - num_vertices) >= D) {
	                alloc(all_rows[k],num_cols_S_new,T);
	                for (j = 0; j < num_cols_S_new; ++j) all_rows[k][j] = 1-temp_row[j];
	                ++k;
	            }
	        }
	    }
	    free(temp_row);
	    int temp_num_all_rows = k;
	    
	    for (i = 0; i < 2*length_B; ++i) alloc(S_new[i],num_cols_S_new,T);
	    
	    num_rows_S_new = 0;
	    // check maximality of rows
	    bool is_maximal, is_subset;
	    for (i = 0; i < temp_num_all_rows; ++i) {
	        //alloc(S_new[i],num_cols_S_new,int);
	        is_maximal = true;
	        for (j = 0; j < temp_num_all_rows && is_maximal; ++j) {
	            if (j != i) {
	                is_subset = true;
	                // Check if the indices of zeros in all_rows[i] is a subset of all_rows[j]
	                for (k = 0; k < num_cols_S_new && is_subset; ++k)
	                    if (all_rows[i][k] == 0) is_subset = (all_rows[j][k] == 0);
	                is_maximal = !(is_subset);
	            }
	        }
	        if (is_maximal) {
	            std::memcpy(S_new[num_rows_S_new],all_rows[i],num_cols_S_new * sizeof(T));
	            num_rows_S_new++;
	        }
	    }
	    
	    for (i = 0; i < temp_num_all_rows; ++i) free(all_rows[i]);
	    free(all_rows);
	    
	    // rearranging rows of S_new
	    int n_row = 0;
	    alloc(temp_row,num_cols_S_new,T);
	    for (i = n_row+1; i < num_rows_S_new && n_row < D; ++i) {
	        accept = true;
	        for (j = 0; j < num_cols_S && accept; ++j)
	            accept = (S_new[i][j+1] == S[n_row][j]);
	        if (accept) {
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