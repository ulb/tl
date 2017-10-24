#ifndef H_BASE_CONSTRUCT_ORBITS
#define H_BASE_CONSTRUCT_ORBITS

#include <cstring> // std::memcpy, std::memset, std::fill

#include "alloc.hpp"
#include "array/index_in_collection.hpp"

namespace base {
    template <typename T,typename SIZE>
    void construct_orbits(T ** orbits,const SIZE num_autom_base,T ** base_H,T ** d_aut_collection,T ** ground_H,const SIZE size_ground_H,const T D) {
        alloc(orbits[0],size_ground_H,T);
        for (SIZE j = 0; j < size_ground_H; ++j) orbits[0][j] = j ;

        T* image;
        alloc(image,D,T);
        image[0] = 1;

        for (SIZE i = 1; i < num_autom_base; ++i) {

            alloc(orbits[i],size_ground_H,T);
            orbits[i][0] = 0;

            for (SIZE j = 1; j < size_ground_H; ++j) {

                bool nonzero = false;

                for (SIZE k = 1; k < D; ++k) {

                    image[k] = 0;

                    for (SIZE h = 1; h < D; ++h) {
                        image[k] += ground_H[j][h]*(base_H[d_aut_collection[i][h-1]][k] - base_H[d_aut_collection[i][D-1]][k]);
                    }

                    nonzero = nonzero || (image[k] == 1) ;

                    if (!nonzero && image[k] == -1) goto skip;

                }

                // special value for not found points
                orbits[i][j] = array::index_in_collection(ground_H, image, size_ground_H,D);
                continue;

                skip: orbits[i][j] = size_ground_H;

            }

        }

        free(image);

    }
}

#endif // H_BASE_CONSTRUCT_ORBITS
