#ifndef H_BASE_CONSTRUCT_FACETS_BASE
#define H_BASE_CONSTRUCT_FACETS_BASE

#include <cstdio>
#include <cstring> // std::memcpy, std::memset, std::fill

#include "mem/alloc.hpp"
#include "array/is_equal.hpp"

namespace base {
	template <typename T,typename SIZE>
	void construct_facets_base(T ** facets_base,T & num_facets_base,T ** S,const SIZE num_rows_S, const SIZE D) {
		SIZE i,j;
		bool found;
		T * E;
		mem::alloc(E,D);
		num_facets_base = 0; // number of element currently in facets_base
		for (i = 0; i < num_rows_S; ++i) {
			if (S[i][D-1] == 1) for (j = 0; j < D-1; ++j) E[j+1] = 1 - S[i][j];
			else for (j = 0; j < D-1; ++j) E[j+1] = S[i][j];

			found = false;
			for  (j = 0; j < num_facets_base && !found; ++j) found = array::is_equal(facets_base[j],E,D);
			if (!found) {
				std::memcpy(facets_base[num_facets_base],E,D * sizeof(T));
				num_facets_base++;
			}
		}
		free(E);
	}
}

#endif // H_BASE_CONSTRUCT_FACETS_BASE
