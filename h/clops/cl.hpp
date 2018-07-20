#ifndef H_CLOPS_CL
#define H_CLOPS_CL


#include "array/pack.hpp"
#include "array/is_all_zeros.hpp"
#include "clops/fast_discreteconvexhull_cl.hpp"
#include "clops/compatibility_cl.hpp"
#include "clops/lexmax_symmetric_cl.hpp"

namespace clops {

	template<typename XT, typename S, typename T, typename W, typename SIZE>
	bool cl ( T * I, W *& B_64, W * CI_64, W * CI_big_64, T *& CI, T * CI_big, W ** IM_64, XT& X, XT& Xr, S& slabs,const T * const * const orbits, const SIZE num_autom_base) {

		clops::fast_discreteconvexhull_cl(I, B_64, CI_big_64, Xr.ps_64, Xr.sp_64_comp, Xr.finalsize, slabs.n_rows, Xr.n_rows_big_64, slabs.n_rows_64);

		if ( array::is_all_zeros_64(CI_big_64, Xr.e1) ) {

			array::unpack64(CI_big+(Xr.e1/64)*64,Xr.compsize-(Xr.e1/64)*64,CI_big_64+(Xr.e1/64));
			std::memcpy(CI,CI_big+Xr.e1,Xr.finalsize * sizeof(T));
			array::pack64(CI,Xr.finalsize,CI_64,Xr.n_rows_64);

			if ( clops::fast_compatibility_cl(CI, CI_64, IM_64, Xr.finalsize) ) {
				clops::lexmax_symmetric_cl(CI, X, orbits, num_autom_base);
				return true;
			}
			return false;
		}
		return false;
	}

}

#endif // H_CLOPS_CL
