#ifndef H_SIMPL_SLACK_MATRIX_SIMPLICIAL_2L
#define H_SIMPL_SLACK_MATRIX_SIMPLICIAL_2L

#include <cstring> // std::equal, std::memcpy, std::memset, std::fill

#include "../alloc.hpp"
#include "matrix_free_sum.hpp"

namespace simpl {
	template <typename T,typename SIZE>
	void slack_matrix_simplicial_2L(const SIZE h,const SIZE n, T **& M, SIZE & num_r_M, SIZE & num_c_M) {
	    int i,j;
	    T ** Id;
	    alloc(Id,h+1,T*);
	    for (i = 0; i < h+1; ++i) alloc(Id[i],h+1,T);
	    
	    linalg::generate_id_matrix(h+1,Id); // Id(k+1) is the slack matrix of the k-simplex Delta_k
	    
	    num_r_M = h+1;
	    num_c_M = h+1;
	    alloc(M,num_r_M,T*);
	    for (i = 0; i < num_r_M; ++i) {
	        alloc(M[i],num_c_M,T);
	        std::memcpy(M[i],Id[i],num_c_M * sizeof(T));
	    }
	    
	    int ** M_oplus;
	    int num_r_M_oplus,num_c_M_oplus;
	    
	    for (i = 0; i < n-1; ++i) {
	        simpl::matrix_free_sum(M,Id,num_r_M,num_c_M,h+1,h+1,M_oplus,num_r_M_oplus,num_c_M_oplus);
	        for (j = 0; j < num_r_M; ++j) free(M[j]);
	        free(M);
	        M = M_oplus;
	        num_r_M = num_r_M_oplus;
	        num_c_M = num_c_M_oplus;
	    }
	    
	    for (i = 0; i < h+1; ++i) free(Id[i]);
	    free(Id);
	}
}

#endif // H_SIMPL_SLACK_MATRIX_SIMPLICIAL_2L