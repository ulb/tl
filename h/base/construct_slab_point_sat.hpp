#ifndef H_BASE_CONSTRUCT_SLAB_POINT_SAT
#define H_BASE_CONSTRUCT_SLAB_POINT_SAT

#include "linalg/my_inner_prod.hpp"

namespace base {
    template <typename T,typename SIZE>
    void construct_slab_point_sat(T ** slab_points_sat,T ** ground_H,T ** slabs,const SIZE size_ground_H,const SIZE num_slabs,const T D) {
        for (SIZE i = 0; i < size_ground_H; ++i) {
            for (SIZE j = 0; j < num_slabs; ++j) {
                T s = linalg::my_inner_prod(ground_H[i],slabs[j],D);
                if (s == 0 || s == 1) slab_points_sat[i][j] = 1;
                else slab_points_sat[i][j] = 0;
            }
        }
    }
}

#endif // H_BASE_CONSTRUCT_SLAB_POINT_SAT
