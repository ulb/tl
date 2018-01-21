#ifndef H_CLOPS_DISCRETECONVEXHULL_CL
#define H_CLOPS_DISCRETECONVEXHULL_CL

#include <cstring> // std::fill
#include "mem/alloc.hpp"
#include "array/get_ones.hpp"

namespace clops {
	template <typename T,typename SIZE>
		SIZE get_idx(T * char_v,const SIZE length,T *& ones,T *& zeros) {
			alloc(ones,length,T);
			alloc(zeros,length,T);
			SIZE nones = 0;
			SIZE nzeros = 0;
			for (SIZE i = 0; i < length; ++i)
				if (char_v[i] == 1) ones[nones++] = i;
				else zeros[nzeros++] = i;
			return nones;
		}

	// Compute the discrete convex hull of a point set A
	template <typename T,typename SIZE>
		void discreteconvexhull_cl(T * A,T *& B,T *& dchcl,T ** slab_points_sat,const SIZE A_length,const SIZE B_length) {

			T *A_ones, *A_zeros;
			SIZE A_nones = get_idx(A,A_length,A_ones,A_zeros);

			std::fill(B,B+B_length,1);

			for (SIZE i = 0; i < A_nones; ++i) {
				// Set all bits of A in dchcl to 1: by definiton A is a subset of dchcl
				T A_indices_i = A_ones[i];
				dchcl[A_indices_i] = 1;
				// auto Bj = B;
				// auto Bend = B + B_length;
				auto slab_points_sat_i = slab_points_sat[A_indices_i];
				// while (Bj != Bend) *(Bj++) &= *(slab_points_sat_i++);
				for (SIZE j = 0; j < B_length; ++j) B[j] &= slab_points_sat_i[j];
			}
			free(A_ones);

			T *B_ones;
			SIZE B_nones = array::get_ones(B,B_length,B_ones);

			// Intersect all sets of points belonging to elements of B
			for (SIZE i = 0; i < A_length - A_nones; ++i) {
				T A_zeros_i = A_zeros[i];
				T dchcl_i = 1;
				// auto B_indices_j = B_ones;
				// auto B_indices_end = B_ones + B_nones;
				auto slab_points_sat_i = slab_points_sat[A_zeros_i];
				// while (B_indices_j != B_indices_end) dchcl_i &= slab_points_sat_i[*(B_indices_j++)];
				for (SIZE j = 0; j < B_nones; ++j) dchcl_i &= slab_points_sat_i[B_ones[j]];
				dchcl[A_zeros_i] = dchcl_i;
			}
			free(A_zeros);
			free(B_ones);
		}
}

#endif // H_CLOPS_DISCRETECONVEXHULL_CL
