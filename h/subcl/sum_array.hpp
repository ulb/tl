#ifndef H_SUBCL_SUM_ARRAY
#define H_SUBCL_SUM_ARRAY

namespace subcl {
	template <typename T,typename SIZE>
	T sum_array(T * arr,const SIZE length_arr) {
		T sum_val = 0;
		for (SIZE i = 0; i < length_arr; ++i) sum_val += arr[i];
		return sum_val;
	}
}
#endif // H_SUBCL_SUM_ARRAY