#ifndef H_TWOLVL_LOADALL
#define H_TWOLVL_LOADALL

#include <iostream>
#include "./load.hpp"

namespace twolvl {

	template <typename I, typename A>
	void loadall(I& istream, A& array) {

		while ( twolvl::load(istream, array) ) ;

	}

}

#endif // H_TWOLVL_LOADALL
