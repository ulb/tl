#ifndef H_BASE_CONSTRUCT_BASE_V
#define H_BASE_CONSTRUCT_BASE_V

#include <cstdio>

namespace base {
	template <typename T,typename SIZE>
	void construct_base_V(T ** base_V,T ** S,const SIZE num_cols_S, const SIZE D) {
        // Loop through all vertices
        for (SIZE i = 0; i < num_cols_S; ++i) {
            // alloc(base_V[i],D,T);
            // Create a point whose first coordinate is 0, and the others are the D-1 first bits
            // of the jth column of the slack matrix S
            T * base_V_i = base_V[i];
            base_V_i[0] = 0;
            ++base_V_i;
            for (SIZE j = 0; j < D-1; ++j) base_V_i[j] = S[j][i];
        }
	}
}

#endif // H_BASE_CONSTRUCT_BASE_V
