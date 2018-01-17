#ifndef H_ALLOC_TRIANGULAR_MATRIX
#define H_ALLOC_TRIANGULAR_MATRIX

#include <cstdlib>
#include "linalg/div_ceil.hpp"

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

// m = #rows-1, strictily lower triangular matrix
template<typename SIZE>
void alloc_triangular_matrix_64(void *& mem,uint64_t **& matrix, const SIZE m) {
	const SIZE m64 = m / 64;
	const SIZE r = m % 64;
	mem = malloc(m * sizeof(uint64_t *) + (32*(m64*(m64+1))+r*(m64+1)) * sizeof(uint64_t)); // TODO should check if this is correct
	//mem = malloc(m * sizeof(uint64_t *) + m * (m64+1) * sizeof(uint64_t));
	if (mem == NULL) {
		fprintf(stderr,"run out of memory.\n");
		exit(1);
	}
	matrix = (uint64_t **)mem;
	uint64_t *data = (uint64_t*)(matrix + m);
	matrix[0] = data;
	for (SIZE i = 1; i < m; ++i) matrix[i] = matrix[i-1] + linalg::div_ceil(i-1, 64);
}

#endif // H_ALLOC_TRIANGULAR_MATRIX
