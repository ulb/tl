#ifndef H_EMB_HEMBEDDING
#define H_EMB_HEMBEDDING

namespace emb {

	template <typename T, typename SIZE, typename W>
	class HEmbedding {
		public:

			void *mem;
			T **comp;
			const SIZE compsize;
			T **final;
			const SIZE finalsize;
			const SIZE e1;

			void * mem_ps;
			T ** ps;
			void * mem_ps_comp;
			T ** ps_comp;
			void * mem_ps_64;
			W ** ps_64;
			void * mem_sp_64_comp;
			W ** sp_64_comp;

			HEmbedding(
					void *mem,
					T **comp,
					const SIZE compsize,
					T **final,
					const SIZE finalsize,
					const SIZE e1,
					void *mem_ps,
					T **ps,
					void *mem_ps_comp,
					T **ps_comp,
					void *mem_ps_64,
					W **ps_64,
					void *mem_sp_64_comp,
					W **sp_64_comp
			):
			mem(mem),
			comp(comp),
			compsize(compsize),
			final(final),
			finalsize(finalsize),
			e1(e1),
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
				free(this->mem_ps);
				free(this->mem_ps_comp);
				free(this->mem_ps_64);
				free(this->mem_sp_64_comp);
			}
	};

}

#endif // H_EMB_HEMBEDDING
