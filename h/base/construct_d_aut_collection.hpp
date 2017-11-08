#ifndef H_BASE_CONSTRUCT_D_AUT_COLLECTION
#define H_BASE_CONSTRUCT_D_AUT_COLLECTION

//#include <stdio.h>
#include "nauty.h"
#include <vector> // std::memcpy, std::memset, std::fill

#include "alloc.hpp"
#include "alloc_matrix.hpp"
#include "base/construct_automorphism_base.hpp"

namespace base {
	template <typename T,typename SIZE>
	void construct_d_aut_collection(void *& mem_d_aut_collection, T **& d_aut_collection,T & num_autom_base,T ** S,const SIZE num_rows_S,const SIZE num_cols_S, const SIZE D) {
        SIZE i,j;
        SIZE n,m;
        n = num_rows_S + num_cols_S;
        m = SETWORDSNEEDED(n);
        std::vector< std::vector<T> > automorphism_base;
        num_autom_base = 0;

        base::construct_automorphism_base(S,num_rows_S,num_cols_S,n,m,automorphism_base,num_autom_base);

        alloc_matrix(mem_d_aut_collection,d_aut_collection,num_autom_base,D);

        for (i = 0; i < num_autom_base; ++i)
            for  (j = num_rows_S; j < num_rows_S + D; ++j) d_aut_collection[i][j - num_rows_S] = automorphism_base[i][j] - num_rows_S;
        automorphism_base.clear();
	}
}

#endif // H_BASE_CONSTRUCT_D_AUT_COLLECTION
