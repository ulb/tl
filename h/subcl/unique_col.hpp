#ifndef H_SUBCL_UNIQUE_COL
#define H_SUBCL_UNIQUE_COL

#include <cstring>
#include <algorithm>
#include "alloc.hpp"
#include "linalg/transposeM.hpp"
#include "subcl/index_in_array.hpp"
#include "subcl/idx_in_collection.hpp"
#include "subcl/extract_col.hpp"

namespace subcl {
	template <typename T,typename SIZE>
	void unique_col(T ** M_transposed,const SIZE num_rows,const SIZE num_cols,const SIZE j,T * row,T * unique_row,const SIZE num_unique_row,T ** unique_cols,SIZE & num_unique_col,T ** col_bkts_mlt) {
		SIZE i,k;
		SIZE idx,test_idx;
		T * col;
		alloc(col,num_rows-1,T);
		for (i = 0; i < num_cols; ++i) {
			idx = subcl::index_in_array(unique_row,row[i],num_unique_row);
			subcl::extract_col(M_transposed[i],col,num_rows,j); // extract column i (all rows but the j-th)
			test_idx = subcl::idx_in_collection(unique_cols,col,num_unique_col,num_rows-1);
			if (test_idx == num_unique_col) {
				for (k = 0; k < idx; ++k) col_bkts_mlt[k][num_unique_col] = 0;
				col_bkts_mlt[idx][num_unique_col] = 1;
				for (++k; k < num_unique_row; ++k) col_bkts_mlt[k][num_unique_col] = 0;
				std::memcpy(unique_cols[num_unique_col++],col,(num_rows-1) * sizeof(T));
			}
			else ++col_bkts_mlt[idx][test_idx];
		}
		free(col);
	}
}
#endif // H_SUBCL_UNIQUE_COL