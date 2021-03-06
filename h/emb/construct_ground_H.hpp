#ifndef H_EMB_CONSTRUCT_GROUND_H
#define H_EMB_CONSTRUCT_GROUND_H

#include <cstring> // std::memcpy, std::memset, std::fill
#include <utility> // std::pair
#include <vector> // std::vector

#include "linalg/mulmv.hpp"
#include "linalg/innerprod.hpp"

namespace emb {

    // The V embedding of the ground set is constructed in lexicographic order.
    // Thus, so is the H embedding of the ground set
    template <typename T,typename SIZE>
    bool accept(T ** facets_base,const SIZE num_facets_base,T * point,const SIZE D) {
        // Facet reduction of the ground set:
        // we can throw away all the points x of the ground set where we do not have x(E) in {-1,0,1}
        // for x(E) >= 0, x(E) <= 1 facet of the base, E subset of {2,...,d}
        for (SIZE i = 0; i < num_facets_base; ++i) {
            T xE = linalg::innerprod(facets_base[i]+1,point+1,D-1);
            if (xE < -1 || xE > 1) return false;
        }
        return true;
    }


	template <typename T,typename SIZE>
	SIZE construct_ground_H(T ** ground_H, T ** ground_V,const SIZE size_ground_V,T ** facets_base, SIZE * list_accepted,const SIZE num_facets_base,T ** Minv,const SIZE D) {
            SIZE size_ground_H = 0;
            for (SIZE i = 0; i < size_ground_V; ++i) {
                T* point = ground_H[size_ground_H];
                linalg::mulmv(Minv,ground_V[i],point,D,D);

                if (accept(facets_base,num_facets_base,point,D)) list_accepted[size_ground_H++] = i;
            }
            return size_ground_H;

	}
}

#endif // H_EMB_CONSTRUCT_GROUND_H
