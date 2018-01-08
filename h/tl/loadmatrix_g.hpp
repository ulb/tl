#ifndef H_TL_LOADMATRIX_G
#define H_TL_LOADMATRIX_G

namespace tl {

	template <typename I>
	int loadmatrix_g(I& istream) {

		char c; istream >> c ;
		if ( c == '0' ) return 0 ;
		else if ( c == '1' ) return 1 ;
		else return -1 ;

	}

}

#endif // H_TL_LOADMATRIX_G
