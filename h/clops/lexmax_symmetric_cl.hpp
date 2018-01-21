#ifndef H_CLOPS_LEXMAX_SYMMETRIC_CL
#define H_CLOPS_LEXMAX_SYMMETRIC_CL

#include <cstring> // std::equal

#include "mem/alloc.hpp"
#include "array/is_equal.hpp"
#include "array/is_all_ones.hpp"
#include "array/is_all_zeros.hpp"
#include "array/get_ones.hpp"
#include "clops/precedes.hpp"
#include "linalg/div_ceil.hpp"

namespace clops {
	template <typename T,typename SIZE>
	bool is_outside_X(T* phi, T * A_sym, T * A_indices,const SIZE num_A_indices,const SIZE length_A) {
		for (SIZE j = 1; j < num_A_indices; ++j) if (phi[A_indices[j]] == length_A) return true;
		std::memset(A_sym+1,0,(length_A-1) * sizeof(T));
		for (SIZE j = 1; j < num_A_indices; ++j) A_sym[phi[A_indices[j]]] = 1;
		return false;
	}

	// compute the lexmax symmetric copy of a set A
	template <typename T,typename SIZE>
	void lexmax_symmetric_cl(T *& A, const SIZE length_A,T ** orbits,const SIZE num_autom_base) {

		if (!array::is_all_ones(A,length_A) && !array::is_all_zeros(A + 1,length_A - 1)) {

			T * A_indices;
			SIZE num_A_indices = array::get_ones(A,length_A,A_indices);

			T * A_sym;
			mem::alloc(A_sym,length_A);
			A_sym[0] = 1;

			for (SIZE i = 0; i < num_autom_base; ++i) {
				if (!clops::is_outside_X(orbits[i],A_sym,A_indices,num_A_indices,length_A)) {
					if (clops::precedes(A,A_sym,length_A)) {
						T* tmp = A;
						A = A_sym;
						A_sym = tmp;
					}
				}
			}

			free(A_sym);
			free(A_indices);

		}
	}

	template <typename T,typename SIZE>
	bool fast_is_outside_X(T* phi, uint64_t* candidate, T * ones,const SIZE nones,const SIZE n, const SIZE n64) {
		for (SIZE j = 1; j < nones; ++j) if (phi[ones[j]] == n) return true;
		candidate[0] &= uint64_t(1);
		std::memset(candidate+1, 0, (n64-1) * sizeof(T));
		for (SIZE j = 1; j < nones; ++j) {
			const auto pos = phi[ones[j]];
			const auto k = pos / 64;
			const auto l = pos % 64;
			candidate[k] |= ( uint64_t(1) << l ) ;
		}
		return false;
	}

	// compute the lexmax symmetric copy of a set A
	template <typename T,typename SIZE>
	void fast_lexmax_symmetric_cl(uint64_t *& A, const SIZE n, T ** orbits, const SIZE num_autom_base) {

		if (array::is_all_ones_64(A,n)) return ;
		if (((*A) & (~uint64_t(0) << 1)) && ( n <= 64 || array::is_all_zeros_64(A+1,n-64))) return ;

		T* ones;
		SIZE nones = array::get_ones_64(A,n,ones);

		SIZE n64(linalg::div_ceil(n,64));
		uint64_t* B;
		mem::alloc(B, n64);
		B[0] = uint64_t(1);

		for (SIZE i = 0; i < num_autom_base; ++i) {
			if (!clops::fast_is_outside_X(orbits[i], B, ones, nones, n, n64)) {
				if (clops::precedes_64(A, B, n64)) {
					uint64_t* tmp = A;
					A = B;
					B = tmp;
				}
			}
		}

		free(B);
		free(ones);

	}

}

#endif // H_CLOPS_LEXMAX_SYMMETRIC_CL
