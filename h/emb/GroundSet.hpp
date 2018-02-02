#ifndef H_EMB_GROUNDSET
#define H_EMB_GROUNDSET

namespace emb {

	template <typename T, typename SIZE>
	class GroundSet {
		public:

			void *mem;
			T **full;
			const SIZE fullsize;
			T **comp;
			const SIZE compsize;
			T **final;
			const SIZE finalsize;
			const SIZE e1;

			GroundSet(void *mem, T **full, const SIZE fullsize, T **comp, const SIZE compsize, T **final, const SIZE finalsize, const SIZE e1):
			mem(mem),full(full),fullsize(fullsize),comp(comp),compsize(compsize),final(final),finalsize(finalsize),e1(e1) {}

			void teardown(){
				free(this->mem);
			}
	};

}

#endif // H_EMB_GROUNDSET
