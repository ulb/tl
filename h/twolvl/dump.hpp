#ifndef H_TWOLVL_DUMP
#define H_TWOLVL_DUMP

#include <iostream>

namespace twolvl {

	template <typename T>
	bool dump(const T D, const T rows, const T cols, T** matrix) {

		std::cout << D << std::endl ;
		std::cout << rows << std::endl ;
		std::cout << cols << std::endl ;

		for (size_t i = 0; i < rows; ++i){

			for ( size_t j = 0 ;;) { // assuming cols > 0
				std::cout << matrix[i][j] ;
				if ( ++j == cols ) break ;
				else std::cout << ' ' ;
			}

			std::cout << std::endl ;

		}

	}

}

#endif // H_TWOLVL_DUMP
