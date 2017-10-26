#ifndef H_MEM_ALLOC
#define H_MEM_ALLOC

#include <cstdlib>

namespace mem {
	template<typename T,typename SIZE>
	void alloc(T *& p, SIZE num) {
		p = (T *) malloc(num * sizeof(T));
		if (p == NULL) {
			fprintf(stderr,"run out of memory.\n");
			exit(1);
		}
	}
}

#endif // H_MEM_ALLOC
