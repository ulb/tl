#ifndef H_CLOPS_CL
#define H_CLOPS_CL

#include "array/pack.hpp"
//#include "clops/discreteconvexhull_cl.hpp"
#include "clops/fast_discreteconvexhull_cl.hpp"
#include "clops/compatibility_cl.hpp"
#include "clops/lexmax_symmetric_cl.hpp"

namespace clops {

	template<typename XT, typename S, typename T, typename W, typename SIZE>
	bool cl ( T *I, W *B64, W *CI64, T*& CI, W **IM64, XT& X, S& slabs, SIZE n_cols_64, T **orbits, SIZE num_autom_base ) {

		//clops::discreteconvexhull_cl(I,B,CI,X.ps,X.finalsize,slabs.rows);
		//clops::fast_discreteconvexhull_cl(I, B64, CI_big_64, X.ps_64, X.sp_64_comp, X.finalsize, slabs.rows, X.n_rows_big_64, n_cols_64);
		clops::fast_discreteconvexhull_cl(I, B64, CI64, X.ps_64, X.sp_64_comp, X.finalsize, slabs.rows, X.n_rows_big_64, n_cols_64);
		//if ( !array::is_all_zeros_64(CI_big_64, X.e1) ) return false;
		//else {
			array::unpack64(CI, X.finalsize, CI64);
			//array::unpack64(CI_big+(X.e1/64)*64,X.compsize-(X.e1/64)*64,CI_big_64+(X.e1/64));
			//std::memcpy(CI,CI_big+X.e1,X.finalsize * sizeof(int));
			//array::pack64(CI,X.finalsize,CI64,n_rows_64);
			if ( clops::fast_compatibility_cl(CI, CI64, IM64, X.finalsize) ) {
			//if ( clops::compatibility_cl(CI,IM,X.finalsize) ) {
				clops::lexmax_symmetric_cl(CI, X.finalsize, orbits, num_autom_base);
				return true;
			}
			return false;
		//}
	}

}

#endif // H_CLOPS_CL
