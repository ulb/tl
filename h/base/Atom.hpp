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

		const T dimension;
		const T rows;
		const T columns;
		T* data;
		T* vector;
		T** matrix;
		setword* cg;
		T cg_length;
		T* cg_pt;
		T* cg_end;

		Atom(const T dimension, const T rows, const T columns, T* data) :
		dimension(dimension), rows(rows), columns(columns), data(data) {

			// proxy for incremented row-first iteration
			this->vector = this->data + 3 ;

			// proxy for indexed row-first iteration
			alloc(this->matrix,rows,T*) ;
			for (size_t i = 0; i < rows; ++i) this->matrix[i] = this->vector + i * columns ;

			// compute canonical form (only needed for d-1 atoms, should skip)
	        int n(this->rows + this->columns);
	        int m(SETWORDSNEEDED(n));
			setword* cg;
	        alloc(cg, m*n, setword);
	        twolvl::canonicize(this->matrix, this->rows, this->columns, n, m, cg);
			this->cg = cg;
			this->cg_length = n*m;

			int length = cg_length * (sizeof(setword)/sizeof(uint8_t));
			uint8_t * pt = (uint8_t *) cg;
			uint8_t * end = pt + length;

			alloc(this->cg_pt, 8*length, T);
			this->cg_end = this->cg_pt + 8*length;

			T* cg_pt(this->cg_pt);
			while ( pt != end ) {
				uint8_t v = *pt;

				*(cg_pt++) = v & 0b10000000;
				*(cg_pt++) = v & 0b01000000;
				*(cg_pt++) = v & 0b00100000;
				*(cg_pt++) = v & 0b00010000;
				*(cg_pt++) = v & 0b00001000;
				*(cg_pt++) = v & 0b00000100;
				*(cg_pt++) = v & 0b00000010;
				*(cg_pt++) = v & 0b00000001;

				++pt;
			}

		}

		void teardown ( ) {
			free(this->cg_pt);
			free(this->cg);
			free(this->matrix);
			free(this->data);
		}

	} ;

}

#endif // H_BASE_ATOM
