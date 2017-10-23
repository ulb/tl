#ifndef H_LINALG_IS_ID
#define H_LINALG_IS_ID

namespace linalg {
	// check if input is identity matrix of size kxk
	template <typename SIZE, typename T>
	bool is_id(SIZE k, T** M) {
	    for (SIZE i = 0; i < k; ++i) {
			SIZE j = 0;
			for (   ; j < i; ++j) if (M[i][j] == 1) return false;
			if (M[i][i] == 0) return false;
			for (++j; j < k; ++j) if (M[i][j] == 1) return false;
	    }
		return true;
	}
}

#endif // H_LINALG_IS_ID
