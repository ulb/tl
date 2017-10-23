#ifndef H_BASE_CONSTRUCT_BASE_H
#define H_BASE_CONSTRUCT_BASE_H

#include <stdio.h>

#include "../alloc.hpp"
#include "../linalg/my_matrix_prod.hpp"

namespace base {
	template <typename T,typename SIZE>
	void construct_base_H(T ** base_H,T ** base_V,T ** Minv,const SIZE num_cols_S, const T D, const T verbose) {
        int i,j;
        for (i = 0; i < num_cols_S; ++i) {
            alloc(base_H[i],D,int);
            linalg::my_matrix_prod(Minv,base_V[i],base_H[i],D,D);
            if (verbose != 0) {
                // Print point
                fprintf(stderr,"[");
                for (j = 0; j < D; ++j) {
                    fprintf(stderr,"%d",base_H[i][j]);
                    if (j != D-1) fprintf(stderr,",");
                }
                fprintf(stderr,"] ");//
            }
        }
	}
}

#endif // H_BASE_CONSTRUCT_BASE_H
