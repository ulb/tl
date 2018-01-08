#ifndef H_ADDNEW_TO_LIST
#define H_ADDNEW_TO_LIST

#include <nauty.h>
#include <stdio.h>
#include <iostream>
#include <cstring> // std::equal, std::memcpy, std::memset, std::fill

#include "../alloc.hpp"
#include "../tl/canonicize.hpp"
#include "../tl/dump.hpp"
#include "../subcl/is_susp.hpp"
#include "../subcl/is_polar.hpp"

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
	            int num_zeros, num_ones;
	            bool has_simplicial = false;
	            for (i = 0; i < num_rows_S_new && !has_simplicial; ++i) {
	                num_zeros = 0;
	                for (j = 0; j < num_cols_S_new; ++j) if (S_new[i][j] == 0) num_zeros++;
	                has_simplicial = (num_zeros == D);
	            }
	            if (has_simplicial) simplicial_facet++;
	            
	            bool STAB = false;
	            for (i = 0; i < num_cols_S_new && !STAB; ++i) {
	                num_zeros = 0;
	                for (j = 0; j < num_rows_S_new; ++j) if (S_new[j][i] == 0) num_zeros++;
	                STAB = (num_zeros == D);
	            }
	            if (STAB) {
	            	stab++;
	            	std::cout << std::endl;
	    			for (SIZE i = 0 ; i < D; ++i ) {
						for (SIZE j = 0 ; j < num_cols_S_new; ++j) std::cout << S_new[i][j];
						std::cout << std::endl;
	    			}
	            }
	            
	            bool CS = true;
	            for (i = 0; i < num_rows_S_new && CS; ++i) {
	                num_ones = 0;
	                for (j = 0; j < num_cols_S_new; ++j) if (S_new[i][j] == 1) num_ones++;
	                CS = (num_ones == num_cols_S_new/2);
	            }
	            if (CS) cs++;
	            
	            if (subcl::is_susp(S_new,num_rows_S_new,num_cols_S_new)) n_suspensions++;

	            if (subcl::is_1sum(S_new,num_rows_S_new,num_cols_S_new)) n_1_sums++;
	            
	            subcl::is_polar(S_new,hash_S_new,canonical_S_new,num_rows_S_new,num_cols_S_new,n,m,LD_hash,LD,current_LD,D,n_polar);
	        }
	    }
	    free(canonical_S_new);
	}
}

#endif // H_ADDNEW_TO_LIST
