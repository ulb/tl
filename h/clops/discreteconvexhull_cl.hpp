#ifndef H_CLOPS_DISCRETECONVEXHULL_CL
#define H_CLOPS_DISCRETECONVEXHULL_CL

#include <cstring> // std::equal

namespace clops {
	// Compute the discrete convex hull of a point set A
	template <typename T,typename SIZE>
	void discreteconvexhull_cl(T * A,T *& B,T *& dchcl,T ** slab_points_sat,const SIZE length_A,const SIZE length_B) {
	    int i, j;
	    
	    // Set all bits of B to 1
	    std::fill(B,B+length_B,1);
	    
	    // Intersect all sets of slabs belonging to elements of A
	    for (i = 0; i < length_A; ++i) {
	        if (A[i] == 1) {
	            for (j = 0; j < length_B; ++j)
	                if (slab_points_sat[i][j] == 0)  B[j] = 0;
	        }
	    }    
	    // Set all bits of dchcl to 1
	    std::fill(dchcl,dchcl+length_A,1);
	    
	    // Intersect all sets of points belonging to elements of B
	    for (i = 0; i < length_B; ++i) {
	        if (B[i] == 1) {
	            for (j = 0; j < length_A; ++j)
	                if (slab_points_sat[j][i] == 0) dchcl[j] = 0;
	        }
	    }
	}
}

#endif // H_CLOPS_DISCRETECONVEXHULL_CL