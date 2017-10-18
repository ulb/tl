#ifndef H_TWOLVL_CANONICIZE
#define H_TWOLVL_CANONICIZE

#include <nauty.h>

namespace twolvl {
	// output the canonical form of a slack matrix
	template <typename T,typename SIZE,typename SIZE_TP,typename NT_T>
	void canonicize(T ** S,const SIZE num_rows,const SIZE num_cols, const SIZE_TP n, const SIZE_TP m, NT_T * cg_vec) {
	    // Initializations for Nauty
	    DYNALLSTAT(T,lab,lab_sz);
	    DYNALLSTAT(T,ptn,ptn_sz);
	    DYNALLSTAT(T,orbits,orbits_sz);
	    DYNALLSTAT(T,map,map_sz);
	    DYNALLSTAT(graph,g,g_sz);
	    DYNALLSTAT(graph,cg,cg_sz);
	    static DEFAULTOPTIONS_GRAPH(options);
	    statsblk stats;
	    
	    // Select option for canonical labelling
	    options.getcanon = TRUE;
	    
	    // Select option for custom partition
	    options.defaultptn = FALSE;
	    
	    nauty_check(WORDSIZE,m,n,NAUTYVERSIONID);
	    
	    // Allocate memory for the graph
	    DYNALLOC1(T,lab,lab_sz,n,"malloc");
	    DYNALLOC1(T,ptn,ptn_sz,n,"malloc");
	    DYNALLOC1(T,orbits,orbits_sz,n,"malloc");
	    DYNALLOC1(T,map,map_sz,n,"malloc");
	    DYNALLOC2(graph,g,g_sz,n,m,"malloc");
	    DYNALLOC2(graph,cg,cg_sz,n,m,"malloc");
	    
	    // Empty the graph
	    EMPTYGRAPH(g,m,n);
	    
	    // Build the custom partition for the graph
	    for (int i = 0; i < num_rows+num_cols; ++i) {
	        lab[i] = i;
	        if (i != num_rows-1 && i != num_rows+num_cols-1)
	            ptn[i] = 1;
	        else ptn[i] = 0;
	    }
	    
	    // Build the edges of the nonincidence graph
	    // Loop through all the entries of the slack matrix and add an edge when there is a one
	    for (int i = 0; i < num_rows; ++i) {
	        for (int j = 0; j < num_cols; ++j)
	            if (S[i][j] == 1) ADDONEEDGE(g,i,j+num_rows,m);
	    }
	    
	    // Obtain canonical graph from Nauty
	    densenauty(g,lab,ptn,orbits,&options,&stats,m,n,cg);
	    
	    for (size_t k = 0; k < m*n; ++k) cg_vec[k] = cg[k];
	    
	    // Clean up
	    DYNFREE(lab,lab_sz);
	    DYNFREE(ptn,ptn_sz);
	    DYNFREE(orbits,orbits_sz);
	    DYNFREE(map,map_sz);
	    DYNFREE(g,g_sz);
	    DYNFREE(cg,cg_sz);
	}
}

#endif // H_TWOLVL_CANONICIZE