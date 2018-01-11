#ifndef H_SUBCL_IS_CS
#define H_SUBCL_IS_CS


namespace subcl {
	template <typename P>
	bool is_cs(P& polytope) {
		const auto S = polytope.matrix;
		const auto rows = polytope.rows;
		const auto cols = polytope.columns;
		size_t num_ones;

        for (size_t i = 0; i < rows; ++i) {
            num_ones = 0;
            for (size_t j = 0; j < cols; ++j) if (S[i][j] == 1) num_ones++;
            if (!(num_ones == cols/2)) return false;
        }
        return true;
	}
}

#endif // H_SUBCL_IS_CS