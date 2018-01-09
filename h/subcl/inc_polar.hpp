#ifndef H_SUBCL_INC_POLAR
#define H_SUBCL_INC_POLAR

#include <utility>
#include "array/is_equal.hpp"

namespace subcl {
	// Test if the polar of the 2-level polytope having slack-matrix S_new is a still 2_level
	template <typename S, typename P, typename COUNT>
	void inc_polar(S& cgs, P& polytope, COUNT& n_polar) {

		P transpose = polytope.dual();
		auto m = polytope.cg_end - polytope.cg;
		auto n = transpose.cg_end - transpose.cg;
	    if (m == n && array::is_equal(transpose.cg,polytope.cg,n)) n_polar += 1; // self-polar
	    else {
        	std::pair<setword*,setword*> pair(transpose.cg, transpose.cg_end);
	    	if (cgs.count(pair)) n_polar += 2; // dual-polar
	    }

	    transpose.teardown();

	}
}

#endif // H_SUBCL_INC_POLAR