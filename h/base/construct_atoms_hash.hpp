#ifndef H_BASE_CONSTRUCT_ATOMS_HASH
#define H_BASE_CONSTRUCT_ATOMS_HASH

#include <stdio.h>
#include <iostream>

namespace base {
	template <typename T>
	void construct_atoms_hash(T * atoms_hash,FILE * my_inputfile, const T D) {
	    T it, i, j;
	    char temp_s;
	    it = 0;
	    i = 0;
	    j = 0;  
	    while ((fscanf(my_inputfile, "%c", &temp_s) != EOF)) {
	        if ((temp_s == '0') || (temp_s == '1')) ++j;
	        else if (temp_s == '\n') {
	            if (i == 1) atoms_hash[it] = (j-1) << D;
	            ++i;
	            j = 0;
	        }
	        else {
	            atoms_hash[it] += (i-1);
	            i = -1;
	            it++;
	        }
	    }
	}
}

#endif // H_BASE_CONSTRUCT_ATOMS_HASH