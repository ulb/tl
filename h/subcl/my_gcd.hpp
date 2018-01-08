#ifndef H_SUBCL_MY_GCD
#define H_SUBCL_MY_GCD

namespace subcl {
	template <typename UNSIGNED_T>
	UNSIGNED_T GCD(UNSIGNED_T u, UNSIGNED_T v) {
	    while (v != 0) {
	        UNSIGNED_T r = u % v;
	        u = v;
	        v = r;
	    }
	    return u;
	}
	
	template <typename T,typename SIZE>
	T my_gcd(T * arr,const SIZE length_arr) {
		T result = arr[0];
		for (SIZE i = 1; i < length_arr; ++i) result = subcl::GCD(arr[i],result);
		return result;
	}
}
#endif // H_SUBCL_MY_GCD