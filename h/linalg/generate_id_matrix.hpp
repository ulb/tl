#ifndef H_LINALG_GENERATE_ID_MATRIX
#define H_LINALG_GENERATE_ID_MATRIX

namespace linalg {
	// generate the identity matrix of size kxk
	template <typename T,typename SIZE>
	void generate_id_matrix(SIZE k,T ** Id) {
	    int i;
	    for (i = 0; i < k; ++i) {
	        std::memset(Id[i],0,i * sizeof(T));
	        Id[i][i] = 1;
	        std::memset(Id[i]+i+1,0,(k-i-1) * sizeof(T));
	    }
	}
}

#endif // H_LINALG_GENERATE_ID_MATRIX