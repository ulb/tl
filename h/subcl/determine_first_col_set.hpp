#ifndef H_SUBCL_DETERMINE_FIRST_COL_SET
#define H_SUBCL_DETERMINE_FIRST_COL_SET

#include <cstring>
#include <algorithm>

namespace subcl {
	template <typename T,typename SIZE>
	void determine_first_col_set(T ** col_bkts_mltconst SIZE num_unique_row,const SIZE length,void *& mem_first_partition,T **& first_partition,T *& first_partition_mlt,SIZE & first_partition_size,T * unique_row_mlt,const SIZE idx_min, SIZE & idx_first_partition) {
		SIZE i,j;
		for (i = 0; i < num_unique_row; ++i) {

			std::min_element(unique_row_mlt, unique_row_mlt+num_unique_row);
			
		}

	}
}
#endif // H_SUBCL_DETERMINE_FIRST_COL_SET