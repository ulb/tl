#ifndef H_BASE_CONSTRUCT_FACETS_BASE
#define H_BASE_CONSTRUCT_FACETS_BASE

#include <cstdio>
#include <cstring> // std::memcpy, std::memset, std::fill

#include "mem/alloc.hpp"
#include "array/is_equal.hpp"

namespace base {
	template <typename T,typename SIZE>
	T construct_facets_base(T ** facets_base, T ** S, const SIZE num_rows_S, const SIZE D) {

		T num_facets_base = 0; // number of element currently in facets_base

		for (SIZE i = 0; i < num_rows_S; ++i) {
			T *E = facets_base[num_facets_base];
			*E = 0;
			if (S[i][D-1] == 1) for (SIZE j = 0; j < D-1; ++j) E[j+1] = 1 - S[i][j];
			else for (SIZE j = 0; j < D-1; ++j) E[j+1] = S[i][j];

			bool found = false;
			for (SIZE j = 0; j < num_facets_base && !found; ++j) found = array::is_equal(facets_base[j],E,D);

			if (!found) ++num_facets_base;

		}

		return num_facets_base;

	}
}

#endif // H_BASE_CONSTRUCT_FACETS_BASE
