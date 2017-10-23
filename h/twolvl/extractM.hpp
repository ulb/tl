#ifndef H_TWOLVL_EXTRACTM
#define H_TWOLVL_EXTRACTM

#include <stdio.h>

namespace twolvl {
    // extract the top (D-1)x(D-1) matrix M_{d-1} and extend it to M_d(0)
	template <typename T>
	void extractM(T ** S, T ** M, const T D, const T verbose) {
	    int i,j;
	    for (i = 0; i < D; ++i) {
	        for (j = 0; j < D; ++j)
	            if (i == 0 && j == 0) M[i][j] = 1;
	            else if (i == 0 || j == 0) M[i][j] = 0;
	            else M[i][j] = S[i-1][j-1];
	    }

        if (verbose == 2) {
            fprintf(stderr,"M_d(0) = \n");
            for (i = 0; i < D; ++i) {
                fprintf(stderr,"[");
                for (j = 0; j < D; ++j) {
                    fprintf(stderr,"%d", M[i][j]);
                    if (j != D-1) fprintf(stderr," ");
                }
                fprintf(stderr,"]\n");
            }
        }
	}
}

#endif // H_TWOLVL_EXTRACTM
