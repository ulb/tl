#ifndef H_BASE_CONSTRUCT_INCOMPATIBILITY_ADJM
#define H_BASE_CONSTRUCT_INCOMPATIBILITY_ADJM

#include "linalg/innerprod.hpp"

namespace base {
    template <typename T,typename SIZE>
    bool is_incompat(const T* i, const T* j, T ** facets_base, const SIZE num_facets_base, const T D) {
        for (SIZE k = 0; k < num_facets_base; ++k) {
            T s_i = linalg::innerprod(facets_base[k]+1, i+1, D-1);
            T s_j = linalg::innerprod(facets_base[k]+1, j+1, D-1);
            if (s_i * s_j == -1) return true;
        }
        return false;
    }

    template <typename T,typename SIZE>
    void construct_incompatibility_adjM(T ** incompatibility_adjM, T ** ground_H, T ** facets_base, const SIZE size_ground_H, const SIZE num_facets_base, const T D) {
        for (SIZE i = 1; i < size_ground_H; ++i) {
            for (SIZE j = 0; j < i; ++j) {
                incompatibility_adjM[i][j] = is_incompat(ground_H[i],ground_H[j],facets_base,num_facets_base,D) ? 1 : 0 ;
            }
        }
    }
}

#endif // H_BASE_CONSTRUCT_INCOMPATIBILITY_ADJM
