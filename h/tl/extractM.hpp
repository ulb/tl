#ifndef H_TL_EXTRACTM
#define H_TL_EXTRACTM

namespace tl {
    // extract the top (D-1)x(D-1) matrix M_{d-1} and extend it to M_d(0)
	template <typename T>
	void extractM(T ** S, T ** M, const T D) {
	    int i,j;
	    for (i = 0; i < D; ++i) {
	        for (j = 0; j < D; ++j)
	            if (i == 0 && j == 0) M[i][j] = 1;
	            else if (i == 0 || j == 0) M[i][j] = 0;
	            else M[i][j] = S[i-1][j-1];
	    }
	}
}

#endif // H_TL_EXTRACTM
