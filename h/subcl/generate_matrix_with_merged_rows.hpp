#ifndef H_SUBCL_GENERATE_MATRIX_WITH_MERGED_ROWS
#define H_SUBCL_GENERATE_MATRIX_WITH_MERGED_ROWS

namespace subcl {
	template <typename T,typename SIZE>
	void merge_rows(T * M_out_row,T ** M,const SIZE num_cols,SIZE * idx_rows_subM,const SIZE num_rows_subM) {
		SIZE k,h;
		SIZE num_unique_cols = 1;
		void * mem_unique_cols;
		T * subcol;
		alloc(subcol,num_rows_subM,T);
		T ** unique_cols;
		alloc_matrix(mem_unique_cols,unique_cols,num_cols,num_rows_subM);
		SIZE test_idx; 
		for (k = 0; k < num_rows_subM; ++k) subcol[k] = M[idx_rows_subM[k]][0];
		std::memcpy(unique_cols[0],subcol,num_rows_subM * sizeof(T));
		M_out_row[0] = 0;
		for (h = 1;  h < num_cols; ++h) {
			for (k = 0; k < num_rows_subM; ++k) subcol[k] = M[idx_rows_subM[k]][h];
			test_idx = subcl::idx_in_collection(unique_cols,subcol,num_unique_cols,num_rows_subM);
			M_out_row[h] = (T)test_idx;
			if (test_idx == num_unique_cols) std::memcpy(unique_cols[num_unique_cols++],subcol,num_rows_subM * sizeof(T));
		}
		free(subcol);
		free(mem_unique_cols);
	}

	
	template <typename T,typename SIZE>
	void generate_matrix_with_merged_rows(T ** M_out,T ** M,const SIZE num_rows,const SIZE num_cols,const SIZE a,const SIZE b) {
		SIZE k;
		for (k = 0; k < a; ++k) std::memcpy(M_out[k],M[k],num_cols * sizeof(T));
		for (++k; k < b; ++k) std::memcpy(M_out[k],M[k],num_cols * sizeof(T));
		for (; k < num_rows-1; ++k) std::memcpy(M_out[k],M[k+1],num_cols * sizeof(T));

		SIZE * idx_rows_subM;
		alloc(idx_rows_subM,2,T);
		idx_rows_subM[0] = a;
		idx_rows_subM[1] = b;
		subcl::merge_rows(M_out[a],M,num_cols,idx_rows_subM,2);
		free(idx_rows_subM);

		// alloc(idx_rows_subM,num_rows-a-2,T);
		// for (++k; k < b; ++k) idx_rows_subM[k-a-1] = k;
		// for (; k < num_rows-1; ++k) idx_rows_subM[k-a-1] = k+1;
		// subcl::merge_rows(M_out[a+1],M,num_cols,idx_rows_subM,num_rows-a-2);
		// free(idx_rows_subM);

		// std::cout << "\e[94m" << a << "\e[0m" << std::endl;

		// for (SIZE s = 0; s <= a; ++s) {
		// 	for (SIZE t = 0; t < num_cols; ++t) std::cout << "\e[93m" << M_out[s][t];
		// 	std::cout << "\e[0m" << std::endl;
		// }

		// // for (k = 0; k < num_cols; ++k) std::cout << "\e[93m" << M_out[a][k];
		// // std::cout << "\e[0m" << std::endl;

		// alloc(idx_rows_subM,num_rows-a-2,T);
		// for (k = a+1; k < b; ++k) {
		// 	idx_rows_subM[k-a-1] = k;
		// 	std::cout << "\e[94m" << k << ' ';
		// }
		// for (; k < num_rows-1; ++k) {
		// 	std::cout << "\e[94m" << k+1 << ' ';
		// 	idx_rows_subM[k-a-1] = k+1;
		// }
		// std::cout << "\e[0m" << std::endl;
		// for (SIZE h = 0; h < num_rows-a-2; ++h) {
		// 	for (k = 0; k < num_cols; ++k) std::cout << "\e[91m" << M[idx_rows_subM[h]][k];
		// 	std::cout << "\e[0m" << std::endl;
		// }
		// subcl::merge_rows(M_out[a+1],M,num_cols,idx_rows_subM,num_rows-a-2);
		// free(idx_rows_subM);

		// for (SIZE s = 0; s <= a+1; ++s) {
		// 	for (SIZE t = 0; t < num_cols; ++t) std::cout << "\e[93m" << M_out[s][t];
		// 	std::cout << "\e[0m" << std::endl;
		// }

	}

}
#endif // H_SUBCL_GENERATE_MATRIX_WITH_MERGED_ROWS