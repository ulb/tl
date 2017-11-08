#ifndef H_MEM_ALLOC_MATRIX
#define H_MEM_ALLOC_MATRIX

#include <cstdlib>

namespace mem {
	// m rows, n cols
	template<typename T,typename SIZE>
	void alloc_matrix(void *& mem,T **& matrix, const SIZE m,const SIZE n) {
		mem = malloc(m * sizeof(T *) + (m*n) * sizeof(T));
		if (mem == NULL) {
			fprintf(stderr,"run out of memory.\n");
			exit(1);
		}
		matrix = (T **)mem;
		T * data = (T *)(matrix + m);
		for (SIZE i = 0; i < m; ++i) matrix[i] = data + i*n;
	}
}

#endif // H_MEM_ALLOC_MATRIX
