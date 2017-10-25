#ifndef H_LINALG_INVERTM
#define H_LINALG_INVERTM

#include <cstring> // std::equal, std::memcpy, std::memset, std::fill

#include "alloc.hpp"
#include "generate_id_matrix.hpp"

namespace linalg {
	// invert the DxD matrix M
	template <typename T>
	void invertM(T ** M, T ** Minv, const T D, const T verbose) {
	    int i,j,k;
	    // the code modifies M, since we do not need it anymore
	    
	    linalg::generate_id_matrix(D,Minv);

	    int s;
	    // Start Gaussian inversion
	    for (i=0; i<D; ++i) {
	        // subtract the right multiple of the i-th row of [W | Minv] from the kth row of [W | Minv], where k > i
	        for (k=i+1; k<D; ++k) {
	            s = M[k][i];
	            if (s != 0) {
	                for (j=0; j<D; ++j) {
	                    M[k][j] = M[k][j] - s * M[i][j];
	                    Minv[k][j] = Minv[k][j] - s * Minv[i][j];
	                }
	            }
	        }
	    }

        if (verbose == 2) {
            fprintf(stderr,"M_d^{-1}(0) = \n");
            for (i = 0; i < D; ++i) {
                fprintf(stderr,"[");
                for (j = 0; j < D; ++j) {
                    fprintf(stderr,"%d", Minv[i][j]);
                    if (j != D-1) fprintf(stderr," ");
                }
                fprintf(stderr,"]\n");
            }
        }
	}
}

#endif // H_LINALG_INVERTM
