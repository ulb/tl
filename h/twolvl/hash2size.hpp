#ifndef H_TWOLVL_HASH2SIZE
#define H_TWOLVL_HASH2SIZE

namespace twolvl {
	// output the canonical form of a slack matrix
	template <typename T,typename SIZE>
	void hash2size(const T S_hash,SIZE & num_rows,SIZE & num_cols, const T D) {
	    num_rows = 1 + (((1 << D) - 1) & S_hash);
	    num_cols = 1 + (((1 << D) - 1) & (S_hash >> D));
	}
}

#endif // H_TWOLVL_HASH2SIZE