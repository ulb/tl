#ifndef H_ST_MIN_A_IDX
#define H_ST_MIN_A_IDX

namespace st {
	template <typename T,typename SIZE>
	T min_A_idx(const T * const bits, const SIZE length) {
	    for (SIZE i = 0; i < length; ++i) if (bits[i] == 1) return i;
	    return length;
	}
}

#endif // H_ST_MIN_A_IDX
