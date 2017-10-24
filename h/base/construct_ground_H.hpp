#ifndef H_BASE_CONSTRUCT_GROUND_H
#define H_BASE_CONSTRUCT_GROUND_H

#include <stdio.h>
#include <cstring> // std::memcpy, std::memset, std::fill
// #include <algorithm> // std::sort
#include <utility> // std::pair
#include <vector> // std::vector


#include "../alloc.hpp"
#include "../linalg/my_matrix_prod.hpp"

namespace base {

    // The V embedding of the ground set is constructed in lexicographic order.
    // Thus, so is the H embedding of the ground set
    template <typename T,typename SIZE>
    bool accept(T ** facets_base,const SIZE num_facets_base,T * point,const T D) {
        int i,j;
        T xE;
        // Facet reduction of the ground set:
        // we can throw away all the points x of the ground set where we do not have x(E) in {-1,0,1}
        // for x(E) >= 0, x(E) <= 1 facet of the base, E subset of {2,...,d}
        for (i = 0; i < num_facets_base; ++i) {
            xE = 0;
            for (j = 1; j < D; ++j) if (facets_base[i][j] == 1) xE += point[j];
            if ((xE != -1) && (xE != 0) && (xE != 1)) return false;
        }
        return true;
    }


	template <typename T,typename SIZE>
	void construct_ground_H(T ** ground_H,SIZE & size_ground_H,T ** ground_V,const SIZE size_ground_V,T ** facets_base,const SIZE num_facets_base,T ** Minv,const T D,const T verbose) {
        int i,j;
        size_ground_H = 0;

        for (i = 0; i < size_ground_V; ++i) {
            T * point;
            alloc(point,D,T);
            alloc(ground_H[i],D,T);
            linalg::my_matrix_prod(Minv,ground_V[i],point,D,D);

            if (accept(facets_base,num_facets_base,point,D)) {
                if (verbose != 0) {
                    // Print
                    fprintf(stderr, "[");
                    for (j = 0; j < D; ++j) {
                        fprintf(stderr, "%d",point[j]);
                        if (j != D-1) fprintf(stderr,",");
                    }
                    fprintf(stderr, "] ");//
                }
                std::memcpy(ground_H[size_ground_H],point,D * sizeof(int));
                size_ground_H++;
            }
            free(point);
        }

	}
}

#endif // H_BASE_CONSTRUCT_GROUND_H
