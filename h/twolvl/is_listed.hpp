#ifndef H_TWOLVL_IS_LISTED
#define H_TWOLVL_IS_LISTED

#include "../array/is_equal.hpp"

namespace twolvl {
	template <typename T,typename SIZE,typename NT_T,typename SIZE_TP>
	bool is_listed(NT_T ** list_2L,T * list_hash,const SIZE list_length,NT_T * canonical_S,const T hash_S,const SIZE_TP length_canonical_S) {
	    for (int i = 0; i < list_length; ++i) {
	        if (hash_S == list_hash[i]) 
	            if (array::is_equal(canonical_S,list_2L[i],length_canonical_S)) return true;
	    }
	    return false;
	}
}

#endif // H_TWOLVL_IS_LISTED