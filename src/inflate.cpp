#include <iostream>
#include <vector>

#define ANSI_RED     "\e[91m"
#define ANSI_YELLOW  "\e[93m"
#define ANSI_MAGENTA "\e[95m"
#define ANSI_CYAN    "\e[96m"
#define ANSI_GREEN   "\e[92m"
#define ANSI_BLUE    "\e[94m"
#define ANSI_RESET   "\e[0m"

#define TITLE_TEXT   "\e[92m\e[4m"
#define FINAL_TEXT   "\e[93m"
#define FINAL_DATA   "\e[91m"

#include "linalg/is_id.hpp"
#include "linalg/invertM.hpp"

#include "nt/my_pow.hpp"
#include "nt/factor.hpp"

#include "twolvl/load.hpp"
#include "twolvl/dump.hpp"
#include "twolvl/extractM.hpp"
#include "twolvl/checksimplicialcore.hpp"
#include "twolvl/construct_slack_matrix.hpp"

#include "base/Atom.hpp"
#include "base/construct_d_aut_collection.hpp"
#include "base/construct_slab_point_sat.hpp"
#include "base/construct_incompatibility_adjM.hpp"
#include "base/construct_orbits.hpp"
#include "base/construct_slabs.hpp"
#include "base/construct_facets_base.hpp"
#include "base/construct_base_V.hpp"
#include "base/construct_base_H.hpp"
#include "base/construct_ground_V.hpp"
#include "base/construct_ground_H.hpp"

#include "array/is_all_ones.hpp"

#include "clops/inc.hpp"
#include "clops/is_sqsubseteq.hpp"
#include "clops/discreteconvexhull_cl.hpp"
#include "clops/incompatibility_cl.hpp"
#include "clops/lexmax_symmetric_cl.hpp"

#include "simpl/slack_matrix_simplicial_2L.hpp"
#include "simpl/push_simplicial_core.hpp"

#include "addnew/pass_test.hpp"

