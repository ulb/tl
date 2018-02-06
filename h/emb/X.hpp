#ifndef H_EMB_X
#define H_EMB_X

#include "mem/alloc_matrix.hpp"
#include "array/pack.hpp"
#include "VEmbedding.hpp"
#include "HEmbedding.hpp"

namespace emb {

	template<typename T, typename SIZE, typename S>
	HEmbedding<T,SIZE,uint64_t> X (const SIZE D, const VEmbedding<T,SIZE>& V, T **facets_base, const SIZE num_facets_base, const S& slabs, const SIZE n_cols_64, T **Minv) {

		void * mem;
		T ** comp;
		mem::alloc_matrix(mem,comp,V.fullsize,D);
		const SIZE e1 = base::construct_ground_H(comp,V.full,V.e1,facets_base,num_facets_base,Minv,D);
		T ** final(comp+e1);
		const SIZE finalsize = base::construct_ground_H(final,V.final,V.finalsize,facets_base,num_facets_base,Minv,D);
		const SIZE compsize = e1 + finalsize;

		const SIZE n_rows_big_64 = linalg::div_ceil(compsize, 64);

		// Check points versus slabs incidence
		void * mem_ps;
		T ** ps;
		mem::alloc_matrix(mem_ps,ps,finalsize,slabs.rows);
		base::construct_slab_point_sat(ps,final,slabs.matrix,finalsize,slabs.rows,D);

		// Construct the block uint64_t
		void * mem_ps_64;
		uint64_t ** ps_64;
		mem::alloc_matrix(mem_ps_64,ps_64,finalsize,n_cols_64);
		array::pack64_matrix(ps,ps_64,finalsize,slabs.rows,n_cols_64);

		// BIG ONES
		// Check points versus slabs incidence - BIG
		void * mem_ps_comp;
		T ** ps_comp;
		mem::alloc_matrix(mem_ps_comp,ps_comp,compsize,slabs.rows);
		base::construct_slab_point_sat(ps_comp,comp,slabs.matrix,compsize,slabs.rows,D);

		// Transpose - BIG
		void * mem_sp_comp;
		T ** sp_comp;
		mem::alloc_matrix(mem_sp_comp,sp_comp,slabs.rows,compsize);
		linalg::transpose(ps_comp,sp_comp,compsize,slabs.rows);

		// Transpose-64 - BIG
		void * mem_sp_comp_64;
		uint64_t ** sp_comp_64;
		mem::alloc_matrix(mem_sp_comp_64,sp_comp_64,slabs.rows,n_rows_big_64);
		array::pack64_matrix(sp_comp,sp_comp_64,slabs.rows,compsize,n_rows_big_64);

		free(mem_sp_comp);

		return HEmbedding<T,SIZE,uint64_t>(
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

#endif // H_EMB_X
