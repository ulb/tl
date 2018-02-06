#ifndef H_EMB_Xr
#define H_EMB_Xr

#include "linalg/div_ceil.hpp"
#include "mem/alloc.hpp"
#include "mem/alloc_matrix.hpp"
#include "array/get_ones.hpp"
#include "array/pack.hpp"
#include "clops/fast_discreteconvexhull_cl.hpp"
#include "HEmbedding.hpp"

namespace emb {

	template<typename T, typename SIZE, typename W>
	HEmbedding<T,SIZE,W> Xr (const SIZE D, const HEmbedding<T,SIZE,W>& X, const SIZE num_slabs, const SIZE n_cols_64) {

		// final set stays the same we only reduce the size of X.comp
		const SIZE finalsize(X.finalsize);

		T *A;
		mem::alloc(A,finalsize);
		std::fill(A,A+finalsize,1);

		W* B_64;
		mem::alloc(B_64,n_cols_64);

		const SIZE e1_64 = linalg::div_ceil(X.e1, 64);
		W* C_64;
		mem::alloc(C_64,e1_64);

		clops::fast_discreteconvexhull_cl(A, B_64, C_64, X.ps_64, X.sp_64_comp, finalsize, num_slabs, e1_64, n_cols_64);

		free(A);
		free(B_64);

		T *C;
		const SIZE e1 = array::get_ones_64(C_64,X.e1,C);
		free(C_64);

		void * mem;
		T ** comp;
		mem::alloc_matrix(mem,comp,e1 + finalsize,D);

		T ** final(comp+e1);
		const SIZE compsize = e1 + finalsize;

        const int n_rows_big_64 = linalg::div_ceil(compsize, 64);

		// Check points versus slabs incidence
		void * mem_ps;
		T ** ps;
		mem::alloc_matrix(mem_ps,ps,finalsize,num_slabs);

		// BIG ONES
		// Check points versus slabs incidence - BIG
		void * mem_ps_comp;
		T ** ps_comp;
		mem::alloc_matrix(mem_ps_comp,ps_comp,compsize,num_slabs);

		SIZE i = 0;
		for (; i < e1; ++i) {
			std::memcpy(comp[i], X.comp[C[i]], D*sizeof(T));
			std::memcpy(ps_comp[i], X.ps_comp[C[i]], num_slabs*sizeof(T));
		}

		free(C);

		for (SIZE j = 0; j < finalsize; ++j) {
			std::memcpy(comp[i], X.final[j], D*sizeof(T));
			std::memcpy(ps[j], X.ps[j], num_slabs*sizeof(T));
			std::memcpy(ps_comp[i], X.ps_comp[X.e1+j], num_slabs*sizeof(T));
			++i;
		}

		// Construct the 64-bit blocks
		void * mem_ps_64;
		W ** ps_64;
		mem::alloc_matrix(mem_ps_64,ps_64,finalsize,n_cols_64);
		array::pack64_matrix(ps,ps_64,finalsize,num_slabs,n_cols_64);

		// Transpose - BIG
		void * mem_sp_comp;
		T ** sp_comp;
		mem::alloc_matrix(mem_sp_comp,sp_comp,num_slabs,compsize);
		linalg::transpose(ps_comp,sp_comp,compsize,num_slabs);

		// Transpose-64 - BIG
		void * mem_sp_comp_64;
		uint64_t ** sp_comp_64;
		mem::alloc_matrix(mem_sp_comp_64,sp_comp_64,num_slabs,n_rows_big_64);
		array::pack64_matrix(sp_comp,sp_comp_64,num_slabs,compsize,n_rows_big_64);

		free(mem_sp_comp);

		return HEmbedding<T,SIZE,W>(
			mem,
			comp,
			compsize,
			final,
			finalsize,
			e1,
			n_rows_big_64,
			mem_ps,
			ps,
			mem_ps_comp,
			ps_comp,
			mem_ps_64,
			ps_64,
			mem_sp_comp_64,
			sp_comp_64
		);

	}

}

#endif // H_EMB_Xr
