#ifndef H_SUBCL_UNIQUE_ROW
#define H_SUBCL_UNIQUE_ROW

#include <algorithm>
#include <cstring>
#include "alloc.hpp"
#include "subcl/sum_array.hpp"

namespace subcl {
	template <typename T,typename SIZE>
	void unique_row(T * row,T * unique_row,T * unique_row_mlt,const SIZE num_cols, SIZE & num_unique) {
		T * row_copy;
		alloc(row_copy,num_cols,T);
		std::memcpy(row_copy,row,num_cols * sizeof(T));
		std::sort(row_copy, row_copy + num_cols);
		for (SIZE k = 0; k < num_cols; ++k) {
			while (k < (num_cols - 1) && row_copy[k] == row_copy[k+1]) ++k;
			unique_row[num_unique] = row_copy[k];
			unique_row_mlt[num_unique] = (T)k + 1 - subcl::sum_array(unique_row_mlt,num_unique);
			++num_unique;
		}
		free(row_copy);
	}
}
#endif // H_SUBCL_UNIQUE_ROW