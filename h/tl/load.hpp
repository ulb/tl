#ifndef H_TL_LOAD
#define H_TL_LOAD

#include <iostream>
#include "alloc.hpp"
#include "loadheader.hpp"
#include "loadmatrix_g.hpp"
#include "loadmatrix_s.hpp"

namespace tl {

	template <typename I, typename A>
	bool load(I& istream, A& array) {

		char format;
		int dimension, rows, columns;

		if ( !tl::loadheader(istream, format, dimension, rows, columns) ) return false ;

		int* data;
		alloc(data, 3 + rows * columns, int);

		int* pt(data);

		*(pt++) = dimension;
		*(pt++) = rows;
		*(pt++) = columns;
		const int length = rows * columns ;
		if ( format == 'g' ) for ( int i = 0 ; i < length ; ++i ) *(pt++) = loadmatrix_g(istream) ;
		else if ( format == 's' ) for ( int i = 0 ; i < length ; ++i ) *(pt++) = loadmatrix_s(istream) ;
		else {
			// error
		}

		array.emplace_back(dimension, rows, columns, data);

		std::cerr << " OK" << std::endl;

		return true;

	}

}

#endif // H_TL_LOAD
