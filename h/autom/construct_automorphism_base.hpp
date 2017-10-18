#ifndef H_AUTOM_CONSTRUCT_AUTOMORPHISM_BASE
#define H_AUTOM_CONSTRUCT_AUTOMORPHISM_BASE

#include <nauty.h>
#include <naugroup.h>

namespace autom {
	DYNALLSTAT(int,allp,allp_sz);
	DYNALLSTAT(int,id,id_sz);

	// Recursive routine used by allgroup.
	template <typename T,typename SIZE>
	static void my_groupelts(levelrec *lr, int n, int level, void (*action)(T*,T,std::vector<std::vector<T> >&,SIZE &),int *before, int *after, int *id,std::vector<std::vector<T> >& automorphism_base, SIZE & num_tot) {
	    int i,j,orbsize;
	    int *p,*cr;
	    cosetrec *coset;
	    
	    coset = lr[level].replist;
	    orbsize = lr[level].orbitsize;
	    
	    for (j = 0; j < orbsize; ++j) {
	        cr = (coset[j].rep == NULL ? NULL : coset[j].rep->p);
	        if (before == NULL) p = cr;
	        else if (cr == NULL) p = before;
	        else {
	            p = after;
	            for (i = 0; i < n; ++i) p[i] = cr[before[i]];
	        }
	        
	        if (level == 0)
	            (*action)((p == NULL ? id : p),n,automorphism_base,num_tot);
	        else my_groupelts(lr,n,level-1,action,p,after+n,id,automorphism_base,num_tot);
	    }
	}

	// Call action(p,n) for every element of the group, including the identity.
	template <typename T,typename SIZE>
	void my_allgroup(grouprec *grp, void (*action)(T*,T,std::vector<std::vector<T> >&,SIZE &),std::vector<std::vector<T> >& automorphism_base, SIZE & num_tot) {
	    int i,depth,n;
	    
	    depth = grp->depth;
	    n = grp->n;
	    
	    DYNALLOC1(int,id,id_sz,n,"malloc");
	    for (i = 0; i < n; ++i)
	        id[i] = i;
	    
	    if (depth == 0) {
	        (*action)(id,n,automorphism_base,num_tot);
	        return;
	    }
	    
	    DYNALLOC1(int,allp,allp_sz,n*depth,"malloc");
	    my_groupelts(grp->levelinfo,n,depth-1,action,NULL,allp,id,automorphism_base,num_tot);
	}

	template <typename T,typename SIZE>
	void writeautom(T *p, T n,std::vector<std::vector<T> >& automorphism_base, SIZE & num_tot) {
	    std::vector<T> permutation(n);
	    for (int i = 0; i < n; ++i) permutation[i] = p[i];
	    automorphism_base.push_back(permutation);
	    permutation.clear();
	    num_tot++;
	}

	template <typename T,typename SIZE,typename SIZE_TP>
	void construct_automorphism_base(T ** S,const SIZE num_rows,const SIZE num_cols, const SIZE_TP n, const SIZE_TP m, std::vector<std::vector<T> >& automorphism_base, SIZE & num_autom_base) {
	    
	    int i,j;
	    // Initializations for Nauty
	    DYNALLSTAT(T,lab,lab_sz);
	    DYNALLSTAT(T,ptn,ptn_sz);
	    DYNALLSTAT(T,orbits,orbits_sz);
	    DYNALLSTAT(graph,g,g_sz);
	    static DEFAULTOPTIONS_GRAPH(options);
	    statsblk stats;
	    
	    grouprec *group;
	    /* The following cause nauty to call two procedures which
	     store the group information as nauty runs. */
	    options.userautomproc = groupautomproc;
	    options.userlevelproc = grouplevelproc;
	    
	    // Select option for custom partition
	    options.defaultptn = FALSE;
	    
	    //n = num_rows + num_cols;
	    //m = SETWORDSNEEDED(n);
	    nauty_check(WORDSIZE,m,n,NAUTYVERSIONID);
	    
	    // Allocate memory for the graph
	    DYNALLOC1(T,lab,lab_sz,n,"malloc");
	    DYNALLOC1(T,ptn,ptn_sz,n,"malloc");
	    DYNALLOC1(T,orbits,orbits_sz,n,"malloc");
	    DYNALLOC2(graph,g,g_sz,n,m,"malloc");
	    
	    // Empty the graph
	    EMPTYGRAPH(g,m,n);
	    
	    // Build the custom partition for the graph
	    for (i = 0; i < n; ++i) {
	        lab[i] = i;
	        if (i != num_rows-1 && i != n-1)
	            ptn[i] = 1;
	        else ptn[i] = 0;
	    }
	    
	    // Build the edges of the nonincidence graph
	    // Loop through all the entries of the slack matrix and add an edge when there is a one
	    for (i = 0; i < num_rows; ++i) {
	        for (j = 0; j < num_cols; ++j)
	            if (S[i][j] == 1) ADDONEEDGE(g,i,j+num_rows,m);
	    }
	    
	    //determine the automorphism group of the graph with nauty
	    densenauty(g,lab,ptn,orbits,&options,&stats,m,n,NULL);
	    
	    /* Get a pointer to the structure in which the group information
	     has been stored.  If you use TRUE as an argument, the
	     structure will be "cut loose" so that it won’t be used
	     again the next time nauty() is called.  Otherwise, as
	     here, the same structure is used repeatedly. */
	    group = groupptr(FALSE);
	    /* Expand the group structure to include a full set of coset
	     representatives at every level.  This step is necessary
	     if allgroup() is to be called. */
	    makecosetreps(group);
	    /* Call the procedure writeautom() for every element of the group.
	     The first call is always for the identity. */
	    //num_autom_base = 0;
	    my_allgroup(group,writeautom,automorphism_base,num_autom_base);
	    
	    // Clean up
	    DYNFREE(allp,allp_sz);
	    DYNFREE(id,id_sz);
	    DYNFREE(lab,lab_sz);
	    DYNFREE(ptn,ptn_sz);
	    DYNFREE(orbits,orbits_sz);
	    DYNFREE(g,g_sz);
	}
}

#endif // H_AUTOM_CONSTRUCT_AUTOMORPHISM_BASE