#ifndef H_LINALG_MY_INNER_PROD
#define H_LINALG_MY_INNER_PROD

namespace linalg {
	// <x,y> = out
	template <typename T,typename SIZE>
	T my_inner_prod(T * x, T * y, SIZE length) {
	    T out = 0;
	    for (SIZE i = 0; i < length; ++i) out += x[i]*y[i];
	    return out;
	}
}
#endif // H_LINALG_MY_INNER_PROD