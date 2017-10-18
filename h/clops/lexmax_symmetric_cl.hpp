#ifndef H_CLOPS_LEXMAX_SYMMETRIC_CL
#define H_CLOPS_LEXMAX_SYMMETRIC_CL

#include <cstring> // std::equal

#include "../alloc.hpp"
#include "../array/is_equal.hpp"
#include "../array/is_all_ones.hpp"
#include "../array/index_in_collection.hpp"

namespace clops {
	// compute the lexmax symmetric copy of a set A
	template <typename T,typename SIZE>
	void lexmax_symmetric_cl(T *& A,T *& symcl,T ** ground_set_H,const SIZE length_A,T *** orbits,const SIZE num_autom_base,const T D) {
	    int i,j;
	    
	    std::memcpy(symcl,A,length_A * sizeof(T));
	    
	    if (!(array::is_all_ones(A,length_A))) {
	        bool is_outside_X;
	        int * A_sym;
	        alloc(A_sym,length_A,int);
	        
	        A_sym[0] = 1;
	        std::memset(A_sym+1,0,(length_A-1) * sizeof(T));
	        
	        if (!array::is_equal(A,A_sym,length_A)) {
	            for (i = 0; i < num_autom_base; ++i) {
	                std::memset(A_sym+1,0,(length_A-1) * sizeof(T));
	                is_outside_X = false;
	                for (j = 1; j < length_A && !(is_outside_X); ++j) {
	                    if (A[j] == 1) {
	                        if (orbits[j][i][0] == 0) is_outside_X = true;
	                        else A_sym[array::index_in_collection(ground_set_H,orbits[j][i],length_A,D)] = 1;
	                    }
	                }
	                if (!(is_outside_X)) {
	                    if (clops::is_preccurlyeq(symcl,A_sym,length_A))
	                        std::memcpy(symcl,A_sym,length_A * sizeof(T));
	                }
	            }
	        }
	        else std::memcpy(symcl,A_sym,length_A * sizeof(T));
	        free(A_sym);
	    }
	}
}

#endif // H_CLOPS_LEXMAX_SYMMETRIC_CL