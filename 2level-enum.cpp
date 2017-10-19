#include <stdio.h>
#include <iostream>
#include <cstring> // std::memcpy, std::memset, std::fill
#include <chrono>

#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_RESET   "\x1b[0m"

#include "alloc.hpp"

#include "array/is_equal.hpp"
#include "array/index_in_collection.hpp"
#include "array/is_all_ones.hpp"
#include "nt/factor.hpp"
#include "nt/my_pow.hpp"

#include "linalg/my_inner_prod.hpp"
#include "linalg/my_matrix_prod.hpp"

#include "clops/is_preccurlyeq.hpp"
#include "clops/is_sqsubseteq.hpp"

#include "clops/inc.hpp"

#include "clops/discreteconvexhull_cl.hpp"
#include "clops/incompatibility_cl.hpp"
#include "clops/lexmax_symmetric_cl.hpp"

#include "linalg/generate_id_matrix.hpp"

#include "simpl/slack_matrix_simplicial_2L.hpp"
#include "simpl/push_simplicial_core.hpp"

#include "twolvl/construct_slack_matrix.hpp"

#include "twolvl/checksimplicialcore.hpp"
#include "twolvl/extractM.hpp"
#include "linalg/invertM.hpp"

#include "twolvl/hash2size.hpp"
#include "twolvl/canonicize.hpp"
#include "twolvl/is_listed.hpp"
#include "twolvl/istwolevelpolytope.hpp"

#include "addnew/pass_test.hpp"
#include "addnew/to_list.hpp"

#include "base/construct_atoms_hash.hpp"
#include "base/construct_atoms.hpp"
#include "base/construct_atoms_cg.hpp"

#include "base/construct_facets_base.hpp"
#include "base/construct_d_aut_collection.hpp"

#include "base/construct_base_V.hpp"
#include "base/construct_base_H.hpp"
#include "base/construct_ground_V.hpp"
#include "base/construct_ground_H.hpp"

#include "base/construct_orbits.hpp"
#include "base/construct_slabs.hpp"
#include "base/construct_slab_point_sat.hpp"
#include "base/construct_incompatibility_adjM.hpp"
            

