#ifndef H_TL_DUMP
#define H_TL_DUMP

#include <iostream>

namespace tl {

	template <typename O, typename P>
	void dump(O& ostream, P& polytope) {

		auto& dimension = polytope.dimension;
		auto& rows = polytope.rows;
		auto& cols = polytope.columns;
		auto pt = polytope.vector;

		ostream << dimension << ' ' ;
		ostream << rows << ' ' ;
		ostream << cols << ' ' ;

		size_t items = rows * cols ;

		// assumes items > 0
		while ( --items > 0 ) ostream << *(pt++) << ' ' ;
		ostream << *(pt++) << std::endl ;

	}

	template <typename O, typename T>
	void dump(O& ostream, const T dimension, const T rows, const T cols, T** matrix) {

		ostream << dimension << ' ' ;
		ostream << rows << ' ' ;
		ostream << cols << ' ' ;

		// assumes rows * cols > 0
		for ( T i = 0 ; i < rows - 1 ; ++i ) {
			for ( T j = 0 ; j < cols ; ++j) {
				ostream << matrix[i][j] << ' ' ;
			}
		}

		for ( T j = 0 ; j < cols - 1 ; ++j) ostream << matrix[rows-1][j] << ' ' ;

		ostream << matrix[rows-1][cols-1] << std::endl ;

	}

}

#endif // H_TL_DUMP
