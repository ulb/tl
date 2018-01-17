#ifndef H_CLOPS_LEXMAX_SYMMETRIC_CL
#define H_CLOPS_LEXMAX_SYMMETRIC_CL

#include <cstring> // std::equal

#include "alloc.hpp"
#include "array/is_equal.hpp"
#include "array/is_all_ones.hpp"
#include "array/is_all_zeros.hpp"
#include "array/get_ones.hpp"
#include "clops/precedes.hpp"
#include "linalg/div_ceil.hpp"

namespace clops {
	template <typename T,typename SIZE>
	bool is_outside_X(T* phi, T * A_sym, T * A_indices,const SIZE num_A_indices,const SIZE length_A) {
		for (SIZE j = 1; j < num_A_indices; ++j) {
			if (phi[A_indices[j]] == length_A) return true;
		}
		std::memset(A_sym+1,0,(length_A-1) * sizeof(T));
		for (SIZE j = 1; j < num_A_indices; ++j) {
			A_sym[phi[A_indices[j]]] = 1;
		}
		return false;
	}

	// compute the lexmax symmetric copy of a set A
	template <typename T,typename SIZE>
	void lexmax_symmetric_cl(T * A,T *& symcl, const SIZE length_A,T ** orbits,const SIZE num_autom_base) {

		std::memcpy(symcl,A,length_A * sizeof(T));

		if (!array::is_all_ones(A,length_A) && !array::is_all_zeros(A + 1,length_A - 1)) {

			T * A_indices;
			SIZE num_A_indices = array::get_ones(A,length_A,A_indices);

			T * A_sym;
			alloc(A_sym,length_A,T);
			A_sym[0] = 1;

			for (SIZE i = 0; i < num_autom_base; ++i) {
				if (!clops::is_outside_X(orbits[i],A_sym,A_indices,num_A_indices,length_A))
					if (clops::precedes(symcl,A_sym,length_A))
						std::memcpy(symcl,A_sym,length_A * sizeof(T));
			}

			free(A_sym);
			free(A_indices);

		}
	}
}

#endif // H_CLOPS_LEXMAX_SYMMETRIC_CL
