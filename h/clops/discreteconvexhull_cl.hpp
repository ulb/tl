#ifndef H_CLOPS_DISCRETECONVEXHULL_CL
#define H_CLOPS_DISCRETECONVEXHULL_CL

#include <cstring> // std::fill
#include "../alloc.hpp"

namespace clops {
	template <typename T,typename SIZE>
	SIZE get_ones(T * char_v,const SIZE length,T *& one_indices) {
		alloc(one_indices,length,T);
		SIZE num_one_indices = 0;
		for (SIZE i = 0; i < length; ++i) if (char_v[i] == 1) one_indices[num_one_indices++] = i;
		return num_one_indices;
	}

	template <typename T,typename SIZE>
	SIZE get_idx(T * char_v,const SIZE length,T *& one_indices,T *& zero_indices) {
		alloc(one_indices,length,T);
		alloc(zero_indices,length,T);
		SIZE num_one_indices = 0;
		SIZE num_zero_indices = 0;
		for (SIZE i = 0; i < length; ++i)
			if (char_v[i] == 1) one_indices[num_one_indices++] = i;
			else zero_indices[num_zero_indices++] = i;
		return num_one_indices;
	} 

	// Compute the discrete convex hull of a point set A
	template <typename T,typename SIZE>
	void discreteconvexhull_cl(T * A,T *& B,T *& dchcl,T ** slab_points_sat,const SIZE length_A,const SIZE length_B) {
	    SIZE i,j;
	    T * A_indices, * not_A_indices, * B_indices;
	    SIZE num_A_indices = get_idx(A,length_A,A_indices,not_A_indices);
   		T dchcl_i;
   		
   		std::fill(B,B+length_B,1);

   		T A_indices_i, not_A_indices_i;

   		for (i = 0; i < num_A_indices; ++i) {
   			// Set all bits of A in dchcl to 1: by definiton A is a subset of dchcl
   			A_indices_i = A_indices[i];
   			dchcl[A_indices_i] = 1;
			// auto Bj = B;
			// auto Bend = B + length_B;
			// auto slab_points_sat_ij = slab_points_sat[A_indices_i];
			// while (Bj != Bend) *(Bj++) &= *(slab_points_sat_ij++);
   			for (j = 0; j < length_B; ++j) B[j] &= slab_points_sat[A_indices_i][j];
   		}
	    free(A_indices);

	    SIZE num_B_indices = get_ones(B,length_B,B_indices);
		
	    // Intersect all sets of points belonging to elements of B
	    for (i = 0; i < length_A - num_A_indices; ++i) {
	    	not_A_indices_i = not_A_indices[i];
	    	dchcl_i = 1;
	  		// auto B_indices_j = B_indices;
			// auto B_indices_end = B_indices + num_B_indices;
			// auto slab_points_sat_i = slab_points_sat[not_A_indices_i];
			// while (B_indices_j != B_indices_end) dchcl_i &= slab_points_sat_i[*(B_indices_j++)];
	    	for (j = 0; j < num_B_indices; ++j) dchcl_i &= slab_points_sat[not_A_indices_i][B_indices[j]];
	    	dchcl[not_A_indices[i]] = dchcl_i;
	    }
	    free(not_A_indices);
	    free(B_indices);
	}
}

#endif // H_CLOPS_DISCRETECONVEXHULL_CL