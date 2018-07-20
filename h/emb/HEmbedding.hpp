#ifndef H_EMB_HEMBEDDING
#define H_EMB_HEMBEDDING

namespace emb {

	template <typename T, typename SIZE, typename W>
	class HEmbedding {
		public:

			const SIZE fullsize;
			void *mem;
			T **comp;
			const SIZE compsize;
			SIZE * list_accepted;
			T ** final;
			const SIZE finalsize;
			const SIZE full_e1;
			const SIZE e1;
			const SIZE n_rows_64;
			const SIZE n_rows_big_64;
			void * mem_ps;
			T ** ps;
			void * mem_ps_comp;
			T ** ps_comp;
			void * mem_ps_64;
			W ** ps_64;
			void * mem_sp_64_comp;
			W ** sp_64_comp;

			HEmbedding(
					const SIZE fullsize,
					void *mem,
					T **comp,
					const SIZE compsize,
					SIZE * list_accepted,
					T **final,
					const SIZE finalsize,
					const SIZE full_e1,
					const SIZE e1,
					const SIZE n_rows_64,
					const SIZE n_rows_big_64,
					void *mem_ps,
					T **ps,
					void *mem_ps_comp,
					T **ps_comp,
					void *mem_ps_64,
					W **ps_64,
					void *mem_sp_64_comp,
					W **sp_64_comp
			):
			fullsize(fullsize),
			mem(mem),
			comp(comp),
			compsize(compsize),
			list_accepted(list_accepted),
			final(final),
			finalsize(finalsize),
			full_e1(full_e1),
			e1(e1),
			n_rows_64(n_rows_64),
			n_rows_big_64(n_rows_big_64),
			mem_ps(mem_ps),
			ps(ps),
			mem_ps_comp(mem_ps_comp),
			ps_comp(ps_comp),
			mem_ps_64(mem_ps_64),
			ps_64(ps_64),
			mem_sp_64_comp(mem_sp_64_comp),
			sp_64_comp(sp_64_comp) {}

			void teardown(){
				free(this->mem);
				free(this->list_accepted);
				free(this->mem_ps);
				free(this->mem_ps_comp);
				free(this->mem_ps_64);
				free(this->mem_sp_64_comp);
			}
	};

}

#endif // H_EMB_HEMBEDDING
