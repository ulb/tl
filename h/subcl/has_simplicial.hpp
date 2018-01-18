#ifndef H_SUBCL_HAS_SIMPLICIAL
#define H_SUBCL_HAS_SIMPLICIAL


namespace subcl {
	template <typename P>
	bool has_simplicial(P& polytope) {
		const auto S = polytope.matrix;
		const auto rows = polytope.rows;
		const auto cols = polytope.columns;
		const auto D = polytope.dimension;
		size_t num_zeros;

        for (size_t i = 0; i < rows; ++i) {
	        num_zeros = 0;
	        for (size_t j = 0; j < cols; ++j) if (S[i][j] == 0) ++num_zeros;
	        if (num_zeros == D) return true;
	    }
        return false;
	}
}

#endif // H_SUBCL_HAS_SIMPLICIAL