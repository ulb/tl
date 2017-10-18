#ifndef H_BASE_CONSTRUCT_ATOMS_CG
#define H_BASE_CONSTRUCT_ATOMS_CG

#include "nauty.h"
#include "../alloc.hpp"
#include "../twolvl/hash2size.hpp"
#include "../twolvl/canonicize.hpp"

namespace base {
	template <typename T,typename NT_T,typename SIZE>
	void construct_atoms_cg(NT_T ** atoms_cg, T *** atoms,T * atoms_hash, SIZE n_atoms, const T D) {
	    SIZE n,m;
	    SIZE num_rows, num_cols;
	    for (int i = 0; i < n_atoms; ++i) {
	        // Get the number of rows and columns of current atom
	        twolvl::hash2size(atoms_hash[i],num_rows,num_cols,D);
	        n = num_rows + num_cols;
	        m = SETWORDSNEEDED(n);
	        alloc(atoms_cg[i],m*n,NT_T);
	        twolvl::canonicize(atoms[i],num_rows,num_cols,n,m,atoms_cg[i]);
	    }
	}
}

#endif // H_BASE_CONSTRUCT_ATOMS_CG