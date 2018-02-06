#ifndef H_LINALG_INNERPROD
#define H_LINALG_INNERPROD

namespace linalg {
	// <x,y> = out
	template <typename T,typename SIZE>
	T innerprod(const T * const x, const T * const y, const SIZE length) {
	    T out = 0;
	    for (SIZE i = 0; i < length; ++i) out += x[i]*y[i];
	    return out;
	}
}
#endif // H_LINALG_INNERPROD