int main (int argc, const char* argv[]) {

    std::ios_base::sync_with_stdio(true);

    if (argc < 1 || argc > 2) {
        std::cerr << "usage: inflate [<verbosity>]" << std::endl;
        return 1;
    }

    int verbose = (argc == 2) ? atoi(argv[1]) : 0;

    // start the popcorn machine
    std::vector<base::Atom<int>> facets;
    while ( true ) {

        if ( !twolvl::load(std::cin, facets) ) break ;

        auto& facet = facets[0]; // ugly we use a length-1 vector atm

        int D = facet.dimension + 1;

        std::cerr << "Inflating facet..." << std::endl;

        // use the characterization of simplicial 2-level polytopes
        if (linalg::is_id(facet.dimension+1, facet.matrix)) {
            std::cerr << "Simplicial facet:" << std::endl;
            std::cerr << "Constructing slack-matrices of simplicial 2-level polytopes... ";
            int K;
            int ** S_new;
            int num_rows_S_new,num_cols_S_new;

            for (auto N : nt::factor(D)) {
                K = D/N;
                simpl::slack_matrix_simplicial_2L(K,N,S_new,num_rows_S_new,num_cols_S_new);
                simpl::push_simplicial_core(S_new,num_rows_S_new,num_cols_S_new,facet.matrix,D);

                twolvl::dump(std::cout, D, num_rows_S_new, num_cols_S_new, S_new);

                for (int i = 0; i < num_rows_S_new; ++i) free(S_new[i]);
                free(S_new);
            }

        }

        // non simplicial case
        else {
            int num_rows_S(facet.rows);
            int num_cols_S(facet.columns);

            fprintf(stderr, "Simplicial core? ");
            if (twolvl::checksimplicialcore(facet.matrix,D)) fprintf(stderr, "OK\n");
            else {
                fprintf(stderr, "Fail\n");
                return 1;
            }

            // Extract embedding transformation matrix M_d(0) and invert it
            int ** M, ** Minv;
            alloc(M,D,int*);
            for (int i = 0; i < D; ++i) alloc(M[i],D,int);
            twolvl::extractM(facet.matrix,M,D,verbose);

            alloc(Minv,D,int*);
            for (int i = 0; i < D; ++i) alloc(Minv[i],D,int);
            linalg::invertM(M,Minv,D,verbose);

            fprintf(stderr, "Constructing H-embedding of facets of the base... ");
            int ** facets_base;
            alloc(facets_base,num_rows_S,int*);
            int num_facets_base = 0; // number of element currently in facets_base
            base::construct_facets_base(facets_base,num_facets_base,facet.matrix,num_rows_S,D,verbose);
            fprintf(stderr, "OK\n");

            fprintf(stderr, "Constructing automorphism group of the base and extending it to R^D... ");
            int ** d_aut_collection;
            int num_autom_base;
            base::construct_d_aut_collection(d_aut_collection,num_autom_base,facet.matrix,num_rows_S,num_cols_S,D);
            fprintf(stderr, "OK\n");

            // Create the set Vert(P_0) (in V-embedding)
            fprintf(stderr, "Building V-embedding of base... ");
            int ** base_V;
            alloc(base_V,num_cols_S,int *);
            base::construct_base_V(base_V,facet.matrix,num_cols_S,D,verbose);
            fprintf(stderr, "OK\n");

            // Create Vert(P_0) (H-embedding this time), the set of fixed points
            fprintf(stderr, "Building H-embedding of base... ");
            int ** base_H;
            alloc(base_H,num_cols_S,int *);
            base::construct_base_H(base_H,base_V,Minv,num_cols_S,D,verbose);
            fprintf(stderr, "OK\n");

            // Create the V-embedding of the reduced ground set (by means of translations)
            fprintf(stderr, "Building V-embedding of the ground set... ");
            int ** ground_V;
            int size_ground_V = (nt::my_pow(3,D-1)+1)/2;
            alloc(ground_V,size_ground_V,int *);
            base::construct_ground_V(ground_V,D,verbose);
            fprintf(stderr, "OK\n");

            // Create ground set
            fprintf(stderr, "Building H-embedding of the reduced ground set... ");
            int ** ground_H;
            alloc(ground_H,size_ground_V,int *);
            int size_ground_H;
            base::construct_ground_H(ground_H,size_ground_H,ground_V,size_ground_V,facets_base,num_facets_base,Minv,D,verbose);
            fprintf(stderr, "OK\n");

            // It is possible to free the base_V and ground_V, we will use the H-embedding
            for (int i = 0; i < size_ground_V; ++i) free(ground_V[i]);
            free(ground_V);

            for (int i = 0; i < num_cols_S; ++i) free(base_V[i]);
            free(base_V);

            fprintf(stderr, "-> Size of the ground set = %d\n",size_ground_V);
            fprintf(stderr, "-> Size of the reduced ground set = %d\n",size_ground_H);
            fprintf(stderr, "-> Size of the automorphism group of the base = %d\n",num_autom_base);

            fprintf(stderr, "Generating orbits of point of the ground set... ");
            int ** orbits;
            alloc(orbits,num_autom_base,int*);
            base::construct_orbits(orbits,num_autom_base,base_H,d_aut_collection,ground_H,size_ground_H,D);
            fprintf(stderr, "OK\n");

            // Compute the slabs: inequalities x(E) <= 1, x(E) >= 0 that are valid for the base_H
            fprintf(stderr, "Building slabs... ");
            int ** slabs;
            alloc(slabs,1 << D,int *);
            int num_slabs;
            base::construct_slabs(slabs,num_slabs,num_cols_S,base_H,D,verbose);
            fprintf(stderr, "OK\n");

            // Check points versus slabs incidence (for each point, list the slabs containing it)
            fprintf(stderr, "Building incidences between points and slabs... ");
            int ** slab_points_sat;
            alloc(slab_points_sat,size_ground_H,int*);
            base::construct_slab_point_sat(slab_points_sat,ground_H,slabs,size_ground_H,num_slabs,D,verbose);
            fprintf(stderr, "OK\n");

            // Construct the incompatibility matrix
            fprintf(stderr, "Constructing the incompatibility matrix... ");
            int ** incompatibility_adjM;
            alloc(incompatibility_adjM,size_ground_H,int*);
            base::construct_incompatibility_adjM(incompatibility_adjM,ground_H,facets_base,size_ground_H,num_facets_base,D);
            fprintf(stderr, "OK\n");

            fprintf(stderr, "Lauching Ganter's next-closure algorithm and checking 2-levelness... ");
            if (verbose != 0) fprintf(stderr, "\n");

            int N_closed_sets_current_base = 0;

            if (verbose != 0) fprintf(stderr, "%*c  | 2-level | next_cl      | slack-matrix | 2-lev_time\n", size_ground_H, ' ');

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
                int i = 0;
                do {
                    while(A[i] == 1) ++i;
                    clops::inc(A,i,I,size_ground_H); // I = inc(A,i)
                    clops::discreteconvexhull_cl(I,B,dchcl,slab_points_sat,size_ground_H,num_slabs);
                    clops::incompatibility_cl(dchcl,inccl,incompatibility_adjM,size_ground_H);
                    clops::lexmax_symmetric_cl(inccl,CI,size_ground_H,orbits,num_autom_base);
                    ++i;
                } while (!clops::is_sqsubseteq(I,CI,size_ground_H));
                std::memcpy(A,CI,size_ground_H * sizeof(int));
                N_closed_sets_current_base++;

                if (verbose != 0) {
                    for (int j = 0; j < size_ground_H; ++j) fprintf(stderr, "%d",A[j]);
                    fprintf(stderr, " ");
                }
                else {
                    if ((N_closed_sets_current_base % 10000 == 0) && (N_closed_sets_current_base >= 10000))
                        fprintf(stderr, "\n%dk",N_closed_sets_current_base/1000);
                }

                int length_A = 0;
                for (i = 0; i < size_ground_H; ++i) if (A[i] == 1) length_A++;

                int length_B = 0;
                for (i = 0; i < num_slabs; ++i) if (B[i] == 1) length_B++;

                // construct the slack matrix S with embedding transformation matrix in top left position
                int ** S_new;
                int num_rows_S_new, num_cols_S_new;
                alloc(S_new,2*length_B,int*);

                twolvl::construct_slack_matrix(base_H,ground_H,A,B,slabs,facet.matrix,S_new,size_ground_H,num_slabs,length_A,length_B,num_cols_S,num_rows_S_new,num_cols_S_new,D);

                if ( addnew::pass_test(S_new,num_rows_S_new,num_cols_S_new,num_cols_S) ) {
                    twolvl::dump(std::cout, D, num_rows_S_new, num_cols_S_new, S_new);
                }

                for (int i = 0; i < 2*length_B; ++i) free(S_new[i]);
                free(S_new);

            }
            fprintf(stderr, "\nOK\n");

            free(CI);
            free(I);
            free(inccl);
            free(dchcl);
            free(B);
            free(A);

            for (int i = 0; i < size_ground_H; ++i) free(incompatibility_adjM[i]);
            free(incompatibility_adjM);

            for (int i = 0; i < size_ground_H; ++i) free(slab_points_sat[i]);
            free(slab_points_sat);

            for (int i = 0; i < num_slabs; ++i) free(slabs[i]);
            free(slabs);

            for (int i = 0; i < num_autom_base; ++i) free(orbits[i]);
            free(orbits);

            for (int i = 0; i < size_ground_V; ++i) free(ground_H[i]);
            free(ground_H);

            for (int i = 0; i < num_cols_S; ++i) free(base_H[i]);
            free(base_H);

            for (int i = 0; i < num_autom_base; ++i) free(d_aut_collection[i]);
            free(d_aut_collection);

            for (int i = 0; i < num_rows_S; ++i) free(facets_base[i]);
            free(facets_base);

            for (int i = 0; i < D; ++i) {
                free(Minv[i]);
                free(M[i]);
            }
            free(Minv);
            free(M);

        }

        facet.teardown();
        facets.clear(); // ugly hack atm, we use a vector containing a single element

    }

    return 0;

}
