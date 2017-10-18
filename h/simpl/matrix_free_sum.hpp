#ifndef H_SIMPL_MATRIX_FREE_SUM
#define H_SIMPL_MATRIX_FREE_SUM

#include "../alloc.hpp"

namespace simpl {
	template <typename T,typename SIZE>
	void matrix_free_sum(T ** P,T ** Q,const SIZE m1,const SIZE n1,const SIZE m2,const SIZE n2,T **& P_oplus_Q,SIZE & num_rows,SIZE & num_cols) {
	    int i,j,k;
	    int offset;
	    num_rows = m1 * m2;
	    num_cols = n1 + n2;
	    
	    alloc(P_oplus_Q,num_rows,T *);
	    for (i = 0; i < num_rows; ++i) alloc(P_oplus_Q[i],num_cols,T);
	    
	    offset = 0;
	    for (i = 0; i < m2; ++i) {
	        for (j = 0; j < m1; ++j)
	            for (k = 0; k < n1; ++k) P_oplus_Q[j + offset][k] = P[j][k];
	        offset += m1;
	    }
	    
	    offset = 0;
	    for (j = 0; j < m2; ++j) {
	        for (i = 0; i < m1; ++i) {
	            for (k = 0; k < n2; ++k) P_oplus_Q[offset][n1 + k] = Q[j][k];
	            offset += 1;
	        }
	    }
	}
}

#endif // H_SIMPL_MATRIX_FREE_SUM