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

	// template <typename T,typename SIZE>
	// SIZE get_idx(T * char_v,const SIZE length,T *& one_indices,T *& zero_indices) {
	// 	alloc(one_indices,length,T);
	// 	alloc(zero_indices,length,T);
	// 	SIZE num_one_indices = 0;
	// 	SIZE num_zero_indices = 0;
	// 	for (SIZE i = 0; i < length; ++i)
	// 		if (char_v[i] == 1) one_indices[num_one_indices++] = i;
	// 		else zero_indices[num_zero_indices++] = i;
	// 	return num_one_indices;
	// } 

	// Compute the discrete convex hull of a point set A
	template <typename T,typename SIZE>
	void discreteconvexhull_cl(T * A,T *& B,T *& dchcl,T ** slab_points_sat,const SIZE length_A,const SIZE length_B) {
	    
	    // Set all bits of B to 1
	    std::fill(B,B+length_B,1);

	    T * A_indices;//, * not_A_indices;
	    SIZE num_A_indices = get_ones(A,length_A,A_indices);
   
	    // Intersect all sets of slabs belonging to elements of A
	    for (SIZE j = 0; j < length_B; ++j) {
	    	for (SIZE i = 0; i < num_A_indices; ++i)
	    		B[j] &= slab_points_sat[A_indices[i]][j];
	    }
	    free(A_indices);

	    // Set all bits of dchcl to 1
	    std::fill(dchcl,dchcl+length_A,1);

	    T * B_indices;
	    SIZE num_B_indices = get_ones(B,length_B,B_indices);
	    
	    // Intersect all sets of points belonging to elements of B
	    for (SIZE i = 0; i < length_A; ++i) {
	    	if (A[i] == 0) {
		    	for (SIZE j = 0; j < num_B_indices; ++j)
		    		dchcl[i] &= slab_points_sat[i][B_indices[j]];
	    	}
	    }
	    free(B_indices);
	}
}

#endif // H_CLOPS_DISCRETECONVEXHULL_CL