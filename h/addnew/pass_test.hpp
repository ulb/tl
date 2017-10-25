#ifndef H_ADDNEW_PASS_TEST
#define H_ADDNEW_PASS_TEST

namespace addnew {
	/**
	 * Counts the number of vertices in each facet. Must be <= to the number of
	 * vertices of the base. Should be replaced by checking that each face that
	 * is not the base is lexicographically larger than the base.
	 */
	template <typename T,typename SIZE>
	bool pass_test(T** S_new, const SIZE num_rows_S_new, const SIZE num_cols_S_new, const SIZE num_cols_S) {
	    for (int i = 0; i < num_rows_S_new; ++i) {
	        SIZE num_vertices_Fi = 0;
	        for (int j = 0; j < num_cols_S_new; ++j) if (S_new[i][j] == 0) ++num_vertices_Fi;
	        if (num_vertices_Fi > num_cols_S) return false;
	    }
	    return true;
	}
}

#endif // H_ADDNEW_PASS_TEST
