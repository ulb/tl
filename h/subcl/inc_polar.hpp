#ifndef H_SUBCL_INC_POLAR
#define H_SUBCL_INC_POLAR

#include <utility>
#include "array/is_equal.hpp"

namespace subcl {
	// Test if the polar of the 2-level polytope having slack-matrix S_new is a still 2_level
	template <typename S, typename G>
	size_t inc_polar(const S& cgs, const G& graph, const G& dualgraph) {

		const auto m = graph.end - graph.begin;
		const auto n = dualgraph.end - dualgraph.begin;
		if (m == n && array::is_equal(dualgraph.begin,graph.begin,n)) return 1; // self-polar
		else {
			std::pair<setword*,setword*> pair(dualgraph.begin, dualgraph.end);
			if (cgs.count(pair)) return 2; // dual-polar
		}

		return 0;

	}
}

#endif // H_SUBCL_INC_POLAR
