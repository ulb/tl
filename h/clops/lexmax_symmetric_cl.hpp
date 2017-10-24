#ifndef H_CLOPS_LEXMAX_SYMMETRIC_CL
#define H_CLOPS_LEXMAX_SYMMETRIC_CL

#include <cstring> // std::equal

#include "alloc.hpp"
#include "array/is_equal.hpp"
#include "array/is_all_ones.hpp"
#include "array/is_all_zeros.hpp"
#include "array/index_in_collection.hpp"
#include "clops/get_ones.hpp"
#include "clops/is_preccurlyeq.hpp"

namespace clops {
	template <typename T,typename SIZE>
	bool is_outside_X(T ** orbits_i,T ** index_H,T* indices_H, T * A_sym, T * A_indices,const SIZE num_A_indices,const SIZE length_A,const T D) {
		std::memset(A_sym+1,0,(length_A-1) * sizeof(T));
		for (SIZE j = 1; j < num_A_indices; ++j) {
		    if (orbits_i[A_indices[j]][0] == 0) return true;
		    else A_sym[array::index_in_collection(index_H,indices_H,orbits_i[A_indices[j]],length_A,D)] = 1;
		}
		return false;
	}

	// compute the lexmax symmetric copy of a set A
	template <typename T,typename SIZE>
	void lexmax_symmetric_cl(T * A,T *& symcl,T ** index_H, T* indices_H, const SIZE length_A,T *** orbits,const SIZE num_autom_base,const T D) {
	    SIZE i;

	    std::memcpy(symcl,A,length_A * sizeof(T));

	    if (!array::is_all_ones(A,length_A)) {
	    	T * A_indices;
    		SIZE num_A_indices = clops::get_ones(A,length_A,A_indices);

	        T * A_sym;
	        alloc(A_sym,length_A,T);
	        A_sym[0] = 1;

	        if (!array::is_all_zeros(A + 1,length_A - 1)) {
	            for (i = 0; i < num_autom_base; ++i) {
	                if (!clops::is_outside_X(orbits[i],index_H,indices_H,A_sym,A_indices,num_A_indices,length_A,D))
	                    if (clops::is_preccurlyeq(symcl,A_sym,length_A)) std::memcpy(symcl,A_sym,length_A * sizeof(T));
	            }
	        }
	        free(A_sym);
	        free(A_indices);
	    }
	}
}

#endif // H_CLOPS_LEXMAX_SYMMETRIC_CL
