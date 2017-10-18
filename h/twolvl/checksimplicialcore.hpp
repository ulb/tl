#ifndef H_TWOLVL_CHECKSIMPLICIALCORE
#define H_TWOLVL_CHECKSIMPLICIALCORE

namespace twolvl {
	template <typename T>
	bool checksimplicialcore(T ** S, const T D) {
	    for (int i = 0; i < D; ++i) {
	        if (S[i][i] != 1) return false;
	        for (int j = i+1; j < D; ++j) if (S[i][j] != 0) return false;
	    }
	    return true;
	}
}

#endif // H_TWOLVL_CHECKSIMPLICIALCORE