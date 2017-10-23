#ifndef H_BASE_ATOM
#define H_BASE_ATOM

#include "nauty.h"
#include "alloc.hpp"
#include "twolvl/canonicize.hpp"

namespace base {

	/**
	 * this->data is used for lexicographical ordering and is layed out as follows
	 * [ dimension , rows , columns , matrix[0][0] , matrix[0][1] , ... ]
	 * this->vector is used for pointer iteration
	 * this->matrix is used for indexed iteration
	 *
	 */

	template <typename T>
	class Atom {

	public:

		typedef uint_fast8_t W;

		const T dimension;
		const T rows;
		const T columns;
		T* data;
		T* vector;
		T** matrix;
		setword* cg;
		T cg_length;
		W* xpt;
		W* xend;

		Atom(const T dimension, const T rows, const T columns, T* data) :
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
	        twolvl::canonicize(this->matrix, this->rows, this->columns, n, m, cg);
			this->cg = cg;
			this->cg_length = n*m;

			// compute bit vector representation
			int t_bytes = sizeof(T)/sizeof(uint8_t);
			uint8_t* dimension_pt = (uint8_t*) &this->dimension;
			uint8_t* rows_pt = (uint8_t*) &this->rows;
			uint8_t* columns_pt = (uint8_t*) &this->columns;
			uint8_t* const dimension_end = dimension_pt + t_bytes;
			uint8_t* const rows_end = rows_pt + t_bytes;
			uint8_t* const columns_end = columns_pt + t_bytes;

			int cg_bytes = cg_length * (sizeof(setword)/sizeof(uint8_t));
			uint8_t* cg_pt = (uint8_t *) cg;
			uint8_t* const cg_end = cg_pt + cg_bytes;

			alloc(this->xpt, 3*t_bytes + cg_bytes, W);
			this->xend = this->xpt + 3*t_bytes + cg_bytes;

			W* xpt(this->xpt);

			while ( dimension_pt != dimension_end ) *(xpt++) = *(dimension_pt++);

			while ( rows_pt != rows_end ) *(xpt++) = *(rows_pt++);

			while ( columns_pt != columns_end ) *(xpt++) = *(columns_pt++);

			while ( cg_pt != cg_end ) *(xpt++) = *(cg_pt++);

		}

		void teardown ( ) {
			free(this->xpt);
			free(this->cg);
			free(this->matrix);
			free(this->data);
		}

	} ;

}

#endif // H_BASE_ATOM
