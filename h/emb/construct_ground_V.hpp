#ifndef H_EMB_CONSTRUCT_GROUND_V
#define H_EMB_CONSTRUCT_GROUND_V

#include <stdio.h>
#include <cstring> // std::memcpy, std::memset, std::fill

#include "mem/alloc.hpp"

namespace emb {
	template <typename T>
	void construct_ground_V(T ** ground_V, const T D) {
        int i,j,k;
        T * count;
        mem::alloc(count,D);
        // initialize count to 0
        bool carry;

        ground_V[0][0] = 1;
        std::memset(ground_V[0]+1,0,(D-1) * sizeof(T));

        k = 1;
        for (i = D-1; i > 0; --i) {
            std::memset(count,0,D * sizeof(T));

            while (count[i] == 0) {
                ground_V[k][0] = 1;
                std::memset(ground_V[k]+1,0,(i-1) * sizeof(T));
                ground_V[k][i] = 1;
                std::memset(ground_V[k]+i+1,0,(D-i-1) * sizeof(T));

                // Extract a vector in {-1,0,1}^{D-i-1} to fill the vector
                for (j = i+1; j < D; ++j) ground_V[k][j] = count[j] - 1;
                ++k;
                // Increase counter, by performing mod-3 computation
                j = D-1;
                do {
                    carry = (count[j] == 2);
                    count[j] = (count[j] + 1) % 3;
                    j--;
                } while (carry && j >= 0);
            }
        }
        free(count);
	}
}

#endif // H_EMB_CONSTRUCT_GROUND_V
