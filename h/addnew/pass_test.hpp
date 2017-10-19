#ifndef H_ADDNEW_PASS_TEST
#define H_ADDNEW_PASS_TEST

namespace addnew {
	// check if the slack matrix S is already listed in LD; if not, add it to LD //int *& LD_hash, setword **& LD
	template <typename T,typename SIZE>
	bool pass_test(T ** S_new, const SIZE num_rows_S_new, const SIZE num_cols_S_new, const SIZE num_cols_S) {
	    int i,j;
	    SIZE num_vertices_Fi;
	    for (i = 0; i < num_rows_S_new; ++i) {
	        num_vertices_Fi = 0;
	        for (j = 0; j < num_cols_S_new; ++j)
	            if (S_new[i][j] == 0) num_vertices_Fi++;
	        if (num_vertices_Fi > num_cols_S) return false;
	    }
	    return true;
	}
}

#endif // H_ADDNEW_PASS_TEST