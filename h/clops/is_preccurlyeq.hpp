#ifndef H_CLOPS_IS_PRECCURLYEQ
#define H_CLOPS_IS_PRECCURLYEQ

#include "../array/is_equal.hpp"

namespace clops {
	// Check if A \preccurlyeq B
	template <typename T,typename SIZE>
	bool is_preccurlyeq(T * A, T * B,const SIZE length) {
	    if (array::is_equal(A,B,length)) return true;
	    int i = length - 1;
	    while (i > 0) {
	        if (((A[i] == 1) && (B[i] == 0)) || ((A[i] == 0) && (B[i] == 1))) return B[i] == 1;
	        i--;
	    }
	    return B[0] == 1;
	}
}

#endif // H_CLOPS_IS_PRECCURLYEQ