#ifndef H_NT_POW
#define H_NT_POW


namespace nt {
	// compute the power of an int, x base, p exponent
	template <typename T,typename SIZE>
	T pow(const T x, const SIZE p) {
		T tot(1);
		for (SIZE i = 1; i <= p; ++i) tot *= x;
		return tot;
	}
}

#endif // H_NT_POW
