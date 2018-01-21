#ifndef H_LINALG_INVERT
#define H_LINALG_INVERT

#include <cstring> // std::equal, std::memcpy, std::memset, std::fill

#include "mem/alloc.hpp"
#include "generate_id_matrix.hpp"

namespace linalg {
	// invert the DxD matrix M and put the result in Minv
	template <typename T>
	void invert(T ** M, T ** Minv, const T D) {
	    // the code modifies M, since we do not need it anymore
	    linalg::generate_id_matrix(D,Minv);

	    // Start Gaussian inversion
	    for (int i=0; i<D; ++i) {
	        // subtract the right multiple of the i-th row of [W | Minv] from the kth row of [W | Minv], where k > i
	        for (int k=i+1; k<D; ++k) {
	            int s = M[k][i];
	            if (s != 0) {
	                for (int j=0; j<D; ++j) {
	                    M[k][j] = M[k][j] - s * M[i][j];
	                    Minv[k][j] = Minv[k][j] - s * Minv[i][j];
	                }
	            }
	        }
	    }
	}
}

#endif // H_LINALG_INVERT
