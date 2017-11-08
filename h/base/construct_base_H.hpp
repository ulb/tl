#ifndef H_BASE_CONSTRUCT_BASE_H
#define H_BASE_CONSTRUCT_BASE_H

#include <cstdio>

#include "linalg/my_matrix_prod.hpp"

namespace base {
	template <typename T,typename SIZE>
	void construct_base_H(T ** base_H,T ** base_V,T ** Minv,const SIZE num_cols_S, const SIZE D) {
        for (SIZE i = 0; i < num_cols_S; ++i)  linalg::my_matrix_prod(Minv,base_V[i],base_H[i],D,D);
	}
}

#endif // H_BASE_CONSTRUCT_BASE_H
