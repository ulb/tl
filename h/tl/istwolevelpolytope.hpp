#ifndef H_TL_ISTWOLEVELPOLYTOPE
#define H_TL_ISTWOLEVELPOLYTOPE

#include <algorithm>
#include <utility>
#include "nauty.h"
#include "alloc.hpp"
#include "tl/Polytope.hpp"
#include "array/get_zeros.hpp"

namespace tl {

	template <typename T>
	bool is_subset(T* Sj, T* Sk, T* zeros,const T* const end) {

		while ( zeros != end ) {
			if (Sj[*zeros] == 0 && Sk[*zeros] != 0) return false;
			++zeros;
		}

	    return true;

	}

	// the only rows in S_Fi are the one containing the maximal sets of zeros
	template <typename T,typename SIZE>
	bool is_maximal(T** S,T* zeros,const T* const end,const T i,const T j,const SIZE rows) {
		T* Sj = S[j];
	    for (SIZE k = 0; k < rows; ++k) {
			T* Sk = S[k];
			// Check if the set of zeros of S[j][.] intersected with the one of S[i][.]
			// is a subset of the one S[k][.] intersected with the one of S[i][.]
			if ((k != j) && (k != i) && is_subset(Sj,Sk,zeros,end)) return false;
	    }
	    return true;
	}

	// Checks whether a given 0-1 matrix is the slack matrix of a D-dimensional 2-level polytope,
	// by using the list of (D-1)-dimensional 2-level polytopes.
	template <typename C, typename A, typename P>
	bool istwolevelpolytope(C& comp, A& cgs, P& poly) {
		auto& S = poly.matrix;
		auto& rows = poly.rows;
		auto& cols = poly.columns;
		auto& D = poly.dimension;
	    int i, j, k, l;
	    // First test: check that every column contains at least D zeros
	    // by construction, every row of S contains at least D zeros
	    int num_facets_contain;
	    for (j = 0; j < cols; ++j) {
	        num_facets_contain = 0;
	        for (i = 0; i < rows; ++i) if (S[i][j] == 0) num_facets_contain += 1;
	        if (num_facets_contain < D) return false;
	    }

		int ** zero_indices;
	    int * num_zero_indices;
	    alloc(zero_indices,rows,int *);
	    alloc(num_zero_indices,rows,int);
	    for (i = 0; i < rows; ++i) num_zero_indices[i] = array::get_zeros(S[i],cols,zero_indices[i]);

	    int** rows_S_Fi;
	    int* num_rows_S_Fi;
	    alloc(rows_S_Fi,rows,int*);
	    alloc(num_rows_S_Fi,rows,int);

	    for (i = 0; i < rows; ++i) {
	        alloc(rows_S_Fi[i],rows,int);
	        l = 0; // current number of rows of S_Fi
			int* zeros = zero_indices[i];
			const int * const end = zeros + num_zero_indices[i];
	        for (j = 0; j < i; ++j)
	            if (is_maximal(S,zeros,end,i,j,rows)) rows_S_Fi[i][l++] = j;
	        for (++j; j < rows; ++j)
	            if (is_maximal(S,zeros,end,i,j,rows)) rows_S_Fi[i][l++] = j;
	        num_rows_S_Fi[i] = l;
	    }

	    // Go through all the rows and build the corresponding submatrix for each of them. If the input is a slack matrix,
	    // this will compute the slack matrix of the corresponding facet and test is it appears in the list L_{D-1}

	    bool found = true;

	    for (i = 0; i < rows && found; ++i) {
	    	const int fdimension = D - 1;
	    	const int frows = num_rows_S_Fi[i];
	    	const int fcolumns = num_zero_indices[i];
	    	int* fdata;
	        alloc(fdata,3+frows*fcolumns,int);
	        int* fpt = fdata;

			*(fpt++) = fdimension;
			*(fpt++) = frows;
			*(fpt++) = fcolumns;
	        for (j = 0; j < frows; ++j)
	            for (k = 0; k < fcolumns; ++k)
	                *(fpt++) = S[rows_S_Fi[i][j]][zero_indices[i][k]];

	        tl::Polytope<int> facet(fdimension,frows,fcolumns,fdata);
			std::pair<setword*, setword*> pair(facet.cg, facet.cg_end);

			found = std::binary_search(cgs.begin(), cgs.end(), pair, comp);

	        facet.teardown();
	    }

	    for (i = 0; i < rows; ++i) free(rows_S_Fi[i]);
	    free(rows_S_Fi);
	    free(num_rows_S_Fi);

	    for (i = 0; i < rows; ++i) free(zero_indices[i]);
	    free(zero_indices);
	    free(num_zero_indices);

	    return found;
	}
}

#endif // H_TL_ISTWOLEVELPOLYTOPE
