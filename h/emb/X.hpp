#ifndef H_EMB_X
#define H_EMB_X

#include "mem/alloc_matrix.hpp"
#include "array/pack.hpp"
#include "emb/VEmbedding.hpp"
#include "emb/HEmbedding.hpp"
#include "emb/construct_ground_H.hpp"

namespace emb {

	template<typename T, typename SIZE, typename S>
	HEmbedding<T,SIZE,uint64_t> X (const SIZE D, const VEmbedding<T,SIZE>& V, T **facets_base, const SIZE num_facets_base, const S& slabs, T ** Minv) {

		void * mem;
		T ** comp;
		mem::alloc_matrix(mem,comp,V.fullsize,D);

		SIZE * list_accepted;
		mem::alloc(list_accepted,V.fullsize);
		SIZE * list_indices;
		mem::alloc(list_indices,V.fullsize);
		const SIZE compsize = emb::construct_ground_H(comp,V.full,V.fullsize,facets_base,list_accepted,list_indices,num_facets_base,Minv,D);
		const SIZE fullsize(V.fullsize);
		const SIZE e1 = (compsize-1)/2;
		const SIZE full_e1 = (fullsize-1)/2;
		T ** final(comp + e1);
		const SIZE finalsize = e1+1;

      const SIZE n_rows_64 = linalg::div_ceil(finalsize, 64);
		const SIZE n_rows_big_64 = linalg::div_ceil(compsize, 64);

		// Check points versus slabs incidence
		void * mem_ps;
		T ** ps;
		mem::alloc_matrix(mem_ps,ps,finalsize,slabs.n_rows);
		base::construct_slab_point_sat(ps,final,slabs.matrix,finalsize,slabs.n_rows,D);

		// Construct the block uint64_t
		void * mem_ps_64;
		uint64_t ** ps_64;
		mem::alloc_matrix(mem_ps_64,ps_64,finalsize,slabs.n_rows_64);
		array::pack64_matrix(ps,ps_64,finalsize,slabs.n_rows,slabs.n_rows_64);

		// BIG ONES
		// Check points versus slabs incidence - BIG
		void * mem_ps_comp;
		T ** ps_comp;
		mem::alloc_matrix(mem_ps_comp,ps_comp,compsize,slabs.n_rows);
		base::construct_slab_point_sat(ps_comp,comp,slabs.matrix,compsize,slabs.n_rows,D);

		// Transpose - BIG
		void * mem_sp_comp;
		T ** sp_comp;
		mem::alloc_matrix(mem_sp_comp,sp_comp,slabs.n_rows,compsize);
		linalg::transpose(ps_comp,sp_comp,compsize,slabs.n_rows);

		// Transpose-64 - BIG
		void * mem_sp_comp_64;
		uint64_t ** sp_comp_64;
		mem::alloc_matrix(mem_sp_comp_64,sp_comp_64,slabs.n_rows,n_rows_big_64);
		array::pack64_matrix(sp_comp,sp_comp_64,slabs.n_rows,compsize,n_rows_big_64);

		free(mem_sp_comp);


		return HEmbedding<T,SIZE,uint64_t>(
			fullsize,
			mem,
			comp,
			compsize,
			list_indices,
			list_accepted,
			final,
			finalsize,
			full_e1,
			e1,
			n_rows_64,
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
