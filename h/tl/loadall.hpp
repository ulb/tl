#ifndef H_TL_LOADALL
#define H_TL_LOADALL

#include <iostream>
#include "load.hpp"

namespace tl {

	template <typename I, typename A>
	void loadall(I & istream, A & array) {
		while ( tl::load(istream, array));
	}

}

#endif // H_TL_LOADALL
