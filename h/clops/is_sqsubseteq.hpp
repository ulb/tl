#ifndef H_CLOPS_IS_SQSUBSETEQ
#define H_CLOPS_IS_SQSUBSETEQ

#include "array/is_equal.hpp"
#include "clops/min_A_idx.hpp"
#include "clops/is_subseteq.hpp"

namespace clops {
	// Check if A \sqsubseteq B
	template <typename T,typename SIZE>
	bool is_sqsubseteq(T * A, T * B,const SIZE length) {
	    T min_A = clops::min_A_idx(A,length);
	    if (clops::is_subseteq(A,B,length)) return array::is_equal(B+min_A,A+min_A,length-min_A);
	    else return false;
	}

}

#endif // H_CLOPS_IS_SQSUBSETEQ
