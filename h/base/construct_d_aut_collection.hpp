#ifndef H_BASE_CONSTRUCT_D_AUT_COLLECTION
#define H_BASE_CONSTRUCT_D_AUT_COLLECTION

//#include <stdio.h>
#include "nauty.h"
#include <vector> // std::memcpy, std::memset, std::fill

#include "../alloc.hpp"
#include "../autom/construct_automorphism_base.hpp"

namespace base {
	template <typename T,typename SIZE>
	void construct_d_aut_collection(T **& d_aut_collection,T & num_autom_base,T ** S,const SIZE num_rows_S,const SIZE num_cols_S, const T D) {
        int i,j;
        SIZE n,m;
        n = num_rows_S + num_cols_S;
        m = SETWORDSNEEDED(n);
        std::vector< std::vector<T> > automorphism_base;
        num_autom_base = 0;

        autom::construct_automorphism_base(S,num_rows_S,num_cols_S,n,m,automorphism_base,num_autom_base);

        alloc(d_aut_collection,num_autom_base,T *);

        for (i = 0; i < num_autom_base; ++i) {
            alloc(d_aut_collection[i],D,T);
            for  (j = num_rows_S; j < num_rows_S + D; ++j) {
                d_aut_collection[i][j - num_rows_S] = automorphism_base[i][j] - num_rows_S;
            }
        }
        automorphism_base.clear();
	}
}

#endif // H_BASE_CONSTRUCT_D_AUT_COLLECTION
