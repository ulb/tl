#ifndef H_CLOPS_IS_SUBSETEQ
#define H_CLOPS_IS_SUBSETEQ

namespace clops {
	// Check if A is a subset of B
	template <typename T,typename SIZE>
	bool is_subseteq(T *A, T *B,const SIZE length) {
	    for (int i = 0; i < length; ++i) if (A[i] & (1-B[i])) return false;
	    return true;
	}
}

#endif // H_CLOPS_IS_SUBSETEQ
