#ifndef H_ST_INC
#define H_ST_INC

#include <cstring> // std::equal

namespace st {
	// I = inc(A,i)
	template <typename T,typename SIZE>
	void inc(const T * const A, T i, T* I,const SIZE length) {
	    std::memset(I,0,i * sizeof(T));
	    I[i] = 1;
	    std::memcpy(I+i+1, A+i+1,(length-i-1) * sizeof(T));
	}
}

#endif // H_ST_INC
