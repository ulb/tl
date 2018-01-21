#ifndef H_MEM_ALLOC
#define H_MEM_ALLOC

#include <cstdlib>
#include <iostream>

namespace mem {
	template<typename T,typename SIZE>
	void alloc(T *& p, SIZE num) {
		const size_t bytes(num * sizeof(T));
		p = (T *) malloc(bytes);
		if (p == NULL) {
			std::cerr << "ran out of memory." << std::endl ;
			exit(1);
		}
	}
}

#endif // H_MEM_ALLOC
