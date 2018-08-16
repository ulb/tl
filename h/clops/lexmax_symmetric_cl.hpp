#ifndef H_CLOPS_LEXMAX_SYMMETRIC_CL
#define H_CLOPS_LEXMAX_SYMMETRIC_CL

#include <cstring> // std::memset

#include "mem/alloc.hpp"
#include "array/get_ones.hpp"

#include "array/dump.hpp"

namespace clops {

	// check if sym is lexicographically greater than A
	// if yes, it also returns the position of the pointers a and accepted
	// accepted is a pointer to X.list_accepted  
	template <typename T>
	bool precedes(const T * A,T * & a, const T * const sym, const T * & accepted) {
	   while (a != A+1) if (*(--a) != sym[*(--accepted)]) return sym[*accepted] == 1;
	   return false;
	}

	template <typename T,typename SIZE>
	void build_A_sym(const T * const phi, T * A_sym, const T * const A_indices, const SIZE num_A_indices,const SIZE length_A_sym, const SIZE e1, const SIZE full_e1, SIZE & min_A_sym,SIZE & max_A_sym) {
		std::memset(A_sym,0,length_A_sym * sizeof(T));
		// SIZE min_A_idx(full_e1);
		min_A_sym = full_e1;
		max_A_sym = full_e1;
		for (SIZE j = 0; j < num_A_indices; ++j) {
			const SIZE candidate_idx(phi[e1+A_indices[j]]);
			A_sym[candidate_idx] = 1;
			if (candidate_idx < min_A_sym) min_A_sym = candidate_idx;
			if (candidate_idx > max_A_sym) max_A_sym = candidate_idx;
		}
		// return min_A_idx;
	}

	// compute the lexmax symmetric copy of a set A
	template<typename XT, typename T,typename SIZE>
	void lexmax_symmetric_cl(T *& A, XT& X, const T * const * const & orbits, const SIZE num_autom_base) {
		const SIZE length_A = X.finalsize;
		const SIZE length_A_sym = X.fullsize;
		T * A_indices;
		const SIZE num_A_indices = array::get_ones(A,length_A,A_indices);

		T * A_sym;
		mem::alloc(A_sym,length_A_sym);

		// array::dump(X.list_accepted,X.compsize);
		// array::dump(X.list_indices,length_A_sym);

		// SIZE old_max_A_comp(A_indices[num_A_indices-1]);
		SIZE old_max_A_full(X.list_accepted[X.e1+A_indices[num_A_indices-1]]);

		SIZE new_max_A_comp, new_max_A_full; 
		SIZE min_A_sym, max_A_sym;

		for (SIZE i = 0; i < num_autom_base; ++i) {
			clops::build_A_sym(orbits[i],A_sym,A_indices,num_A_indices,length_A_sym,X.e1,X.full_e1,min_A_sym, max_A_sym);

			const T * const sym(A_sym + min_A_sym - X.full_e1);
			new_max_A_full = max_A_sym - min_A_sym + X.full_e1;
			new_max_A_comp = X.list_indices[new_max_A_full];
			const T * accepted(X.list_accepted + new_max_A_comp);
			T * a(A + (new_max_A_comp-X.e1));

			if (new_max_A_full > old_max_A_full) {
				while (a != A+1) *(a--) = sym[*(accepted--)];				
			}
			else if (new_max_A_full == old_max_A_full) {
				if (clops::precedes(A,a,sym,accepted))
					while (a != A+1) *(a--) = sym[*(accepted--)];
			}
		}
		free(A_sym);
		free(A_indices);
	}
}

#endif // H_CLOPS_LEXMAX_SYMMETRIC_CL
