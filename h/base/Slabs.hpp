#ifndef H_BASE_SLABS
#define H_BASE_SLABS

#include "mem/alloc_matrix.hpp"
#include "base/construct_slabs.hpp"

namespace base {

template<typename T, typename SIZE>
class Slabs {
	public:
		void *mem;
		T **matrix;
		SIZE rows;
		SIZE cols;

		Slabs ( const SIZE D , const SIZE num_cols_S , T **base_H) {
			// Compute the slabs: inequalities x(E) <= 1, x(E) >= 0 that are valid for the base_H
			void * mem_slabs;
			T ** slabs;
			mem::alloc_matrix(mem_slabs, slabs, 1 << D, D); // 1 << D = 2^D
			const SIZE num_slabs = base::construct_slabs(slabs, num_cols_S, base_H, D);
			this->mem = mem_slabs;
			this->matrix = slabs;
			this->rows = num_slabs;
			this->cols = D;
		}

		void teardown () {
			free(this->mem);
		}

} ;

}

#endif // H_BASE_SLABS
