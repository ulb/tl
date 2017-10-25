#ifndef H_TL_CHECKSIMPLICIALCORE
#define H_TL_CHECKSIMPLICIALCORE

namespace tl {
	template <typename T>
	bool checksimplicialcore(T** S, const T D) {
	    for (T i = 0; i < D; ++i) {
	        if (S[i][i] != 1) return false;
	        for (T j = i+1; j < D; ++j) if (S[i][j] != 0) return false;
	    }
	    return true;
	}
}

#endif // H_TL_CHECKSIMPLICIALCORE
