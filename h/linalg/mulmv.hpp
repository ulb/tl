#ifndef H_LINALG_MULMV
#define H_LINALG_MULMV

#include "linalg/innerprod.hpp"

namespace linalg {
	// y = Mx
	template <typename T,typename SIZE>
	void mulmv(const T * const * const M, const T * const x, T * const y, const SIZE rows, const SIZE cols) {
	    for (SIZE i = 0; i < rows; ++i) y[i] = innerprod(M[i], x, cols);
	}
}

#endif // H_LINALG_MULMV
