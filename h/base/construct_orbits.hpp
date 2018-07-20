#ifndef H_BASE_CONSTRUCT_ORBITS
#define H_BASE_CONSTRUCT_ORBITS

#include <cstring> // std::memcpy, std::memset, std::fill

#include "mem/alloc.hpp"
#include "array/index_in_collection.hpp"
#include "array/dump.hpp"

namespace base {
    template <typename T,typename XT,typename SIZE>
    void jth_entry_orbit_i(const SIZE j, const T * const * const  base_Ht, const T img_origin,const T * const d_aut_collection_i, T * orbit_i, T * image, XT X, const T D) {
        const auto ground_H_j(X.comp[j]);
        // array::dump(ground_H_j,D);
        for (SIZE k = 1; k < D; ++k) {
            const auto * const base_Htk(base_Ht[k]);
            const auto img_origin_k(base_Htk[img_origin]);
            auto& image_k(image[k]);

            image_k = 0;
            const auto * pt(d_aut_collection_i);
            for (SIZE h = 1; h < D; ++h) image_k += ground_H_j[h] * (base_Htk[*(pt++)] - img_origin_k);
        }
        orbit_i[j] = X.list_accepted[array::index_in_collection(X.comp, image, X.compsize, D)];
    }


    template <typename T,typename XT,typename SIZE>
    void construct_orbits(T ** orbits, const SIZE num_autom_base, const T * const * const base_Ht, const T * const * const d_aut_collection,XT X, const T D) {

        T * image;
        mem::alloc(image,D);
        image[0] = 1;

        std::memcpy(orbits[0],X.list_accepted,(X.compsize) * sizeof(T));

        for (SIZE i = 1; i < num_autom_base; ++i) {
            const auto d_aut_collection_i(d_aut_collection[i]);
            const auto img_origin(d_aut_collection_i[D-1]);
            const auto orbit_i(orbits[i]);

            orbit_i[X.e1] = X.full_e1;
            SIZE j = 0;
            for (; j < X.e1; ++j) 
                jth_entry_orbit_i(j,base_Ht,img_origin,d_aut_collection_i,orbit_i,image,X,D);
            for (++j ; j < X.compsize; ++j) 
                jth_entry_orbit_i(j,base_Ht,img_origin,d_aut_collection_i,orbit_i,image,X,D);
        }
        free(image);
    }
}

#endif // H_BASE_CONSTRUCT_ORBITS
