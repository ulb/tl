#ifndef H_TL_POLYTOPE
#define H_TL_POLYTOPE

#include "alloc.hpp"

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

		Polytope(const T dimension, const T rows, const T columns, T* data) :
		dimension(dimension), rows(rows), columns(columns), data(data) {

			// proxy for incremented row-first iteration
			this->vector = this->data + 3 ;

			// proxy for indexed row-first iteration
			alloc(this->matrix,rows,T*) ;
			for (T i = 0; i < rows; ++i) this->matrix[i] = this->vector + i * columns ;
		}

		void teardown ( ) {
			free(this->matrix);
			free(this->data);
		}

		Polytope<T> dual ( ) const {
			int dimension = this->dimension ;
			int rows = this->columns ;
			int columns = this->rows ;

			int* data;
			alloc(data, 3 + rows * columns, int);

			int* pt(data);

			*(pt++) = dimension;
			*(pt++) = rows;
			*(pt++) = columns;
			for ( int i = 0 ; i < rows ; ++i )
				for ( int j = 0 ; j < columns ; ++j )
					*(pt++) = this->matrix[j][i];

			return Polytope<T>(dimension, rows, columns, data);
		}

	} ;

}

#endif // H_TL_POLYTOPE
