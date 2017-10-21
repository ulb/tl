#ifndef H_CLOPS_IS_PRECCURLYEQ
#define H_CLOPS_IS_PRECCURLYEQ

namespace clops {
	// Check if A \preccurlyeq B
	template <typename T,typename SIZE>
	bool is_preccurlyeq(T* A, T* B, const SIZE length) {
		T* a(A+length), * b(B+length);
	    while (a != A) if (*(--a) != *(--b)) return *b == 1;
	    return true;
	}
}

#endif // H_CLOPS_IS_PRECCURLYEQ
