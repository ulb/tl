#ifndef H_ALLOC_TRIANGULAR_MATRIX
#define H_ALLOC_TRIANGULAR_MATRIX

#include <cstdlib>

// m = #rows-1, strictily lower triangular matrix 
template<typename T,typename SIZE>
void alloc_triangular_matrix(void *& mem,T **& matrix, const SIZE m) {
	mem = malloc(m * sizeof(T *) + (m*(m-1)/2) * sizeof(T));
	if (mem == NULL) {
		fprintf(stderr,"run out of memory.\n");
		exit(1);
	}
	matrix = (T **)mem;
	T * data = (T *)(matrix + m);
	matrix[0] = NULL;
	for (SIZE i = 1; i < m; ++i) matrix[i] = data + i*(i-1)/2;



}

#endif // H_ALLOC_TRIANGULAR_MATRIX
