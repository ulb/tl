#ifndef H_SIMPL_SLACK_MATRIX_SIMPLICIAL_2L
#define H_SIMPL_SLACK_MATRIX_SIMPLICIAL_2L

#include <cstring> // std::equal, std::memcpy, std::memset, std::fill

#include "mem/alloc_matrix.hpp"
#include "simpl/matrix_free_sum.hpp"

namespace simpl {
	template <typename T,typename SIZE>
	void slack_matrix_simplicial_2L(const SIZE h,const SIZE n,void *& mem_M, T **& M, SIZE & num_r_M, SIZE & num_c_M) {
	    SIZE i;
	    void * mem_Id;
	    T ** Id;
		mem::alloc_matrix(mem_Id,Id,h+1,h+1);
	    linalg::generate_id_matrix(h+1,Id); // Id(h+1) is the slack matrix of the k-simplex Delta_k

	    num_r_M = h+1;
	    num_c_M = h+1;
		mem::alloc_matrix(mem_M,M,num_r_M,num_c_M);
	    for (i = 0; i < num_r_M; ++i) std::memcpy(M[i],Id[i],num_c_M * sizeof(T));

	    void * mem_M_oplus;
	    int ** M_oplus;
	    int num_r_M_oplus,num_c_M_oplus;

	    for (i = 0; i < n-1; ++i) {
	        simpl::matrix_free_sum(M,Id,num_r_M,num_c_M,h+1,h+1,mem_M_oplus,M_oplus,num_r_M_oplus,num_c_M_oplus);
	        free(mem_M);
	        mem_M = mem_M_oplus;
	        M = M_oplus;
	        num_r_M = num_r_M_oplus;
	        num_c_M = num_c_M_oplus;
	    }

	    free(mem_Id);
	}
}

#endif // H_SIMPL_SLACK_MATRIX_SIMPLICIAL_2L
