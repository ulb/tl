#ifndef H_LINALG_DIV_CEIL
#define H_LINALG_DIV_CEIL

namespace linalg {
	template <typename X>
	X div_ceil(X n, X d) {
		return (n/d) + ( n%d == 0 ? 0 : 1 );
	}
}

#endif // H_LINALG_DIV_CEIL