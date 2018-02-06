#ifndef H_EMB_Xs
#define H_EMB_Xs

#include "linalg/div_ceil.hpp"
#include "mem/alloc.hpp"
#include "mem/alloc_matrix.hpp"
#include "array/get_ones.hpp"
#include "array/pack.hpp"
#include "clops/fast_discreteconvexhull_cl.hpp"
#include "HEmbedding.hpp"

namespace emb {

	template<typename T, typename SIZE, typename W>
	HEmbedding<T,SIZE,W> Xs (const SIZE D, const HEmbedding<T,SIZE,W>& X, const SIZE num_slabs, const SIZE n_cols_64) {

		// final set stays the same we only reduce the size of X.comp
		const SIZE finalsize(X.finalsize);

		void * mem;
		T ** final;
		mem::alloc_matrix(mem,final,finalsize,D);

		std::memcpy(*final, *X.final, finalsize * D * sizeof(T));

		const int n_rows_big_64 = linalg::div_ceil(finalsize, 64);

		// Check points versus slabs incidence
		void * mem_ps;
		T ** ps;
		mem::alloc_matrix(mem_ps, ps, finalsize, num_slabs);
		std::memcpy(*ps, *X.ps, finalsize * num_slabs * sizeof(T));

		// Check points versus slabs incidence - same as final
		void * mem_ps_comp;
		T ** ps_comp;
		mem::alloc_matrix(mem_ps_comp, ps_comp, finalsize, num_slabs);
		std::memcpy(*ps_comp, *X.ps, finalsize * num_slabs * sizeof(T));

		// Construct the 64-bit blocks
		void * mem_ps_64;
		W ** ps_64;
		mem::alloc_matrix(mem_ps_64, ps_64, finalsize,n_cols_64);
		std::memcpy(*ps_64, *X.ps_64, finalsize * n_cols_64 * sizeof(W));

		// Transpose - BIG
		void * mem_sp_comp;
		T ** sp_comp;
		mem::alloc_matrix(mem_sp_comp,sp_comp,num_slabs,finalsize);
		linalg::transpose(ps_comp,sp_comp,finalsize,num_slabs);

		// Transpose-64 - BIG
		void * mem_sp_comp_64;
		uint64_t ** sp_comp_64;
		mem::alloc_matrix(mem_sp_comp_64,sp_comp_64,num_slabs,n_rows_big_64);
		array::pack64_matrix(sp_comp,sp_comp_64,num_slabs,finalsize,n_rows_big_64);

		free(mem_sp_comp);

		return HEmbedding<T,SIZE,W>(
			mem,
			final,
			finalsize,
			final,
			finalsize,
			0,
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

#endif // H_EMB_Xs
