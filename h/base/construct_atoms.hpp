#ifndef H_BASE_CONSTRUCT_ATOMS
#define H_BASE_CONSTRUCT_ATOMS

#include <stdio.h>
#include <iostream>

namespace base {
	template <typename T>
	T construct_atoms(T *** atoms,FILE * my_inputfile, const T D, const T verbose) {
	    if (verbose == 2) printf("\n");
	    T it, i, j;
	    char temp_s;
	    it = 0;
	    i = 0;
	    j = 0;  
	    while ((fscanf(my_inputfile, "%c", &temp_s) != EOF)) {
	        if ((temp_s == '0') || (temp_s == '1')){
	            atoms[it][i][j] = temp_s - '0';
	            if (verbose == 2) printf("%d",atoms[it][i][j]);
	            ++j;
	        }
	        else if (temp_s == '\n'){
	            if (verbose == 2) printf("\n");
	            ++i;
	            j = 0;
	        }
	        else {
	            if (verbose == 2) printf("-");
	            i = -1;
	            it++;
	        }
	    }
	    if (verbose == 2) printf("\n");
	    return it;
	}
}

#endif // H_BASE_CONSTRUCT_ATOMS