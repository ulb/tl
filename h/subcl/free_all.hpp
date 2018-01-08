#ifndef H_SUBCL_FREE_ALL
#define H_SUBCL_FREE_ALL

namespace subcl {
    template <typename T,typename SIZE>
	void free_all(T * unique_row,T * red_unique_row_mlt,T *** col_bkts,SIZE * col_bkts_sizes,void ** mem_col_bkts,const SIZE num_unique, void * mem_col_bkts_mlt) {
		for (SIZE i = 0; i < num_unique; ++i) free(mem_col_bkts[i]);
		free(unique_row);
		free(red_unique_row_mlt);
		free(col_bkts);
		free(mem_col_bkts);
		free(col_bkts_sizes);
		free(mem_col_bkts_mlt);
	}
}
#endif // H_SUBCL_FREE_ALL