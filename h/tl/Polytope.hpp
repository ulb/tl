#ifndef H_TL_POLYTOPE
#define H_TL_POLYTOPE

#include "alloc_matrix.hpp"
#include "linalg/transpose.hpp"

namespace tl {

	template <typename T>
	class Polytope {

	public:

		const T dimension;
		const T rows;
		const T columns;
		void* data;
		T** matrix;

		Polytope(const T dimension, const T rows, const T columns, void* data, T** matrix) :
		dimension(dimension), rows(rows), columns(columns), data(data), matrix(matrix) {}

		void teardown ( ) {
			free(this->data);
		}

		Polytope<T> dual ( ) const {
			int dimension = this->dimension ;
			int rows = this->columns ;
			int columns = this->rows ;

			void* data;
			T** matrix;
			alloc_matrix(data, matrix, rows, columns);

			linalg::transpose(this->matrix, matrix, this->rows, this->columns);

			return Polytope<T>(dimension, rows, columns, data, matrix);
		}

	} ;

}

#endif // H_TL_POLYTOPE
