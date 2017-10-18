#ifndef H_BASE_CONSTRUCT_ORBITS
#define H_BASE_CONSTRUCT_ORBITS

#include <cstring> // std::memcpy, std::memset, std::fill

#include "../alloc.hpp"

namespace base {
	template <typename T,typename SIZE>
	void construct_orbits(T *** orbits,const SIZE num_autom_base,T ** base_H,T ** d_aut_collection,T ** ground_H,const SIZE size_ground_H,const T D) {
        int i,j,k,h;
        // printf("\n");
        alloc(orbits[0],num_autom_base,T*);
        for (j = 0; j < num_autom_base; ++j) {
            alloc(orbits[0][j],D,T);
            std::memcpy(orbits[0][j],ground_H[0],D * sizeof(T));
            // printf("[");
            // for (k = 0; k < D; ++k) {
            //     printf("%d",orbits[0][j][k]);
            //     if (k != D-1)
            //         printf(",");
            // }
            // printf("] ");
        }
        // printf("\n");
        
        bool found_first_non_zero,reject;
        
        for (i = 1; i < size_ground_H; ++i) {
            alloc(orbits[i],num_autom_base,T*);
            alloc(orbits[i][0],D,T);
            std::memcpy(orbits[i][0],ground_H[i],D * sizeof(T));
            
            // printf("[");
            // for (k = 0; k < D; ++k) {
            //     printf("%d",orbits[i][0][k]);
            //     if (k != D-1)
            //         printf(",");
            // }
            // printf("] ");
            
            for (j = 1; j < num_autom_base; ++j) {
                alloc(orbits[i][j],D,T);
                orbits[i][j][0] = 1;
                found_first_non_zero = false;
                reject = false;
                for (k = 1; k < D && !reject; ++k) {
                    orbits[i][j][k] = 0;
                    for (h = 1; h < D; ++h)
                        orbits[i][j][k] += ground_H[i][h]*(base_H[d_aut_collection[j][h-1]][k] - base_H[d_aut_collection[j][D-1]][k]);
                    if (orbits[i][j][k] != 0 && !found_first_non_zero) {
                        if (orbits[i][j][k] == -1) reject = true;
                        found_first_non_zero = true;
                    }
                }
                if (reject) std::memset(orbits[i][j],0,D * sizeof(T));
                // printf("[");
                // for (k = 0; k < D; ++k) {
                //     printf("%d",orbits[i][j][k]);
                //     if (k != D-1)
                //         printf(",");
                // }
                // printf("] ");
            }
            // printf("\n");
        }

	}
}

#endif // H_BASE_CONSTRUCT_ORBITS