#ifndef H_TWOLVL_DUMP
#define H_TWOLVL_DUMP

#include <iostream>

namespace twolvl {

	template <typename O, typename A>
	void dump(O& ostream, A& atom) {

		auto& dimension = atom.dimension;
		auto& rows = atom.rows;
		auto& cols = atom.columns;
		auto pt = atom.vector;

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

#endif // H_TWOLVL_DUMP
