#ifndef H_TL_LOADHEADER
#define H_TL_LOADHEADER

#include <iostream>

namespace tl {

	template <typename I>
	bool loadheader(I& istream, char& format, int& dimension, int& rows, int& columns) {

		istream >> format;

		if ( istream.eof() ) return false ; // otherwise assume input is correctly formatted and canonicized

		istream >> dimension >> rows >> columns ;

		std::cerr << "reading facet in format " << format
				  << " of dimension " << dimension
				  << " with " << rows << " rows and "
				  << columns << " columns...";

		return true;

	}

}

#endif // H_TL_LOADHEADER
