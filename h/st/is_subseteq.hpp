#ifndef H_ST_IS_SUBSETEQ
#define H_ST_IS_SUBSETEQ

namespace st {
	// Check if A is a subst of B
	template <typename T, typename SIZE>
	bool is_subseteq(const T * const A, const T * const B, const SIZE n) {
	    for (SIZE i = 0; i < n; ++i) if (A[i] & (1-B[i])) return false;
	    return true;
	}
}

#endif // H_ST_IS_SUBSETEQ
