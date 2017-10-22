#ifndef H_BASE_CONSTRUCT_ATOM_CG
#define H_BASE_CONSTRUCT_ATOM_CG

#include "nauty.h"
#include "../alloc.hpp"
#include "../twolvl/canonicize.hpp"

namespace base {
	template <typename T,typename NT_T,typename SIZE>
	void construct_atom_cg(NT_T * atom_cg, T ** atom, const T D, const SIZE num_rows, const SIZE num_cols) {
		SIZE n = num_rows + num_cols;
		SIZE m = SETWORDSNEEDED(n);
		alloc(atom_cg,m*n,NT_T);
		twolvl::canonicize(atom,num_rows,num_cols,n,m,atom_cg);
	}
}

#endif // H_BASE_CONSTRUCT_ATOM_CG
