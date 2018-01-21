#ifndef H_BASE_CONSTRUCT_INCOMPATIBILITY_ADJM
#define H_BASE_CONSTRUCT_INCOMPATIBILITY_ADJM

#include <cstring> // std::memcpy, std::memset, std::fill

#include "mem/alloc.hpp"
#include "linalg/my_inner_prod.hpp"

namespace base {
    template <typename T,typename SIZE>
    bool is_incompat(T ** ground_H, const T i, const T j,T ** facets_base,const SIZE num_facets_base,const T D) {
        int k,h;
        T s_i, s_j;
        for (k = 0; k < num_facets_base; ++k) {
            s_i = 0;
            s_j = 0;
            for (h = 1; h < D; ++h) {
                if (facets_base[k][h] != 0) {
                    s_i += ground_H[i][h];
                    s_j += ground_H[j][h];
                }
            }
            if (s_i * s_j == -1) return true;
        }
        return false;
    }

    template <typename T,typename SIZE>
    void construct_incompatibility_adjM(T ** incompatibility_adjM, T ** ground_H,T ** facets_base,const SIZE size_ground_H,const SIZE num_facets_base,const T D) {
        int i,j;
        for (i = 1; i < size_ground_H; ++i) {
            for (j = 0; j < i; ++j) {
                if (is_incompat(ground_H,i,j,facets_base,num_facets_base,D)) incompatibility_adjM[i][j] = 1;
                else incompatibility_adjM[i][j] = 0;
            }
        }
    }
}

#endif // H_BASE_CONSTRUCT_INCOMPATIBILITY_ADJM
