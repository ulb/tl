#ifndef H_SUBCL_FROM_ITH_ROW_ON
#define H_SUBCL_FROM_ITH_ROW_ON

#include <cstring>

namespace subcl {
	template <typename T,typename SIZE>
	void from_ith_row_on(T ** M_out,T ** M,const SIZE num_rows,const SIZE num_cols,const SIZE i) {
		for (SIZE k = 0; k < num_rows - i; ++k) std::memcpy(M_out[k],M[k+i],num_cols * sizeof(T));
	}
}
#endif // H_SUBCL_FROM_ITH_ROW_ON