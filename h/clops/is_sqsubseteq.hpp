#ifndef H_CLOPS_IS_SQSUBSETEQ
#define H_CLOPS_IS_SQSUBSETEQ

#include "is_subseteq.hpp"

namespace clops {
	template <typename T,typename SIZE>
	int min_A_idx(T * A,const SIZE length) {
	    for (int i = 0; i < length; ++i) if (A[i] == 1) return i;
	    return length;
	}

	// Check if A \sqsubseteq B
	template <typename T,typename SIZE>
	bool is_sqsubseteq(T * A, T * B,const SIZE length) {
	    int min_A = min_A_idx(A,length);
	    if (clops::is_subseteq(A,B,length)) return array::is_equal(B+min_A,A+min_A,length-min_A);
	    else return false;
	}

}

#endif // H_CLOPS_IS_SQSUBSETEQ