#ifndef H_ST_PRECEDES
#define H_ST_PRECEDES

namespace st {
	// Check if A \preccurly B
	template <typename T,typename SIZE>
	bool precedes(T * A, T * B, const SIZE length) {
		T * a(A + length), * b(B + length);
	    while (a != A) if (*(--a) != *(--b)) return *b == 1;
	    return false;
	}

	// Check if A \preccurly B 64 bits at a time
	template <typename SIZE>
	bool precedes_64(uint64_t* A, uint64_t* B, const SIZE length) {
		uint64_t *a(A+length), *b(B+length);
	    while (a != A) if (*(--a) != *(--b)) return *b > *a;
	    return false;
	}
}

#endif // H_ST_PRECEDES
