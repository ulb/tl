#ifndef H_BASE_CONSTRUCT_ORBITS
#define H_BASE_CONSTRUCT_ORBITS

#include <cstring> // std::memcpy, std::memset, std::fill

#include "mem/alloc.hpp"
#include "array/index_in_collection.hpp"

namespace base {

    template <typename T,typename SIZE>
    void construct_orbits(T ** orbits, const SIZE num_autom_base, T ** base_H, T ** d_aut_collection, T ** ground_H, const SIZE size_ground_H, const T D) {

        for (SIZE j = 0; j < size_ground_H; ++j) orbits[0][j] = j ;

        T* image;
        mem::alloc(image,D);
        image[0] = 1;

        for (SIZE i = 1; i < num_autom_base; ++i) {
            const auto d_aut_collection_i(d_aut_collection[i]);
            const auto orbits_i(orbits[i]);

            orbits_i[0] = 0;

            for (SIZE j = 1; j < size_ground_H; ++j) {
                const auto ground_H_j(ground_H[j]);

                SIZE k;

                for (k = 1; k < D; ++k) {

                    image[k] = 0;

                    for (SIZE h = 1; h < D; ++h) {
                        image[k] += ground_H_j[h]*(base_H[d_aut_collection_i[h-1]][k] - base_H[d_aut_collection_i[D-1]][k]);
                    }

                    if (image[k] == 1) break;
                    if (image[k] == -1) goto skip;

                }

                for (++k; k < D; ++k) {

                    image[k] = 0;

                    for (SIZE h = 1; h < D; ++h) {
                        image[k] += ground_H_j[h]*(base_H[d_aut_collection_i[h-1]][k] - base_H[d_aut_collection_i[D-1]][k]);
                    }

                }

                // special value for not found points is size_ground_H
                orbits_i[j] = array::index_in_collection(ground_H, image, size_ground_H, D);
                continue;

                skip: orbits_i[j] = size_ground_H;

            }

        }

        free(image);

    }
}

#endif // H_BASE_CONSTRUCT_ORBITS
