#ifndef H_SUBCL_IS_POLAR
#define H_SUBCL_IS_POLAR

#include "alloc.hpp"
#include "alloc_matrix.hpp"
#include "array/is_equal.hpp"
#include "tl/is_listed.hpp"
#include "tl/canonicize.hpp"

namespace subcl {
	// Test if the polar of the 2-level polytope having slack-matrix S_new is a still 2_level
	template <typename T,typename NT_T,typename SIZE,typename UNSIGNED_T>
	void is_polar(T ** S_new,T hash_S_new, NT_T * canonical_S_new, SIZE num_rows_S_new,SIZE num_cols_S_new,const SIZE n, const SIZE m,T * LD_hash,NT_T ** LD,T current_LD,const T D,UNSIGNED_T & n_polar) {
	    SIZE i,j;
	    void * mem_S_T;
	    T ** S_T;
	    alloc_matrix(mem_S_T,S_T,num_cols_S_new,num_rows_S_new);
	    for (i = 0; i < num_cols_S_new; ++i)
	        for (j = 0; j < num_rows_S_new; ++j) S_T[i][j] = S_new[j][i];

	    T hash_S_T = ((num_rows_S_new-1) << D) + num_cols_S_new - 1;
	    NT_T * canonical_S_T;

	    alloc(canonical_S_T,m*n,setword);
	    tl::canonicize(S_T,num_cols_S_new,num_rows_S_new,n,m,canonical_S_T);

	    // for (i = 0; i < num_cols_S_new; ++i) free(S_T[i]);
	    free(mem_S_T);

	    if (hash_S_new == hash_S_T) {
	        if (array::is_equal(canonical_S_T,canonical_S_new,m*n)) n_polar += 1; // self-polar
	    }
	    else if (tl::is_listed(LD,LD_hash,current_LD,canonical_S_T,hash_S_T,m*n))
	        n_polar += 2;
	    free(canonical_S_T);
	}
}

#endif // H_SUBCL_IS_POLAR