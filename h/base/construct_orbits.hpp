#ifndef H_BASE_CONSTRUCT_ORBITS
#define H_BASE_CONSTRUCT_ORBITS

#include <cstring> // std::memcpy, std::memset, std::fill

#include "mem/alloc.hpp"
#include "array/index_in_collection.hpp"

namespace base {

    template <typename T,typename SIZE>
    void construct_orbits(T ** orbits, const SIZE num_autom_base, const T * const * const base_Ht, const T * const * const d_aut_collection, const T * const * const ground_H, const SIZE size_ground_H, const T D) {

        for (SIZE j = 0; j < size_ground_H; ++j) orbits[0][j] = j ;

        T* image;
        mem::alloc(image,D);
        image[0] = 1;

        for (SIZE i = 1; i < num_autom_base; ++i) {
            const auto d_aut_collection_i(d_aut_collection[i]);
            const auto last(d_aut_collection_i[D-1]);
            const auto orbits_i(orbits[i]);

            orbits_i[0] = 0;

            for (SIZE j = 1; j < size_ground_H; ++j) {
                const auto ground_H_j(ground_H[j]);

                SIZE k;

                for (k = 1; k < D; ++k) {

                    const auto * const base_Htk(base_Ht[k]);
                    const auto lastk(base_Htk[last]);
                    auto& imagek(image[k]);

                    imagek = 0;

                    const auto * pt(d_aut_collection_i);
                    for (SIZE h = 1; h < D; ++h) {
                        imagek += ground_H_j[h]*(base_Htk[*(pt++)] - lastk);
                    }

                    if (imagek == 1) break;
                    if (imagek == -1) goto skip;

                }

                for (++k; k < D; ++k) {

                    const auto * const base_Htk(base_Ht[k]);
                    const auto lastk(base_Htk[last]);
                    auto& imagek(image[k]);

                    imagek = 0;

                    const auto * pt(d_aut_collection_i);
                    for (SIZE h = 1; h < D; ++h) {
                        imagek += ground_H_j[h]*(base_Htk[*(pt++)] - lastk);
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
