#ifndef H_CLOPS_LEXMAX_SYMMETRIC_CL
#define H_CLOPS_LEXMAX_SYMMETRIC_CL

#include <cstring> // std::memset

#include "mem/alloc.hpp"
#include "array/get_ones.hpp"
#include "st/precedes.hpp"

namespace clops {
	template <typename T,typename SIZE>
	SIZE build_A_sym(const T * const phi, T * A_sym, const T * const A_indices, const SIZE num_A_indices,const SIZE length_A_sym, const SIZE e1, const SIZE full_e1) {
		std::memset(A_sym,0,length_A_sym * sizeof(T));
		for (SIZE j = 0; j < num_A_indices; ++j) A_sym[phi[e1+A_indices[j]]] = 1;
		return st::min_A_idx(A_sym, full_e1+1);
	}

	// compute the lexmax symmetric copy of a set A
	template<typename XT, typename T,typename SIZE>
	void lexmax_symmetric_cl(T *& A, XT& X, const T * const * const orbits, const SIZE num_autom_base) {
		const SIZE length_A = X.finalsize;
		const SIZE length_A_sym = X.fullsize;
		T * A_indices;
		const SIZE num_A_indices = array::get_ones(A,length_A,A_indices);

		T * A_sym, * A_sym_tra;
		mem::alloc(A_sym,length_A_sym);
		mem::alloc(A_sym_tra,length_A);

		A_sym_tra[0] = 1;

		for (SIZE i = 0; i < num_autom_base; ++i) {
			SIZE min_A_sym = clops::build_A_sym(orbits[i],A_sym,A_indices,num_A_indices,length_A_sym,X.e1,X.full_e1);
			
			for (SIZE j = 1; j < length_A; ++j) A_sym_tra[j] = (A_sym + min_A_sym)[X.list_accepted[j+X.e1]-X.full_e1];

			if (st::precedes(A,A_sym_tra,length_A)) {
				T* tmp(A);
				A = A_sym_tra;
				A_sym_tra = tmp;
			}
		}
		free(A_indices);
		free(A_sym);
		free(A_sym_tra);
	}


}

#endif // H_CLOPS_LEXMAX_SYMMETRIC_CL
