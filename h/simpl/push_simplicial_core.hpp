#ifndef H_SIMPL_PUSH_SIMPLICIAL_CORE
#define H_SIMPL_PUSH_SIMPLICIAL_CORE

#include <cstring> // std::equal, std::memcpy, std::memset, std::fill
#include "mem/alloc.hpp"

namespace simpl {
	template <typename T,typename SIZE>
	void push_simplicial_core(T **& M,const SIZE num_rows_M,const SIZE num_cols_M,T **& Id,const SIZE D) {
	    int i,j,k;
	    bool accept;
	    // rearrange the columns in such a way that the first row begins with [1,0,...,0,...] (D zeros)
	    // remember that every row has precisely D zeros and D ones since it is the slack matrix of a simplicial 2-level polytope
	    T * temp_col;
		mem::alloc(temp_col,num_rows_M);
	    bool found = false;
	    if (M[0][0] != 1) {
	        for (j = 1; j < num_cols_M && !found; ++j) {
	            if (M[0][j] == 1) {
	                found = true;
	                for (i = 0; i < num_rows_M; ++i) {
	                    temp_col[i] = M[i][j];
	                    M[i][j] = M[i][0];
	                    M[i][0] = temp_col[i];
	                }
	            }
	        }
	    }
	    for (k = 1; k < D+1; ++k) {
	        found = false;
	        if (M[0][k] != 0) {
	            for (j = k+1; j < num_cols_M && !found; ++j) {
	                if (M[0][j] == 0) {
	                    found = true;
	                    for (i = 0; i < num_rows_M; ++i) {
	                        temp_col[i] = M[i][j];
	                        M[i][j] = M[i][k];
	                        M[i][k] = temp_col[i];
	                    }
	                }
	            }
	        }
	    }
	    free(temp_col);

	    // rearranging rows of M
	    T * temp_row;
		mem::alloc(temp_row,num_cols_M);
	    int temp_idx = 0;
	    for (i = temp_idx+1; i < num_rows_M && temp_idx < D; ++i) {
	        accept = true;
	        for (j = 0; j < D && accept; ++j) accept = (M[i][j+1] == Id[temp_idx][j]);

	        if (accept) {
	            std::memcpy(temp_row,M[i],num_cols_M * sizeof(T));
	            std::memcpy(M[i],M[temp_idx+1],num_cols_M * sizeof(T));
	            std::memcpy(M[temp_idx+1],temp_row,num_cols_M * sizeof(T));
	            temp_idx++;
	        }
	    }
	    free(temp_row);
	}
}

#endif // H_SIMPL_PUSH_SIMPLICIAL_CORE
