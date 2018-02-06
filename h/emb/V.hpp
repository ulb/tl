#ifndef H_EMB_V
#define H_EMB_V

#include "nt/pow.hpp"
#include "mem/alloc_matrix.hpp"
#include "construct_big_ground_V.hpp"
#include "VEmbedding.hpp"

namespace emb {

	template<typename SIZE>
	VEmbedding<int,SIZE> V (const SIZE d) {

		const SIZE bigsize = nt::pow(3,d-2) * 2;
		const SIZE size = (nt::pow(3,d-1)+1) / 2;
		const SIZE e1 = (nt::pow(3,d-2)-1) / 2;

		void * mem;
		int ** bigmatrix;
		mem::alloc_matrix(mem,bigmatrix,bigsize,d);
		construct_big_ground_V(bigmatrix,d);
		int ** matrix(bigmatrix+e1);

		return VEmbedding<int,SIZE>(mem,bigmatrix,bigsize,matrix,size,e1);

	}

}

#endif // H_EMB_V
