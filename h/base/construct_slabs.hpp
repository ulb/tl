#ifndef H_BASE_CONSTRUCT_SLABS
#define H_BASE_CONSTRUCT_SLABS

#include <cstring> // std::memcpy, std::memset, std::fill

#include "alloc.hpp"
#include "linalg/my_inner_prod.hpp"

namespace base {
    template <typename T,typename SIZE>
    bool accept(T * normal_vector,T ** base_H,const SIZE num_cols_S,const T D) {
        T s;
        for (SIZE j = 0; j < num_cols_S; ++j) {
            s = linalg::my_inner_prod(normal_vector,base_H[j],D);
            if ((s != 0) && (s != 1)) return false;
        }
        return true;
    }

	template <typename T,typename SIZE>
	void construct_slabs(T ** slabs,T & num_slabs,const SIZE num_cols_S,T ** base_H,const T D,const T verbose) {
        SIZE i,j;
        T * count;
        alloc(count,D+1,T);
        // initialize count to e_1
        count[0] = 1;
        std::memset(count+1,0,D * sizeof(T));
        bool carry;

        num_slabs = 0;
        T * normal_vector;
        alloc(normal_vector,D,T);
        while (count[D] == 0) {
            std::memcpy(normal_vector, count, D * sizeof(T));

            // Add normal vector of slab to the list if it contains all points of the base
            if (accept(normal_vector,base_H,num_cols_S,D)) {
                if (verbose != 0) {
                    // Print normal vector
                    for (j = 0; j < D; ++j) fprintf(stderr, "%d",normal_vector[j]);
                    fprintf(stderr, " ");
                }
                // alloc(slabs[num_slabs],D,T);
                std::memcpy(slabs[num_slabs],normal_vector,D * sizeof(T));
                num_slabs++;
            }

            // Increase counter, by performing mod-2 computations
            i = 0;
            do {
                carry = (count[i] == 1);
                count[i] = (count[i] + 1) % 2;
                ++i;
            } while (carry && i <= D);
        }
        free(normal_vector);
        free(count);
	}
}

#endif // H_BASE_CONSTRUCT_SLABS
