#ifndef H_BASE_CONSTRUCT_D_AUT_COLLECTION
#define H_BASE_CONSTRUCT_D_AUT_COLLECTION

//#include <stdio.h>
#include "nauty.h"
#include <vector> // std::memcpy, std::memset, std::fill

#include "mem/alloc.hpp"
#include "mem/alloc_matrix.hpp"
#include "base/construct_automorphism_base.hpp"

namespace base {
	template <typename T,typename SIZE>
	T construct_d_aut_collection(void *& mem_d_aut_collection, T **& d_aut_collection,T ** S,const SIZE num_rows_S,const SIZE num_cols_S, const SIZE D) {
		const SIZE n = num_rows_S + num_cols_S;
		const SIZE m = SETWORDSNEEDED(n);
		std::vector<std::vector<T>> automorphism_base;
		T num_autom_base = base::construct_automorphism_base(S,num_rows_S,num_cols_S,n,m,automorphism_base);

		mem::alloc_matrix(mem_d_aut_collection,d_aut_collection,num_autom_base,D);

		for (SIZE i = 0; i < num_autom_base; ++i)
			for (SIZE j = num_rows_S; j < num_rows_S + D; ++j) d_aut_collection[i][j - num_rows_S] = automorphism_base[i][j] - num_rows_S;
		automorphism_base.clear();

		return num_autom_base ;
	}
}

#endif // H_BASE_CONSTRUCT_D_AUT_COLLECTION
