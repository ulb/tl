#ifndef H_TWOLVL_LOAD
#define H_TWOLVL_LOAD

#include <iostream>
#include "alloc.hpp"

namespace twolvl {

	template <typename I, typename A>
	bool load(I& istream, A& array) {

		int dimension;

		istream >> dimension ;
		if ( istream.eof() ) return false ; // otherwise assume input is correctly formatted and canonicized

		int rows, columns;

		istream >> rows >> columns ;

		std::cerr << "reading facet"
				  << " of dimension " << dimension
				  << " with " << rows << " rows and "
				  << columns << " columns...";

		int* data;
		alloc(data, 3 + rows * columns, int);

		int* pt(data);

		*(pt++) = dimension;
		*(pt++) = rows;
		*(pt++) = columns;
		const int length = rows * columns ;
		for ( int i = 0 ; i < length ; ++i ) istream >> *(pt++) ;

		array.emplace_back(dimension, rows, columns, data);

		std::cerr << " OK" << std::endl;

		return true;

	}

}

#endif // H_TWOLVL_LOAD
