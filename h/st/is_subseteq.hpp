#ifndef H_ST_IS_SUBSETEQ
#define H_ST_IS_SUBSETEQ

namespace st {
	// Check if A is a subst of B
	template <typename T, typename SIZE>
	bool is_subseteq(T *A, T *B, const SIZE length) {
	    for (int i = 0; i < length; ++i) if (A[i] & (1-B[i])) return false;
	    return true;
	}
}

#endif // H_ST_IS_SUBSETEQ
