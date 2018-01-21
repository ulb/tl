#ifndef H_ADDNEW_TO_LIST
#define H_ADDNEW_TO_LIST

#include <nauty.h>
#include <stdio.h>
#include <iostream>
#include <cstring> // std::equal, std::memcpy, std::memset, std::fill

#include "mem/alloc.hpp"
#include "tl/canonicize.hpp"
#include "tl/dump.hpp"
#include "subcl/is_susp.hpp"
#include "subcl/is_polar.hpp"

namespace addnew {
	// check if the slack matrix S is already listed in LD; if not, add it to LD //int *& LD_hash, setword **& LD
	template <typename T,typename NT_T,typename UNSIGNED_T,typename SIZE>
	void to_list(T **& S_new,SIZE & num_rows_S_new,SIZE & num_cols_S_new,T *& LD_hash,NT_T **& LD,T & current_LD, const T D, T & verbose,UNSIGNED_T & simplicial_facet,UNSIGNED_T & cs,UNSIGNED_T & stab,UNSIGNED_T & n_suspensions,UNSIGNED_T & n_polar) {
	    int i, j;
	    SIZE n, m;
	    T hash_S_new = ((num_cols_S_new-1) << D) + num_rows_S_new - 1;

	    NT_T * canonical_S_new;
	    n = num_rows_S_new + num_cols_S_new;
	    m = SETWORDSNEEDED(n);
	    alloc(canonical_S_new,m*n,NT_T);
	    tl::canonicize(S_new,num_rows_S_new,num_cols_S_new,n,m,canonical_S_new);

	    // Browse through all nonincidence graphs that have the same hash to see if one of them
	    // is isomorphic to the current nonincidence graph
	    if (!tl::is_listed(LD,LD_hash,current_LD,canonical_S_new,hash_S_new,m*n)) {
	        alloc(LD[current_LD],m*n,NT_T);
	        std::memcpy(LD[current_LD],canonical_S_new,m*n * sizeof(NT_T));
	        LD_hash[current_LD] = hash_S_new;
	        current_LD++;

			tl::dump(D,num_rows_S_new,num_cols_S_new,S_new);

	        // tests for subclasses of 2-level polytopes
	        if (verbose != 0) {
	            if (subcl::is_susp(S_new,num_rows_S_new,num_cols_S_new)) n_suspensions++;

	            if (subcl::is_1sum(S_new,num_rows_S_new,num_cols_S_new)) n_1_sums++;
	        }
	    }
	    free(canonical_S_new);
	}
}

#endif // H_ADDNEW_TO_LIST
