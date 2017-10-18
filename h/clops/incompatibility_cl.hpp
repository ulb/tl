#ifndef H_CLOPS_INCOMPATIBILITY_CL
#define H_CLOPS_INCOMPATIBILITY_CL

#include <cstring> // std::equal

namespace clops {
	// test for incompatibility
	template <typename T,typename SIZE>
	bool is_incompatible(T * A,T ** incompatibility_adjM,const SIZE length_A) {
	    for (int i = 0; i < length_A; ++i) {
	        if (A[i] == 0) continue;
	        for (int j = 0; j < i; ++j) if (A[j] == 1 && incompatibility_adjM[i][j] == 1) return false;
	    }
	    return true;
	}

	// incompatibility closure operator
	template <typename T,typename SIZE>
	void incompatibility_cl(T * A,T * inccl,T ** incompatibility_adjM,const SIZE length_A) { 
	    if (is_incompatible(A,incompatibility_adjM,length_A))
	        std::memcpy(inccl,A,length_A * sizeof(T));
	    else std::fill(inccl,inccl+length_A,1);
	}
}

#endif // H_CLOPS_INCOMPATIBILITY_CL