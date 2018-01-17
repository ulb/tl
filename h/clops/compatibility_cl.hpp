#ifndef H_CLOPS_COMPATIBILITY_CL
#define H_CLOPS_COMPATIBILITY_CL

#include <cstring> // std::equal

namespace clops {

	// compatibility closure operator
	template <typename T,typename SIZE>
	bool compatibility_cl(T * A,T ** incompatibility_adjM,const SIZE length_A) {
	    for (SIZE i = 1; i < length_A; ++i) {
	        if (A[i] == 0) continue;
			const auto B = incompatibility_adjM[i];
	        for (SIZE j = 0; j < i; ++j) if (A[j] & B[j]) return false;
	    }
	    return true;
	}

	// fast compatibility closure operator
	template <typename T,typename SIZE>
	bool fast_compatibility_cl(T* A, uint64_t* A64, uint64_t ** I64,const SIZE n) {
	    for (SIZE i = 1; i < n; ++i) {
	        if (A[i] == 0) continue;
			const auto B64 = I64[i];
			const auto k = i / 64;
			const auto l = i % 64;
	        for (SIZE j = 0; j < k; ++j) if (A64[j] & B64[j]) return false;
			if ( l != 0 ) if (A64[k] & B64[k] & ((~uint64_t(0)) >> (64-l))) return false;
	    }
	    return true;
	}


}

#endif // H_CLOPS_COMPATIBILITY_CL
