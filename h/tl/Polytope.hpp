#ifndef H_TL_POLYTOPE
#define H_TL_POLYTOPE

#include "nauty.h"
#include "alloc.hpp"
#include "tl/canonicize.hpp"

namespace tl {

	/**
	 * this->data is used for lexicographical ordering and is layed out as follows
	 * [ dimension , rows , columns , matrix[0][0] , matrix[0][1] , ... ]
	 * this->vector is used for pointer iteration
	 * this->matrix is used for indexed iteration
	 *
	 */

	template <typename T>
	class Polytope {

	public:

		const T dimension;
		const T rows;
		const T columns;
		T* data;
		T* vector;
		T** matrix;
		setword* cg;
		setword* cg_end;

		Polytope(const T dimension, const T rows, const T columns, T* data) :
		dimension(dimension), rows(rows), columns(columns), data(data) {

			// proxy for incremented row-first iteration
			this->vector = this->data + 3 ;

			// proxy for indexed row-first iteration
			alloc(this->matrix,rows,T*) ;
			for (T i = 0; i < rows; ++i) this->matrix[i] = this->vector + i * columns ;

			// compute canonical form (only needed for d-1 atoms, should skip)
	        int n(this->rows + this->columns);
	        int m(SETWORDSNEEDED(n));
			setword* cg;
	        alloc(cg, m*n, setword);
	        tl::canonicize(this->matrix, this->rows, this->columns, n, m, cg);
			this->cg = cg;
			this->cg_end = cg + m*n;

		}

		void teardown ( ) {
			free(this->cg);
			free(this->matrix);
			free(this->data);
		}

	} ;

}

#endif // H_TL_POLYTOPE
