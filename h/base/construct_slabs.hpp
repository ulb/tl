#ifndef H_BASE_CONSTRUCT_SLABS
#define H_BASE_CONSTRUCT_SLABS

#include <cstring> // std::memcpy, std::memset, std::fill

#include "mem/alloc.hpp"
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
    SIZE construct_slabs(T ** slabs,const SIZE num_cols_S,T ** base_H,const T D) {
        T * count;
        mem::alloc(count,D+1);
        // initialize count to e_1
        count[0] = 1;
        std::memset(count+1,0,D * sizeof(T));
        bool carry;

        SIZE num_slabs = 0;
        T * normal_vector;
        mem::alloc(normal_vector,D);
        while (count[D] == 0) {
            std::memcpy(normal_vector, count, D * sizeof(T));

            // Add normal vector of slab to the list if it contains all points of the base
            if (accept(normal_vector,base_H,num_cols_S,D)) {
                std::memcpy(slabs[num_slabs],normal_vector,D * sizeof(T));
                num_slabs++;
            }

            // Increase counter, by performing mod-2 computations
            SIZE i = 0;
            do {
                carry = (count[i] == 1);
                count[i] = (count[i] + 1) % 2;
                ++i;
            } while (carry && i <= D);
        }
        free(normal_vector);
        free(count);
        return num_slabs;
    }
}

#endif // H_BASE_CONSTRUCT_SLABS
