#ifndef H_TWOLVL_IS_LISTED
#define H_TWOLVL_IS_LISTED

#include "../array/is_equal.hpp"

namespace twolvl {
	// Should use MTF trie here MTF!!
	template <typename A,typename NT_T, typename T,typename SIZE_TP>
	bool is_listed(A& atoms, NT_T* S,const T rows, const T cols, const SIZE_TP length) {
	    for (auto& atom : atoms) {
	        if (rows == atom.rows && cols == atom.columns && array::is_equal(S, atom.cg, length)) return true;
	    }
	    return false;
	}
}

#endif // H_TWOLVL_IS_LISTED
