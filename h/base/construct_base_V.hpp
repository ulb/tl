#ifndef H_BASE_CONSTRUCT_BASE_V
#define H_BASE_CONSTRUCT_BASE_V

#include <stdio.h>

#include "../alloc.hpp"

namespace base {
	template <typename T,typename SIZE>
	void construct_base_V(T ** base_V,T ** S,const SIZE num_cols_S, const T D, const T verbose) {
        int i,j;
        // Loop through all vertices
        for (i = 0; i < num_cols_S; ++i) {
            alloc(base_V[i],D,T);
            // Create a point whose first coordinate is 0, and the others are the D-1 first bits
            // of the jth column of the slack matrix S
            base_V[i][0] = 0;
            for (j = 0; j < D-1; ++j) base_V[i][j+1] = S[j][i];
            
            if (verbose != 0) {
                // Print point
                printf("[");
                for (j = 0; j < D; ++j){
                    printf("%d",base_V[i][j]);
                    if (j != D-1) printf(",");
                }
                printf("] ");//
            }
        }
	}
}

#endif // H_BASE_CONSTRUCT_BASE_V