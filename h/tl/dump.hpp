#ifndef H_TL_DUMP
#define H_TL_DUMP

#include <iostream>

namespace tl {

	template <typename O, typename P>
	void dump_g(O& ostream, P& polytope) {

		auto& dimension = polytope.dimension;
		auto& rows = polytope.rows;
		auto& cols = polytope.columns;
		auto pt = *(polytope.matrix);

		ostream << 'g' << ' ' ;
		ostream << dimension << ' ' ;
		ostream << rows << ' ' ;
		ostream << cols << ' ' ;

		const size_t items = rows * cols ;

		for ( size_t i = 0 ; i < items ; ++i ) ostream << *(pt++) ;
		ostream << std::endl ;

	}

	template <typename O, typename P>
	void dump_s(O& ostream, P& polytope) {

		auto& dimension = polytope.dimension;
		auto& rows = polytope.rows;
		auto& cols = polytope.columns;
		auto pt = *(polytope.matrix);

		ostream << 's' << ' ' ;
		ostream << dimension << ' ' ;
		ostream << rows << ' ' ;
		ostream << cols << ' ' ;

		size_t items = rows * cols ;

		// assumes items > 0
		while ( --items > 0 ) ostream << *(pt++) << ' ' ;
		ostream << *(pt++) << std::endl ;

	}

	template <typename O, typename T>
	void dump_g(O& ostream, const T dimension, const T rows, const T cols, T** matrix) {

		ostream << 'g' << ' ' ;
		ostream << dimension << ' ' ;
		ostream << rows << ' ' ;
		ostream << cols << ' ' ;

		// assumes rows * cols > 0
		for ( T i = 0 ; i < rows ; ++i ) {
			for ( T j = 0 ; j < cols ; ++j) {
				ostream << matrix[i][j] ;
			}
		}

		ostream << std::endl ;

	}

	template <typename O, typename T>
	void dump_s(O& ostream, const T dimension, const T rows, const T cols, T** matrix) {

		ostream << 's' << ' ' ;
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

	template <typename O, typename P>
	void dump(O& ostream, P& polytope) {
		dump_g(ostream, polytope); // default
	}

	template <typename O, typename T>
	void dump(O& ostream, const T dimension, const T rows, const T cols, T** matrix) {
		dump_g(ostream, dimension, rows, cols, matrix) ; // default
	}

}

#endif // H_TL_DUMP
