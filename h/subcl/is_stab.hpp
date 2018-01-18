#ifndef H_SUBCL_IS_STAB
#define H_SUBCL_IS_STAB


namespace subcl {
	template <typename P>
	bool is_stab(P& polytope) {
		const auto S = polytope.matrix;
		const auto rows = polytope.rows;
		const auto cols = polytope.columns;
		const auto D = polytope.dimension;
		size_t num_zeros;
		
        for (size_t i = 0; i < cols; ++i) {
            num_zeros = 0;
            for (size_t j = 0; j < rows; ++j) if (S[j][i] == 0) ++num_zeros;
            if (num_zeros == D) return true;
        }
        return false;
	}
}

#endif // H_SUBCL_IS_STAB