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

#include "alloc.hpp"
#include "alloc_triangular_matrix.hpp"
#include "alloc_matrix.hpp"

#include "linalg/is_id.hpp"
#include "linalg/invert.hpp"

#include "linalg/div_ceil.hpp"
#include "linalg/transpose.hpp"

#include "nt/my_pow.hpp"
#include "nt/factor.hpp"

#include "tl/load.hpp"
#include "tl/dump.hpp"
#include "tl/extractM.hpp"
#include "tl/checksimplicialcore.hpp"
#include "tl/construct_slack_matrix.hpp"

#include "tl/Polytope.hpp"
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

#include "array/pack.hpp"
#include "array/dump.hpp"

#include "clops/inc.hpp"
#include "clops/is_sqsubseteq.hpp"
//#include "clops/discreteconvexhull_cl.hpp"
#include "clops/fast_discreteconvexhull_cl.hpp"
#include "clops/incompatibility_cl.hpp"
#include "clops/lexmax_symmetric_cl.hpp"

#include "simpl/slack_matrix_simplicial_2L.hpp"
#include "simpl/push_simplicial_core.hpp"

int main () {

    std::ios_base::sync_with_stdio(true);

    int tot_N_closed_sets = 0;

    // start the popcorn machine
    std::vector<tl::Polytope<int>> facets;
    while ( true ) {

        if ( !tl::load(std::cin, facets) ) break ;

        auto& facet = facets[0]; // ugly we use a length-1 vector atm

        int D = facet.dimension + 1;

        // std::cerr << "Inflating facet..." << std::endl;

        // use the characterization of simplicial 2-level polytopes
        if (linalg::is_id(facet.dimension+1, facet.matrix)) {
            // std::cerr << "Simplicial facet:" << std::endl;
            // std::cerr << "Constructing slack-matrices of simplicial 2-level polytopes... ";
            int K;
            void * mem_S_new;
            int ** S_new;
            int num_rows_S_new,num_cols_S_new;

            for (auto N : nt::factor(D)) {
                K = D/N;
                simpl::slack_matrix_simplicial_2L(K,N,mem_S_new,S_new,num_rows_S_new,num_cols_S_new);
                simpl::push_simplicial_core(S_new,num_rows_S_new,num_cols_S_new,facet.matrix,D);

                tl::dump(std::cout, D, num_rows_S_new, num_cols_S_new, S_new);

                free(mem_S_new);
            }
        }

        // non simplicial case
        else {
            int num_rows_S(facet.rows);
            int num_cols_S(facet.columns);

            // fprintf(stderr, "Simplicial core? ");fprintf(stderr, "OK\n");else
            if (!tl::checksimplicialcore(facet.matrix,D)) {
                fprintf(stderr, "Fail. Simplicial core not found.\n");
                return 1;
            }

            // Extract embedding transformation matrix M_d(0) and invert it
            void * mem_M, * mem_Minv;
            int ** M, ** Minv;
            alloc_matrix(mem_M,M,D,D);
            alloc_matrix(mem_Minv,Minv,D,D);

            tl::extractM(facet.matrix,M,D);
            linalg::invert(M,Minv,D);

            free(mem_M);

            // fprintf(stderr, "Constructing H-embedding of facets of the base... ");
            void * mem_facets_base;
            int ** facets_base;
            alloc_matrix(mem_facets_base,facets_base,num_rows_S,D);
            int num_facets_base = 0; // number of element currently in facets_base
            base::construct_facets_base(facets_base,num_facets_base,facet.matrix,num_rows_S,D);
            // fprintf(stderr, "OK\n");

            // fprintf(stderr, "Constructing automorphism group of the base and extending it to R^D... ");
            void * mem_d_aut_collection;
            int ** d_aut_collection;
            const int num_autom_base = base::construct_d_aut_collection(mem_d_aut_collection,d_aut_collection,facet.matrix,num_rows_S,num_cols_S,D);
            // fprintf(stderr, "OK\n");

            // Create the set Vert(P_0) (in V-embedding)
            // fprintf(stderr, "Building V-embedding of base... ");
            void * mem_base_V;
            int ** base_V;
            alloc_matrix(mem_base_V,base_V,num_cols_S,D);
            base::construct_base_V(base_V,facet.matrix,num_cols_S,D);
            // fprintf(stderr, "OK\n");

            // Create Vert(P_0) (H-embedding this time), the set of fixed points
            // fprintf(stderr, "Building H-embedding of base... ");
            void * mem_base_H;
            int ** base_H;
            alloc_matrix(mem_base_H,base_H,num_cols_S,D);
            base::construct_base_H(base_H,base_V,Minv,num_cols_S,D);
            // fprintf(stderr, "OK\n");

            // Create the V-embedding of the reduced ground set (by means of translations)
            // fprintf(stderr, "Building V-embedding of the ground set... ");
            void * mem_ground_V;
            int ** ground_V;
            int size_ground_V = (nt::my_pow(3,D-1)+1)/2;
            alloc_matrix(mem_ground_V,ground_V,size_ground_V,D);
            base::construct_ground_V(ground_V,D);
            // fprintf(stderr, "OK\n");

            // Create ground set
            // fprintf(stderr, "Building H-embedding of the reduced ground set... ");
            void * mem_ground_H;
            int ** ground_H;
            alloc_matrix(mem_ground_H,ground_H,size_ground_V,D);
            int size_ground_H;
            base::construct_ground_H(ground_H,size_ground_H,ground_V,size_ground_V,facets_base,num_facets_base,Minv,D);
            // fprintf(stderr, "OK\n");

            // It is possible to free the base_V and ground_V, we will use the H-embedding
            free(mem_ground_V);
            free(mem_base_V);

            fprintf(stderr, "-> Size of the ground set = %d\n",size_ground_V);
            fprintf(stderr, "-> Size of the reduced ground set = %d\n",size_ground_H);
            fprintf(stderr, "-> Size of the automorphism group of the base = %d\n",num_autom_base);

            // fprintf(stderr, "Generating orbits of point of the ground set... ");
            void * mem_orbits;
            int ** orbits;
            alloc_matrix(mem_orbits,orbits,num_autom_base,size_ground_H);
            base::construct_orbits(orbits,num_autom_base,base_H,d_aut_collection,ground_H,size_ground_H,D);
            // fprintf(stderr, "OK\n");

            // Compute the slabs: inequalities x(E) <= 1, x(E) >= 0 that are valid for the base_H
            // fprintf(stderr, "Building slabs... ");
            void * mem_slabs;
            int ** slabs;
            alloc_matrix(mem_slabs,slabs,1 << D,D);
            int num_slabs;
            base::construct_slabs(slabs,num_slabs,num_cols_S,base_H,D);
            // fprintf(stderr, "OK\n");

            // Check points versus slabs incidence (for each point, list the slabs containing it)
            // fprintf(stderr, "Building incidences between points and slabs... ");
            void * mem_slab_points_sat;
            int ** slab_points_sat;
            alloc_matrix(mem_slab_points_sat,slab_points_sat,size_ground_H,num_slabs);
            base::construct_slab_point_sat(slab_points_sat,ground_H,slabs,size_ground_H,num_slabs,D);
            // fprintf(stderr, "OK\n");

            void * mem_slab_points_sat_t;
            int ** slab_points_sat_t;
            alloc_matrix(mem_slab_points_sat_t,slab_points_sat_t,num_slabs,size_ground_H);
            linalg::transpose(slab_points_sat,slab_points_sat_t,size_ground_H,num_slabs);

            const int n_cols_64 = linalg::div_ceil(num_slabs, 64);
            const int n_rows_64 = linalg::div_ceil(size_ground_H, 64);

            // construct the block uint64_t
            void * mem_sp_64;
            uint64_t ** sp_64;
            alloc_matrix(mem_sp_64,sp_64,size_ground_H,n_cols_64);
            array::pack64_matrix(slab_points_sat,sp_64,size_ground_H,num_slabs,n_cols_64);

            void * mem_sp_t_64;
            uint64_t ** sp_t_64;
            alloc_matrix(mem_sp_t_64,sp_t_64,num_slabs,n_rows_64);
            array::pack64_matrix(slab_points_sat_t,sp_t_64,num_slabs,size_ground_H,n_rows_64);

            // std::cerr << "slab_points_sat = " << std::endl;
            // array::dump_matrix(slab_points_sat,size_ground_H,num_slabs);
            // std::cerr << "\nslab_points_sat_t = " << std::endl;
            // array::dump_matrix(slab_points_sat_t,num_slabs,size_ground_H);
            // std::cerr << "\nsp_64 = " << std::endl;
            // array::dump_matrix_64(sp_64,size_ground_H,n_cols_64);
            // std::cerr << "\nsp_t_64 = " << std::endl;
            // array::dump_matrix_64(sp_t_64,num_slabs,n_rows_64);


            // Construct the incompatibility matrix
            // fprintf(stderr, "Constructing the incompatibility matrix... ");
            void * mem_incompatibility_adjM;
            int ** incompatibility_adjM;
            alloc_triangular_matrix(mem_incompatibility_adjM,incompatibility_adjM,size_ground_H);
            base::construct_incompatibility_adjM(incompatibility_adjM,ground_H,facets_base,size_ground_H,num_facets_base,D);
            // fprintf(stderr, "OK\n");

            // fprintf(stderr, "Lauching Ganter's next-closure algorithm and checking 2-levelness... ");

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

            uint64_t * B_64;
            alloc(B_64,n_cols_64,uint64_t);
            uint64_t * dchcl_64;
            alloc(dchcl_64,n_rows_64,uint64_t);

            while (!array::is_all_ones(A,size_ground_H)) {
                int i = 0;
                do {
                    // std::cerr << "I am in" << std::endl;
                    while(A[i] == 1) ++i;
                    // for (int s = 0; s < size_ground_H; ++s) std::cerr << A[s];
                    // std::cerr << std::endl;
                    clops::inc(A,i,I,size_ground_H); // I = inc(A,i)
                    // for (int s = 0; s < size_ground_H; ++s) std::cerr << I[s];
                    // std::cerr << std::endl;
                    // std::cerr << "0" << std::endl;
                    //clops::discreteconvexhull_cl(I,B,dchcl,slab_points_sat,size_ground_H,num_slabs);
                    clops::fast_discreteconvexhull_cl(I,B_64,dchcl_64,sp_64,sp_t_64,size_ground_H,num_slabs,n_rows_64,n_cols_64);
                    array::unpack64(dchcl,size_ground_H,dchcl_64);
                    // std::cerr << ANSI_YELLOW << "C" << ANSI_RESET << std::endl;
                    clops::incompatibility_cl(dchcl,inccl,incompatibility_adjM,size_ground_H);
                    // std::cerr << ANSI_MAGENTA << "D" << ANSI_RESET << std::endl;
                    clops::lexmax_symmetric_cl(inccl,CI,size_ground_H,orbits,num_autom_base);
                    // std::cerr << ANSI_BLUE << "E" << ANSI_RESET << std::endl;
                    ++i;
                    // std::cerr << "I am testing" << std::endl;
                } while (!clops::is_sqsubseteq(I,CI,size_ground_H));
                // std::cerr << "I am out" << std::endl;
                array::unpack64(B,num_slabs,B_64);
                // std::cerr << "unpack" << std::endl;
                std::memcpy(A,CI,size_ground_H * sizeof(int));
                ++tot_N_closed_sets;

                // construct the slack matrix S with embedding transformation matrix in top left position
                void * mem_S_new;
                int ** S_new;
                int num_rows_S_new, num_cols_S_new;

                bool base_is_lex_max = tl::construct_slack_matrix(base_H,ground_H,A,B,slabs,facet.matrix,mem_S_new,S_new,size_ground_H,num_slabs,num_cols_S,num_rows_S_new,num_cols_S_new,D);

                if ( base_is_lex_max ) {
                    tl::dump(std::cout, D, num_rows_S_new, num_cols_S_new,S_new);
                    free(mem_S_new);
                }

            }
            fprintf(stderr, "OK\n");
            free(CI);
            free(I);
            free(inccl);
            free(dchcl);
            free(B);
            free(dchcl_64);
            free(B_64);
            free(A);

            free(mem_incompatibility_adjM);
            free(mem_slab_points_sat_t);
            free(mem_sp_64);
            free(mem_sp_t_64);
            free(mem_slab_points_sat);
            free(mem_slabs);
            free(mem_orbits);
            free(mem_ground_H);
            free(mem_base_H);
            free(mem_d_aut_collection);
            free(mem_facets_base);
            free(mem_Minv);
        }

        facet.teardown();
        facets.clear(); // ugly hack atm, we use a vector containing a single element

    }
    std::cerr << tot_N_closed_sets << std::endl;
    return 0;

}
