#ifndef H_TWOLVL_ISTWOLEVELPOLYTOPE
#define H_TWOLVL_ISTWOLEVELPOLYTOPE

#include <nauty.h>
#include <algorithm> // std::find

#include "../alloc.hpp"

#include "canonicize.hpp"
#include "is_listed.hpp"

namespace twolvl {
	// Checks whether a given 0-1 matrix is the slack matrix of a D-dimensional 2-level polytope,
	// by using the list of (D-1)-dimensional 2-level polytopes.
	template <typename T,typename SIZE,typename NT_T>
	bool istwolevelpolytope(T ** S_new,const SIZE num_rows_S_new,const SIZE num_cols_S_new,T * atoms_hash,NT_T ** atoms_cg,const SIZE n_atoms, const T D) {
	    int i, j, k, h, l;
	    // First test: check that every column contains at least D zeros
	    // by construction, every row of S contains at least D zeros
	    int num_facets_contain;
	    for (j = 0; j < num_cols_S_new; ++j) {
	        num_facets_contain = 0;
	        for (i = 0; i < num_rows_S_new; ++i) if (S_new[i][j] == 0) num_facets_contain += 1;
	        if (num_facets_contain < D) return false;
	    }
	    
	    T ** zero_indices;
	    SIZE * num_zero_indices;
	    alloc(zero_indices,num_rows_S_new,T*);
	    alloc(num_zero_indices,num_rows_S_new,T);
	    
	    for (i = 0; i < num_rows_S_new; ++i) {
	        k = 0;
	        alloc(zero_indices[i],num_cols_S_new,T);
	        zero_indices[i][0] = 0;
	        for (j = 0; j < num_cols_S_new; ++j){
	            if (S_new[i][j] == 0) {
	                zero_indices[i][k] = j;
	                ++k;
	            }
	        }
	        num_zero_indices[i] = k;
	    }
	    
	    T ** rows_S_Fi;
	    SIZE * num_rows_S_Fi;
	    alloc(rows_S_Fi,num_rows_S_new,T*);
	    alloc(num_rows_S_Fi,num_rows_S_new,T);
	    
	    bool is_maximal, is_subset;
	    for (i = 0; i < num_rows_S_new; ++i) {
	        alloc(rows_S_Fi[i],num_rows_S_new,T);
	        l = 0; // current number of rows of S_Fi
	        for (j = 0; j < num_rows_S_new; ++j) {
	            if (j != i) {
	                // the only rows in S_Fi are the one containing the maximal sets of zeros
	                is_maximal = true;
	                for (k = 0; k < num_rows_S_new && is_maximal; ++k) {
	                    if ((k != j) && (k != i)) {
	                        // Check if the set of zeros of S_new[j][.] intersected with the one of S_new[i][.]
	                        // is a subset of the one S_new[k][.] intersected with the one of S_new[i][.]
	                        is_subset = true;
	                        for (h = 0; h < num_zero_indices[i] && is_subset; ++h) {                    
	                            if (std::find(zero_indices[j],zero_indices[j]+num_zero_indices[j],zero_indices[i][h]) != zero_indices[j]+num_zero_indices[j])
	                                is_subset = std::find(zero_indices[k],zero_indices[k]+num_zero_indices[k],zero_indices[i][h]) != zero_indices[k]+num_zero_indices[k];
	                        }
	                        is_maximal = !is_subset;
	                    }
	                }
	                if (is_maximal) {
	                    rows_S_Fi[i][l] = j;
	                    ++l;
	                }
	            }
	        }
	        num_rows_S_Fi[i] = l;
	    }
	    
	    // Go through all the rows and build the corresponding submatrix for each of them. If the input is a slack matrix,
	    // this will compute the slack matrix of the corresponding facet and test is it appears in the list L_{D-1}
	    T ** S_Fi;
	    bool found = true;
	    SIZE n,m;

	    for (i = 0; i < num_rows_S_new && found; ++i) {
	        alloc(S_Fi,num_rows_S_Fi[i],T *);
	        for (j = 0; j < num_rows_S_Fi[i]; ++j) {
	            alloc(S_Fi[j],num_zero_indices[i],T);
	            for (k = 0; k < num_zero_indices[i]; ++k)
	                S_Fi[j][k] = S_new[rows_S_Fi[i][j]][zero_indices[i][k]];
	        }
	        
	        setword * canonical_S_Fi;
	        n = num_rows_S_Fi[i] + num_zero_indices[i];
	        m = SETWORDSNEEDED(n);
	        alloc(canonical_S_Fi,m*n,setword);
	        twolvl::canonicize(S_Fi,num_rows_S_Fi[i],num_zero_indices[i],n,m,canonical_S_Fi);
	        T hash_S_Fi = ((num_zero_indices[i]-1) << D) + num_rows_S_Fi[i] - 1;
	        
	        found = twolvl::is_listed(atoms_cg,atoms_hash,n_atoms,canonical_S_Fi,hash_S_Fi,m*n);

	        // found = false;
	        // for (it = 0; it < n_atoms && !found; it++) {
	        //     hash2size(atoms_hash[it],num_rows_atom,num_cols_atom,D);
	        //     if ((num_rows_S_Fi[i] == num_rows_atom) && (num_zero_indices[i] == num_cols_atom))
	        //         found = array::is_equal(canonical_S_Fi,atoms_cg[it],m*n);
	        // }
	        
	        free(canonical_S_Fi);
	        for (j = 0; j < num_rows_S_Fi[i]; ++j) free(S_Fi[j]);
	        free(S_Fi);
	    }
	    
	    for (i = 0; i < num_rows_S_new; ++i) free(rows_S_Fi[i]);
	    free(rows_S_Fi);
	    free(num_rows_S_Fi);
	    
	    for (i = 0; i < num_rows_S_new; ++i) free(zero_indices[i]);
	    free(zero_indices);
	    free(num_zero_indices);
	    
	    return found;
	}
}

#endif // H_TWOLVL_ISTWOLEVELPOLYTOPE