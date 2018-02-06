#ifndef H_LINALG_EYE
#define H_LINALG_EYE

namespace linalg {
	// generate the identity matrix of size kxk
	template <typename T,typename SIZE>
	void eye(const SIZE k, T * const * const dest) {
	    for (SIZE i = 0; i < k; ++i) {
	        std::memset(dest[i],0,i * sizeof(T));
	        dest[i][i] = 1;
	        std::memset(dest[i]+i+1,0,(k-i-1) * sizeof(T));
	    }
	}
}

#endif // H_LINALG_EYE
