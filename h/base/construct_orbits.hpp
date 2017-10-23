#ifndef H_BASE_CONSTRUCT_ORBITS
#define H_BASE_CONSTRUCT_ORBITS

#include <cstring> // std::memcpy, std::memset, std::fill

#include "../alloc.hpp"

namespace base {
	template <typename T,typename SIZE>
	void construct_orbits(T *** orbits,const SIZE num_autom_base,T ** base_H,T ** d_aut_collection,T ** ground_H,const SIZE size_ground_H,const T D) {
        SIZE i,j,k,h;
        fprintf(stderr, "\n");
        alloc(orbits[0],size_ground_H,T*);
        for (j = 0; j < size_ground_H; ++j) {
            alloc(orbits[0][j],D,T);
            std::memcpy(orbits[0][j],ground_H[j],D * sizeof(T));
        }

        bool found_first_non_zero,reject;

        for (i = 1; i < num_autom_base; ++i) {
            alloc(orbits[i],size_ground_H,T*);
            alloc(orbits[i][0],D,T);
            std::memcpy(orbits[i][0],ground_H[0],D * sizeof(T));

            for (j = 1; j < size_ground_H; ++j) {
                alloc(orbits[i][j],D,T);
                orbits[i][j][0] = 1;
                found_first_non_zero = false;
                reject = false;
                for (k = 1; k < D && !reject; ++k) {
                    orbits[i][j][k] = 0;
                    for (h = 1; h < D; ++h)
                        orbits[i][j][k] += ground_H[j][h]*(base_H[d_aut_collection[i][h-1]][k] - base_H[d_aut_collection[i][D-1]][k]);
                    if (orbits[i][j][k] != 0 && !found_first_non_zero) {
                        if (orbits[i][j][k] == -1) reject = true;
                        found_first_non_zero = true;
                    }
                }
                if (reject) std::memset(orbits[i][j],0,D * sizeof(T));
            }
        }

	}
}

#endif // H_BASE_CONSTRUCT_ORBITS
