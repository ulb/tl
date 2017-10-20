#ifndef H_CLOPS_DISCRETECONVEXHULL_CL
#define H_CLOPS_DISCRETECONVEXHULL_CL

#include <cstring> // std::fill
#include "../alloc.hpp"

namespace clops {
	// template <typename T,typename SIZE>
	// SIZE get_ones(T * char_v,const SIZE length,T *& one_indices) {
	// 	alloc(one_indices,length,T);
	// 	SIZE num_one_indices = 0;
	// 	for (SIZE i = 0; i < length; ++i) if (char_v[i] == 1) one_indices[num_one_indices++] = i;
	// 	return num_one_indices;
	// }

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
	    SIZE i, j;

	    int B_j, dchcl_i;
	    // Set all bits of B to 1
	    //std::fill(B,B+length_B,1);

	    T * A_indices, * not_A_indices, * B_indices;
	    SIZE num_A_indices = get_idx(A,length_A,A_indices,not_A_indices);
   		SIZE num_B_indices = 0;
   		alloc(B_indices,length_B,T);
	    // Intersect all sets of slabs belonging to elements of A
	    for (j = 0; j < length_B; ++j) {
	    	B_j = 1;
	    	for (i = 0; i < num_A_indices; ++i)
	    		B_j &= slab_points_sat[A_indices[i]][j];
	    	if (B_j == 1) B_indices[num_B_indices++] = j;
	    	B[j] = B_j;
	    }
	    for (i = 0; i < num_A_indices; ++i) dchcl[A_indices[i]] = 1;
	    free(A_indices);

	    // Set all bits of dchcl to 1
	    //std::fill(dchcl,dchcl+length_A,1);
	    
	    // = get_ones(B,length_B,B_indices);
	    // Intersect all sets of points belonging to elements of B
	    for (i = 0; i < length_A - num_A_indices; ++i) {
	    	dchcl_i = 1;
	    	for (j = 0; j < num_B_indices; ++j)
	    		dchcl_i &= slab_points_sat[not_A_indices[i]][B_indices[j]];
	    	dchcl[not_A_indices[i]] = dchcl_i;
	    }
	    free(not_A_indices);
	    free(B_indices);
	}
}

#endif // H_CLOPS_DISCRETECONVEXHULL_CL