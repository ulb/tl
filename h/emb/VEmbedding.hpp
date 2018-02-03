#ifndef H_EMB_VEMBEDDING
#define H_EMB_VEMBEDDING

namespace emb {

	template <typename T, typename SIZE>
	class VEmbedding {
		public:

			void *mem;
			T **full;
			const SIZE fullsize;
			T **final;
			const SIZE finalsize;
			const SIZE e1;

			VEmbedding(void *mem, T **full, const SIZE fullsize, T **final, const SIZE finalsize, const SIZE e1):
			mem(mem),full(full),fullsize(fullsize),final(final),finalsize(finalsize),e1(e1) {}

			void teardown(){
				free(this->mem);
			}
	};

}

#endif // H_EMB_VEMBEDDING
