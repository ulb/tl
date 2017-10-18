#ifndef H_NT_MY_POW
#define H_NT_MY_POW


namespace nt {
	// compute the power of an int, x base, p exponent
	template <typename T,typename UNSIGNED_T>
	T my_pow(T x, UNSIGNED_T p) {
	    T tot = 1;
	    for (int i = 1; i <= p; ++i) tot *= x;
	    return tot;
	}
}

#endif // H_NT_MY_POW