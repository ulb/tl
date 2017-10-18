#ifndef H_LINALG_MY_INNER_PROD
#define H_LINALG_MY_INNER_PROD

namespace linalg {
	// <x,y> = out
	template <typename T,typename SIZE>
	void my_inner_prod(T * x, T * y, T & out, SIZE length) {
	    out = 0;
	    for (int i = 0; i < length; ++i) out += x[i]*y[i];
	}
}
#endif // H_LINALG_MY_INNER_PROD