// Main function
int main (int argc, const char* argv[]) {
    if (argc < 2) {
        printf("ERROR: Not enough input arguments.\n");
        return 1;
    }
    if (argc > 5) {
        printf("ERROR: insert 1 or 5 input arguments.\n");
        return 1;
    }
    
    int D = atoi(argv[1]);
    
    int n_atoms, n_LD;
    if (D == 3) {
        n_atoms = 2;
        n_LD = 5;
    }
    else if (D == 4) {
        n_atoms = 5;
        n_LD = 19;
    }
    else if (D == 5) {
        n_atoms = 19;
        n_LD = 106;
    }
    else if (D == 6) {
        n_atoms = 106;
        n_LD = 1150;
    }
    else if (D == 7) {
        n_atoms = 1150;
        n_LD = 27292;
    }
    else if (D == 8) {
        n_atoms = 27292;
        n_LD = 2000000;
    }
    else {
        printf("ERROR: D out of bounds.\n");
        return 1;
    }

    unsigned int first_base, last_base;
    if (argc > 2) {
        if (argv[2][0] != '-')  first_base = atoi(argv[2]);
        else first_base = 0;
        if (argc == 4) {
            if (argv[3][0] != '-') last_base = atoi(argv[3]);
            else last_base = n_atoms - 1;
        }
        else last_base = n_atoms - 1;
    }
    else {
        first_base = 0;
        last_base = n_atoms - 1;
    }


    int verbose;
    if (argc == 5) verbose = atoi(argv[4]);
    else verbose = 0;
        
    using clock_type = std::chrono::time_point<std::chrono::system_clock>;
    using my_clock = std::chrono::system_clock;
    clock_type begin_nextcl,end_nextcl;
    clock_type begin_slack_matrix,end_slack_matrix;
    clock_type begin_2L_test,end_2L_test;
    clock_type begin_skip_test,end_skip_test;
    clock_type begin_per_base,end_per_base;
    clock_type begin_tot_time,end_tot_time;
    clock_type begin_simplicial,end_simplicial;
    std::chrono::duration<double> time_nextcl,time_slack_matrix,time_2L_test,time_skip_test;
    std::chrono::duration<double> time_per_base,time_simplicial,tot_elapsed_time;
    double tot_nextcl = 0;
    double tot_slack_matrix = 0;
    double tot_2L_test = 0;

    // Counters for the classes of 2L polytopes
    unsigned int simplicial_facet = 0; // 2L polytopes with a simplicial facet
    unsigned int cs = 0; // Centrally-symmetric 2L polytopes
    unsigned int stab = 0; // Stable set polytopes of a perfect graph
    unsigned int n_suspensions = 0; // 2L suspensions
    unsigned int n_polar = 0; // Polar 2L polytopes
    
    begin_tot_time = my_clock::now();
    
    FILE * my_inputfile, * my_outputfile;
    
    char inputfile_Name[6],outputfile_Name[20];
    sprintf(inputfile_Name,"%dd.txt",D-1);
    my_inputfile = fopen(inputfile_Name,"r");
    
    int i, j, it; // iterators

    int total_2level = 0;
    int N_closed_sets = 0;
    
    int * LD_hash;
    setword ** LD;
    alloc(LD_hash,n_LD,int);
    alloc(LD,n_LD,setword *);
    
    int *** atoms;
    int * atoms_hash;
    
    alloc(atoms_hash,n_atoms,int);
    alloc(atoms,n_atoms,int**);
    
    printf("Preprocessing sizes of slack matrices... ");
    base::construct_atoms_hash(atoms_hash,my_inputfile,D);
    printf("OK\n");
    
    int num_rows, num_cols;
    // allocating exact memory for each slack matrix of 2-level (D-1)-polytope
    for (i = 0; i < n_atoms; ++i) {
        twolvl::hash2size(atoms_hash[i],num_rows,num_cols,D);
        alloc(atoms[i],num_rows,int*);
        for (j = 0; j < num_rows; ++j) alloc(atoms[i][j],num_cols,int);
    }

    rewind(my_inputfile);
    
    printf("Reading all %d-dimensional 2-level polytopes... ",D-1);
    it = base::construct_atoms(atoms,my_inputfile,D,verbose);
    printf("OK\nNumber of polytopes read = %d\n",it);

    fclose(my_inputfile);
    
    printf("Computing canonical forms for all nonincidence graphs... ");
    setword ** atoms_cg;
    alloc(atoms_cg,n_atoms,setword *);
    base::construct_atoms_cg(atoms_cg,atoms,atoms_hash,n_atoms,D);
    printf("OK\n");


    printf("Processing bases...\n");

    if ((first_base == 0) && (last_base == n_atoms - 1)) sprintf(outputfile_Name,"%dd.txt",D);
    else sprintf(outputfile_Name,"%dd_%d-%d.txt",D,first_base,last_base);
    my_outputfile = fopen(outputfile_Name,"w+");
    
    int n_simplicial = 0;
    int current_LD = 0;
    // use the characterization of simplicial 2-level polytopes
    if (first_base == 0) {
        printf("\nBase #1:\n");
        printf("Constructing slack-matrices of simplicial 2-level polytopes... ");
        begin_simplicial = my_clock::now();
        int K;
        int ** S_new;
        int num_rows_S_new,num_cols_S_new;

        for (auto N : nt::factor(D)) {
            K = D/N;
            simpl::slack_matrix_simplicial_2L(K,N,S_new,num_rows_S_new,num_cols_S_new);
            simpl::push_simplicial_core(S_new,num_rows_S_new,num_cols_S_new,atoms[0],D);
            addnew::to_list(S_new,num_rows_S_new,num_cols_S_new,my_outputfile,LD_hash,LD,current_LD,D,verbose,simplicial_facet,cs,stab,n_suspensions,n_polar);
            n_simplicial++;
            for (i = 0; i < num_rows_S_new; ++i) free(S_new[i]);
            free(S_new);
        } 

        end_simplicial = my_clock::now();
        time_simplicial = end_simplicial - begin_simplicial;
        printf("OK\n");
        printf("Number of simplicial polytopes generated = %d\n",n_simplicial);
        printf("Elapsed time for base #1 = %lfs\n", time_simplicial.count());
    }
    else first_base -= 1;

    it = 0;
    int n_base;
    // Main loop: loop through all the bases (except the simplex)
    for (it = first_base+1; it <= last_base; it++) {
        begin_per_base = my_clock::now();
        int num_rows_S, num_cols_S;
        twolvl::hash2size(atoms_hash[it],num_rows_S,num_cols_S,D);
        
        n_base = it + 1;
        printf("\nBase #%d:\n",n_base);
        
        printf("Simplicial core? ");
        if (twolvl::checksimplicialcore(atoms[it],D)) printf("OK\n");
        else {
            printf("Fail\n");
            return 1;
        }
        
        // Extract embedding transformation matrix M_d(0) and invert it
        int ** M, ** Minv;
        alloc(M,D,int*);
        for (i = 0; i < D; ++i) alloc(M[i],D,int);
        twolvl::extractM(atoms[it],M,D,verbose);
        
        alloc(Minv,D,int*);
        for (i = 0; i < D; ++i) alloc(Minv[i],D,int);
        linalg::invertM(M,Minv,D,verbose);
        
        printf("Constructing H-embedding of facets of the base... ");
        int ** facets_base;
        alloc(facets_base,num_rows_S,int*);
        int num_facets_base = 0; // number of element currently in facets_base
        base::construct_facets_base(facets_base,num_facets_base,atoms[it],num_rows_S,D,verbose);
        printf("OK\n");
        
        printf("Constructing automorphism group of the base and extending it to R^D... ");
        int ** d_aut_collection;
        int num_autom_base;
        base::construct_d_aut_collection(d_aut_collection,num_autom_base,atoms[it],num_rows_S,num_cols_S,D,verbose);
        printf("OK\n");
        
        // Create the set Vert(P_0) (in V-embedding)
        printf("Building V-embedding of base... ");
        int ** base_V;
        alloc(base_V,num_cols_S,int *);
        base::construct_base_V(base_V,atoms[it],num_cols_S,D,verbose);
        printf("OK\n");
        
        // Create Vert(P_0) (H-embedding this time), the set of fixed points
        printf("Building H-embedding of base... ");
        int ** base_H;
        alloc(base_H,num_cols_S,int *);
        base::construct_base_H(base_H,base_V,Minv,num_cols_S,D,verbose);
        printf("OK\n");
        
        // Create the V-embedding of the reduced ground set (by means of translations)
        printf("Building V-embedding of the ground set... ");
        int ** ground_V;
        int size_ground_V = (nt::my_pow(3,D-1)+1)/2;
        alloc(ground_V,size_ground_V,int *);
        base::construct_ground_V(ground_V,D,verbose);
        printf("OK\n");

        // Create ground set
        printf("Building H-embedding of the reduced ground set... ");
        int ** ground_H;
        alloc(ground_H,size_ground_V,int *);
        int size_ground_H;
        base::construct_ground_H(ground_H,size_ground_H,ground_V,size_ground_V,facets_base,num_facets_base,Minv,D,verbose);
        printf("OK\n");
        
        // It is possible to free the base_V and ground_V, we will use the H-embedding
        for (i = 0; i < size_ground_V; ++i) free(ground_V[i]);
        free(ground_V);
        
        for (i = 0; i < num_cols_S; ++i) free(base_V[i]);
        free(base_V);
        
        printf("-> Size of the ground set = %d\n",size_ground_V);
        printf("-> Size of the reduced ground set = %d\n",size_ground_H);
        printf("-> Size of the automorphism group of the base = %d\n",num_autom_base);

        printf("Generating orbits of point of the ground set... ");
        int *** orbits;
        alloc(orbits,size_ground_H,int**);
        base::construct_orbits(orbits,num_autom_base,base_H,d_aut_collection,ground_H,size_ground_H,D);
        printf("OK\n");
        
        // Compute the slabs: inequalities x(E) <= 1, x(E) >= 0 that are valid for the base_H
        printf("Building slabs... ");
        int ** slabs;
        alloc(slabs,1 << D,int *);
        int num_slabs;
        base::construct_orbits(slabs,num_slabs,num_cols_S,base_H,D,verbose);
        printf("OK\n");
        
        // Check points versus slabs incidence (for each point, list the slabs containing it)
        printf("Building incidences between points and slabs... ");
        int ** slab_points_sat;
        alloc(slab_points_sat,size_ground_H,int*);
        base::construct_slab_point_sat(slab_points_sat,ground_H,slabs,size_ground_H,num_slabs,D,verbose);
        printf("OK\n");
        
        // Construct the incompatibility matrix
        printf("Constructing the incompatibility matrix... ");
        int ** incompatibility_adjM;
        alloc(incompatibility_adjM,size_ground_H,int*);
        base::construct_incompatibility_adjM(incompatibility_adjM,ground_H,facets_base,size_ground_H,num_facets_base,D);
        printf("OK\n");
        
        printf("Lauching Ganter's next-closure algorithm and checking 2-levelness... ");
        if (verbose != 0) printf("\n");
        
        int N_closed_sets_current_base = 0;
        int N_2level_current_base = 0;
        
        if (verbose != 0) printf("%*c  | 2-level | next_cl      | slack-matrix | 2-lev_time\n", size_ground_H, ' ');
        
        int * A;
        alloc(A,size_ground_H,int);
        std::memset(A,0,size_ground_H * sizeof(int));

        int * B;
        alloc(B,num_slabs,int);

        int * dchcl, * inccl, * I, * CI;
        alloc(dchcl,size_ground_H,int);
        alloc(inccl,size_ground_H,int);
        alloc(I,size_ground_H,int);
        alloc(CI,size_ground_H,int);
        
        while (!array::is_all_ones(A,size_ground_H)) {
            i = 0;
            if (verbose != 0) begin_nextcl = my_clock::now();
            do {
                while(A[i] == 1) ++i;
                clops::inc(A,i,I,size_ground_H); // I = inc(A,i)
                clops::discreteconvexhull_cl(I,B,dchcl,slab_points_sat,size_ground_H,num_slabs);
                clops::incompatibility_cl(dchcl,inccl,incompatibility_adjM,size_ground_H);
                clops::lexmax_symmetric_cl(inccl,CI,ground_H,size_ground_H,orbits,num_autom_base,D);
                ++i;
            } while (!clops::is_sqsubseteq(I,CI,size_ground_H));
            if (verbose != 0) {
                end_nextcl = my_clock::now();
                time_nextcl = end_nextcl-begin_nextcl;
            }
            std::memcpy(A,CI,size_ground_H * sizeof(int));
            N_closed_sets_current_base++;
            
            if (verbose != 0) {
                for (j = 0; j < size_ground_H; ++j) printf("%d",A[j]);
                printf(" ");
            }
            else {
                if ((N_closed_sets_current_base % 10000 == 0) && (N_closed_sets_current_base >= 10000))
                    printf("\n%dk",N_closed_sets_current_base/1000);
            }
            
            int length_A = 0;
            for (i = 0; i < size_ground_H; ++i) if (A[i] == 1) length_A++;
            
            int length_B = 0;
            for (i = 0; i < num_slabs; ++i) if (B[i] == 1) length_B++;
            
            // construct the slack matrix S with embedding transformation matrix in top left position
            int ** S_new;
            int num_rows_S_new, num_cols_S_new;
            alloc(S_new,2*length_B,int*);
            
            if (verbose != 0) begin_slack_matrix = my_clock::now();    
            twolvl::construct_slack_matrix(base_H,ground_H,A,B,slabs,atoms[it],S_new,size_ground_H,num_slabs,length_A,length_B,num_rows_S,num_cols_S,num_rows_S_new,num_cols_S_new,D);
            if (verbose != 0) {
                end_slack_matrix = my_clock::now();
                time_slack_matrix = end_slack_matrix - begin_slack_matrix;
            }
            
            if (verbose != 0) begin_skip_test = my_clock::now();
            bool accept = addnew::pass_test(S_new,num_rows_S_new,num_cols_S_new,num_cols_S);
            if (verbose != 0) {
                end_skip_test = my_clock::now();
                time_skip_test = end_skip_test - begin_skip_test;
            }

            bool istwolevel = false;
            if (accept) {
                if (verbose != 0) begin_2L_test = my_clock::now();
                istwolevel = twolvl::istwolevelpolytope(S_new,num_rows_S_new,num_cols_S_new,atoms_hash,atoms_cg,n_atoms,D);
                if (verbose != 0) {
                    end_2L_test = my_clock::now();
                    time_2L_test = end_2L_test - begin_2L_test;
                }
            }
            
            if (verbose != 0) {
                tot_nextcl += time_nextcl.count();
                tot_slack_matrix += time_slack_matrix.count();
                
                printf(" | ");
                if (accept) printf("%d",istwolevel);
                else printf("-");
                
                printf("       | %lf   s | ",time_nextcl.count());
                printf("%lf   s | ",time_slack_matrix.count());
                
                if (accept) {
                    tot_2L_test += time_2L_test.count();
                    printf("%lf   s ",time_2L_test.count());
                }
                else tot_2L_test += time_skip_test.count();
                printf("\n");
            }
            
            if (istwolevel) {
                addnew::to_list(S_new,num_rows_S_new,num_cols_S_new,my_outputfile,LD_hash,LD,current_LD,D,verbose,simplicial_facet,cs,stab,n_suspensions,n_polar);
                N_2level_current_base++;
                total_2level++;
            }
            
            for (i = 0; i < 2*length_B; ++i) free(S_new[i]);
            free(S_new);
        }
        
        free(CI);
        free(I);
        free(inccl);
        free(dchcl);
        free(B);
        free(A);
        
        for (i = 0; i < size_ground_H; ++i) free(incompatibility_adjM[i]);
        free(incompatibility_adjM);
        
        for (i = 0; i < size_ground_H; ++i) free(slab_points_sat[i]);
        free(slab_points_sat);
        
        for (i = 0; i < num_slabs; ++i) free(slabs[i]);
        free(slabs);
        
        for (i = 0; i < size_ground_H; ++i) {
            for (j = 0; j < num_autom_base; ++j) free(orbits[i][j]);
            free(orbits[i]);
        }
        free(orbits);
        
        for (i = 0; i < size_ground_V; ++i) free(ground_H[i]);
        free(ground_H);
        
        for (i = 0; i < num_cols_S; ++i) free(base_H[i]);
        free(base_H);
        
        for (i = 0; i < num_autom_base; ++i) free(d_aut_collection[i]);
        free(d_aut_collection);
        
        for (i = 0; i < num_rows_S; ++i) free(facets_base[i]);
        free(facets_base);
        
        for (i = 0; i < D; ++i) {
            free(Minv[i]);
            free(M[i]);
        }
        free(Minv);
        free(M);
        
        if (verbose != 0) {
            printf("%*c | ", size_ground_H+11, ' ');
            printf("%lf   s | ",tot_nextcl);
            printf("%lf   s | ",tot_slack_matrix);
            printf("%lf   s \n",tot_2L_test);
            
            tot_nextcl = 0;
            tot_slack_matrix = 0;
            tot_2L_test = 0;
        }
        else printf("\n");
        printf("OK\n");
        
        end_per_base = my_clock::now();
        time_per_base = end_per_base-begin_per_base;
        printf("#closed sets found for base #%d = %d\n", n_base, N_closed_sets_current_base);
        printf("#2L polytopes for base #%d = %d\n", n_base, N_2level_current_base);
        printf("Elapsed time for base #%d = %lfs\n", n_base, time_per_base.count());
        
        //tot_elapsed_time_sum += time_per_base.count();
        N_closed_sets += N_closed_sets_current_base;
    }
    
    end_tot_time = my_clock::now();
    tot_elapsed_time = end_tot_time - begin_tot_time;

    printf("\n" ANSI_GREEN "Total #closed sets found = %d" ANSI_RESET "\n", N_closed_sets);
    printf(ANSI_GREEN "#2-level polytopes tested = %d" ANSI_RESET "\n", total_2level);
    printf(ANSI_GREEN "#non-isomorphic 2-level polytopes found = %d" ANSI_RESET "\n", current_LD);
    printf(ANSI_GREEN "Total elapsed time = %lfs" ANSI_RESET "\n\n", tot_elapsed_time.count());
    //printf("Total elapsed time(sum)= %lfs\n", tot_elapsed_time_sum);

    if (verbose != 0) {
        printf("\nD       | %d\n",D);
        printf("Delta-f | %d\n",simplicial_facet);
        printf("CS      | %d\n",cs);
        printf("STAB    | %d\n",stab);
        printf("polar   | %d\n",n_polar);
        printf("susp    | %d\n",n_suspensions);
        printf("2L      | %d\n\n",current_LD);
    }

    fclose(my_outputfile);
    
    for (i = 0; i < n_atoms; ++i) {
        twolvl::hash2size(atoms_hash[i],num_rows,num_cols,D);
        for (j = 0; j < num_rows; ++j) free(atoms[i][j]);
        free(atoms[i]);
        free(atoms_cg[i]);
    }
    free(atoms_cg);
    free(atoms);
    free(atoms_hash);
    
    for (i = 0; i < current_LD; ++i) free(LD[i]);
    free(LD);
    free(LD_hash);
    
    return 0;
}