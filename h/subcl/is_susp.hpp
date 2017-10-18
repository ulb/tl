#ifndef H_SUBCL_IS_SUSP
#define H_SUBCL_IS_SUSP

#include "../array/is_equal.hpp"

namespace subcl {
	// test if the 2-level polytope having slack-matrix S_new is a suspension
	template <typename T,typename SIZE>
	bool is_susp(T ** S_new,SIZE num_rows_S_new,SIZE num_cols_S_new) {
	    // For all rows i of the slack matrix M
	    // Partition the columns into F_0 = {j : M(i,j) = 0} and F_1 = {j : M(i,j) = 1}
	    // For all translation vectors t such that the vertices of F_1 - t are a subset of those of F_0
	    // Check that F_1 - t is a face by testing whether the intersection of all facets of P that contain it is exactly F_1 - t
	    int i,j,k,h,l,t;
	    bool flag = false;
	    bool is_contained,found,is_subset;

	    /*printf("   (%d,%d)\n",num_rows_S_new,num_cols_S_new);
	    for (i = 0; i < num_rows_S_new; ++i){
	        printf("   ");
	        for (j = 0; j < num_cols_S_new; ++j)
	            printf("%d",S_new[i][j]);
	        printf("\n");
	    }
	    printf("\n");*/

	    for (i = 0; i < num_rows_S_new; ++i) {
			SIZE * zeros_idx, * ones_idx;
	        alloc(zeros_idx,num_cols_S_new,T); 
	        alloc(ones_idx,num_cols_S_new,T);
	        SIZE num_zeros, num_ones;
	        num_zeros = 0;
	        num_ones = 0;
	        // Count zeroes and ones in the row and record their positions
	        for (j = 0; j < num_cols_S_new; ++j) {
	            if (S_new[i][j] == 1) {
	                ones_idx[num_ones] = j;
	                num_ones++;
	            }
	            else {
	                zeros_idx[num_zeros] = j;
	                num_zeros++;
	            }
	        }

	        for (j = 0; j < num_ones; ++j) {
	            for (k = 0; k < num_zeros; ++k) {
	                T * char_F1;
	                alloc(char_F1,num_cols_S_new,T);
	                for (l = 0; l < num_cols_S_new; ++l)
	                    char_F1[l] = 1;
	                is_contained = true;
	                for (l = 0; l < num_ones && is_contained; ++l) {
	                    T * translated_F1_point;
	                    alloc(translated_F1_point,num_rows_S_new,T);
	                    for (h = 0; h < num_rows_S_new; ++h)
	                        translated_F1_point[h] = S_new[h][ones_idx[l]] + S_new[h][zeros_idx[k]] - S_new[h][ones_idx[j]];
	                    found = false;
	                    for (h = 0; h < num_zeros && !found; ++h) {
	                        T * F0_point;
	                        alloc(F0_point,num_rows_S_new,T);
	                        for (t = 0; t < num_rows_S_new; t++)
	                            F0_point[t] = S_new[t][zeros_idx[h]];
	                        found = array::is_equal(translated_F1_point,F0_point,num_rows_S_new);
	                        free(F0_point);
	                        if (found)
	                            char_F1[zeros_idx[h]] = 0;
	                    }
	                    free(translated_F1_point);
	                    is_contained &= found;  
	                }
	                
	                if (is_contained) {
	                    T * intersect_rows_containing_F1;
	                    alloc(intersect_rows_containing_F1,num_cols_S_new,T);
	                    for (l = 0; l < num_cols_S_new; ++l)
	                        intersect_rows_containing_F1[l] = 0;
	                    for (h = 0; h < num_rows_S_new; ++h) {
	                        is_subset = true;
	                        for (l = 0; l < num_cols_S_new && is_subset; ++l) {
	                            if (S_new[h][l] == 1)
	                                is_subset = (char_F1[l] == 1);
	                        }
	                        if (is_subset) {
	                            for (l = 0; l < num_cols_S_new; ++l) {
	                                if ((S_new[h][l] == 1) || (intersect_rows_containing_F1[l] == 1))
	                                    intersect_rows_containing_F1[l] = 1;
	                                else intersect_rows_containing_F1[l] = 0;
	                            }
	                        }
	                    }
	                    flag = array::is_equal(intersect_rows_containing_F1,char_F1,num_cols_S_new);
	                    free(intersect_rows_containing_F1);  
	                }
	                free(char_F1);
	            }
	        }
	        free(zeros_idx);
	        free(ones_idx);
	    }
	    return flag;
	}
}

#endif // H_SUBCL_IS_SUSP