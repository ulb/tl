#ifndef H_EMB_V
#define H_EMB_V

#include "nt/my_pow.hpp"
#include "mem/alloc_matrix.hpp"
#include "construct_big_ground_V.hpp"
#include "GroundSet.hpp"

namespace emb {

	template<typename SIZE>
	GroundSet<int,SIZE> V (const SIZE d) {

		const SIZE bigsize = nt::my_pow(3,d-2) * 2;
		const SIZE size = (nt::my_pow(3,d-1)+1) / 2;
		const SIZE e1 = (nt::my_pow(3,d-2)-1) / 2;

		void * mem;
		int ** bigmatrix;
		mem::alloc_matrix(mem,bigmatrix,bigsize,d);
		construct_big_ground_V(bigmatrix,d);
		int ** matrix(bigmatrix+e1);

		return GroundSet<int,SIZE>(mem,bigmatrix,bigsize,NULL,0,matrix,size,e1);

	}

}

#endif // H_EMB_V
