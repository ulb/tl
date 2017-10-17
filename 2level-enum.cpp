#include <nauty.h>
#include <naugroup.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <chrono>

#include <cstring> // std::equal
#include <algorithm> // std::find

//#include "my_lin_algebra_lib.h"
//#include "closure_operators.h"
//#include "closure_operators.h"

//#define D 3
//#define verbose 0

#define alloc(p,a,t) do \
if (!(p = (t *) malloc ((size_t) ((a) * sizeof (t))))) { \
printf("run out of memory [alloc(%s,%d,%s)]\n",#p,a,#t); \
exit(1); \
} while(0);

// Counters for the classes of 2L polytopes
unsigned int simplicial_facet = 0; // 2L polytopes with a simplicial facet
unsigned int cs = 0; // Centrally-symmetric 2L polytopes
unsigned int stab = 0; // Stable set polytopes of a perfect graph
unsigned int n_suspensions = 0; // 2L suspensions
unsigned int n_polar = 0; // Polar 2L polytopes

// check equality of T * arrays
template <class T>
bool is_equal(T * array1,T * array2, const int length) {
    for (int i = 0; i < length; ++i) if (array1[i] != array2[i]) return false;
    return true;
}

template <class T>
int index_in_collection(T ** collection,T * vect, const int length_collection, const int length_vect) {
    for (int i = 0; i < length_collection; ++i) if (is_equal(collection[i],vect,length_vect)) return i;
    return length_collection;
}

template <class T>
bool is_all_ones(T * pt, const int length) {
    T* end = pt + length;
    while (pt != end) {
        if (*pt == 0) return false;
        ++pt;
    }
    return true;
}

// assign int * array to another: array2 to array1
template <class T>
void assign_array(T * array1,T * array2, const int length) {
    std::memcpy(array1,array2,length * sizeof(T));
}

// check if val occurs in array
template <class T>
bool is_in_array(T * array, T val, const int length) {
    return std::find(array,array+length,val) != array+length;
}

// factor an integer
template <class T>
std::vector<T> factor(T n) {
    std::vector<T> factors;   
    for (T d = 1; d <= n; ++d) if (n % d == 0) factors.push_back(d);
    return factors;
}

// y = Mx
void my_matrix_prod(int ** M, int * x, int * y, const int num_rows, const int num_cols) {
    int i, j;
    for (i = 0; i < num_rows; ++i) {
        y[i] = 0;
        for (j = 0; j < num_cols; ++j) y[i] += M[i][j]*x[j];
    }
}

// <x,y> = out
void my_inner_prod(int * x, int * y, int & out, const int length) {
    out = 0;
    for (int i = 0; i < length; ++i) out += x[i]*y[i];
}

// Check if A \preccurlyeq B
bool preccurlyeq(int * A, int * B, const int length) {
    if (is_equal(A,B,length)) return true;
    int i = length - 1;
    while (i > 0) {
        if (((A[i] == 1) && (B[i] == 0)) || ((A[i] == 0) && (B[i] == 1))) return B[i] == 1;
        i--;
    }
    return B[0] == 1;
}

// Check if A is a subset of B
bool is_subseteq(int * A, int * B, const int length) {
    for (int i = 0; i < length; ++i) if (A[i] == 1 && B[i] == 0) return false;
    return true;
}

int min_A_idx(int * A, const int length) {
    for (int i = 0; i < length; ++i) if (A[i] == 1) return i;
    return length;
}

// Check if A \sqsubseteq B
bool is_sqsubseteq(int * A, int * B, const int length) {
    int min_A = min_A_idx(A,length);
    if (is_subseteq(A,B,length)) return is_equal(B+min_A,A+min_A,length-min_A);
    else return false;
}

// I = inc(A,i)
void inc(int* A, const int i, int* I, const int length) {
    std::memset(I,0,i * sizeof(int));
    I[i] = 1;
    std::memcpy(I+i+1, A+i+1,(length-i-1) * sizeof(int));
}

// Compute the discrete convex hull of a point set A
void discreteconvexhull_cl(int *& A,int *& B,int *& dchcl,int **& slab_points_sat, const int length_A, const int length_B) {
    int i, j;
    
    // Set all bits of B to 1
    std::fill(B,B+length_B,1);
    
    // Intersect all sets of slabs belonging to elements of A
    for (i = 0; i < length_A; ++i) {
        if (A[i] == 1) {
            for (j = 0; j < length_B; ++j)
                if (slab_points_sat[i][j] == 0)  B[j] = 0;
        }
    }    
    // Set all bits of dchcl to 1
    std::fill(dchcl,dchcl+length_A,1);
    
    // Intersect all sets of points belonging to elements of B
    for (i = 0; i < length_B; ++i) {
        if (B[i] == 1) {
            for (j = 0; j < length_A; ++j)
                if (slab_points_sat[j][i] == 0) dchcl[j] = 0;
        }
    }
}

// test for incompatibility
bool is_incompatible(int * A,int ** incompatibility_adjM,const int length_A) {
    for (int i = 0; i < length_A; ++i) {
        if (A[i] == 0) continue;
        for (int j = 0; j < i; ++j) if (A[j] == 1 && incompatibility_adjM[i][j] == 1) return false;
    }
    return true;
}

// incompatibility closure operator
void incompatibility_cl(int * A,int * inccl,int ** incompatibility_adjM,const int length_A) { 
    if (is_incompatible(A,incompatibility_adjM,length_A))
        std::memcpy(inccl,A,length_A * sizeof(int));
    else std::fill(inccl,inccl+length_A,1);
}

void lexmax_symmetric(int *& A,int *& symcl,int **& ground_set_H,const int length_A,int ***& orbits,const int num_autom_base,const int D) {
    int i,j;
    
    std::memcpy(symcl,A,length_A * sizeof(int));
    
    if (!(is_all_ones(A,length_A))) {
        bool is_outside_X;
        int * A_sym;
        alloc(A_sym,length_A,int);
        
        A_sym[0] = 1;
        std::memset(A_sym+1,0,(length_A-1) * sizeof(int));
        
        if (!is_equal(A,A_sym,length_A)) {
            for (i = 0; i < num_autom_base; ++i) {
                std::memset(A_sym+1,0,(length_A-1) * sizeof(int));
                is_outside_X = false;
                for (j = 1; j < length_A && !(is_outside_X); ++j) {
                    if (A[j] == 1) {
                        if (orbits[j][i][0] == 0) is_outside_X = true;
                        else A_sym[index_in_collection(ground_set_H,orbits[j][i],length_A,D)] = 1;
                    }
                }
                if (!(is_outside_X)) {
                    if (preccurlyeq(symcl,A_sym,length_A))
                        std::memcpy(symcl,A_sym,length_A * sizeof(int));
                }
            }
        }
        else std::memcpy(symcl,A_sym,length_A * sizeof(int));
        free(A_sym);
    }
}

void generate_id_matrix(const int k,int ** Id) {
    int i;
    for (i = 0; i < k; ++i) {
        std::memset(Id[i],0,i * sizeof(int));
        Id[i][i] = 1;
        std::memset(Id[i]+i+1,0,(k-i-1) * sizeof(int));
    }
}

void slack_matrix_free_sum(int ** P,int ** Q,const int m1,const int n1,const int m2,const int n2,int **& P_oplus_Q,int & num_rows,int & num_cols) {
    int i,j,k;
    int offset;
    num_rows = m1 * m2;
    num_cols = n1 + n2;
    
    alloc(P_oplus_Q,num_rows,int*);
    for (i = 0; i < num_rows; ++i) alloc(P_oplus_Q[i],num_cols,int);
    
    offset = 0;
    for (i = 0; i < m2; ++i) {
        for (j = 0; j < m1; ++j)
            for (k = 0; k < n1; ++k) P_oplus_Q[j + offset][k] = P[j][k];
        offset += m1;
    }
    
    offset = 0;
    for (j = 0; j < m2; ++j) {
        for (i = 0; i < m1; ++i) {
            for (k = 0; k < n2; ++k) P_oplus_Q[offset][n1 + k] = Q[j][k];
            offset += 1;
        }
    }
}

void slack_matrix_simplicial_2L(const int h, const int n, int **& M, int & num_r_M, int & num_c_M) {
    int i,j;
    int ** Id;
    alloc(Id,h+1,int*);
    for (i = 0; i < h+1; ++i) alloc(Id[i],h+1,int);
    
    generate_id_matrix(h+1,Id); // Id(k+1) is the slack matrix of the k-simplex Delta_k
    
    num_r_M = h+1;
    num_c_M = h+1;
    alloc(M,num_r_M,int*);
    for (i = 0; i < num_r_M; ++i) {
        alloc(M[i],num_c_M,int);
        std::memcpy(M[i],Id[i],num_c_M * sizeof(int));
    }
    
    int ** M_oplus;
    int num_r_M_oplus,num_c_M_oplus;
    
    for (i = 0; i < n-1; ++i) {
        slack_matrix_free_sum(M,Id,num_r_M,num_c_M,h+1,h+1,M_oplus,num_r_M_oplus,num_c_M_oplus);
        for (j = 0; j < num_r_M; ++j) free(M[j]);
        free(M);
        M = M_oplus;
        num_r_M = num_r_M_oplus;
        num_c_M = num_c_M_oplus;
    }
    
    for (i = 0; i < h+1; ++i) free(Id[i]);
    free(Id);
}

void push_simplicial_core(int **& M,const int num_rows_M,const int num_cols_M,int **& Id,const int D) {
    int i,j,k;
    bool accept;
    // rearrange the columns in such a way that the first row begins with [1,0,...,0,...] (D zeros)
    // remember that every row has precisely D zeros and D ones since it is the slack matrix of a simplicial 2-level polytope
    int * temp_col;
    alloc(temp_col,num_rows_M,int);
    bool found = false;
    if (M[0][0] != 1) {
        for (j = 1; j < num_cols_M && !found; ++j) {
            if (M[0][j] == 1) {
                found = true;
                for (i = 0; i < num_rows_M; ++i) {
                    temp_col[i] = M[i][j];
                    M[i][j] = M[i][0];
                    M[i][0] = temp_col[i];
                }
            }
        }
    }
    for (k = 1; k < D+1; ++k) {
        found = false;
        if (M[0][k] != 0) {
            for (j = k+1; j < num_cols_M && !found; ++j) {
                if (M[0][j] == 0) {
                    found = true;
                    for (i = 0; i < num_rows_M; ++i) {
                        temp_col[i] = M[i][j];
                        M[i][j] = M[i][k];
                        M[i][k] = temp_col[i];
                    }
                }
            }
        }
    }
    free(temp_col);
    
    // rearranging rows of M
    int * temp_row;
    alloc(temp_row,num_cols_M,int);
    int temp_idx = 0;
    for (i = temp_idx+1; i < num_rows_M && temp_idx < D; ++i) {
        accept = true;
        for (j = 0; j < D && accept; ++j) accept = (M[i][j+1] == Id[temp_idx][j]);
        
        if (accept) {
            assign_array(temp_row,M[i],num_cols_M);
            assign_array(M[i],M[temp_idx+1],num_cols_M);
            assign_array(M[temp_idx+1],temp_row,num_cols_M);
            temp_idx++;
        }
    }
    free(temp_row);
}

// slack matrix construction
void construct_slack_matrix(int **& base_H,int **& ground_set_H,int *& A,int *& B,int **& slabs,int **& S,int **& S_new,const int size_ground_H, const int num_slabs,const int length_A,const int length_B,const int num_rows_S,const int num_cols_S, int & num_rows_S_new, int & num_cols_S_new,const int D) {
    int i, j, k, h;
    int ** all_rows;
    alloc(all_rows,2*num_slabs,int*);
    int s;
    num_cols_S_new = num_cols_S + length_A;
    int * temp_row;
    alloc(temp_row,num_cols_S_new,int);
    bool accept;
    
    k = 0;
    for (i = 0; i < num_slabs; ++i) {
        if (B[i] == 1) {
            my_inner_prod(ground_set_H[0],slabs[i],s,D);
            if (s != 0) temp_row[0] = 1;
            else temp_row[0] = 0;
            
            for (j = 0; j < num_cols_S; ++j) {
                my_inner_prod(base_H[j],slabs[i],s,D);
                if (s != 0) temp_row[1+j] = 1;
                else temp_row[1+j] = 0;
            }
            h = 1;
            for (j = 1; j < size_ground_H; ++j) {
                if (A[j] == 1) {
                    my_inner_prod(ground_set_H[j],slabs[i],s,D);
                    if (s != 0) temp_row[num_cols_S+h] = 1;
                    else temp_row[num_cols_S+h] = 0;
                    ++h;
                }
            }
            
            int num_vertices = 0;
            for (j = 0; j < num_cols_S_new; ++j)
                if (temp_row[j] == 0) num_vertices += 1;
            
            if (num_vertices >= D) {
                alloc(all_rows[k],num_cols_S_new,int);
                for (j = 0; j < num_cols_S_new; ++j) all_rows[k][j] = temp_row[j];
                ++k;
            }
            
            if ((num_cols_S_new - num_vertices) >= D) {
                alloc(all_rows[k],num_cols_S_new,int);
                for (j = 0; j < num_cols_S_new; ++j) all_rows[k][j] = 1-temp_row[j];
                ++k;
            }
        }
    }
    free(temp_row);
    int temp_num_all_rows = k;
    
    for (i = 0; i < 2*length_B; ++i) alloc(S_new[i],num_cols_S_new,int);
    
    num_rows_S_new = 0;
    // check maximality of rows
    bool is_maximal, is_subset;
    for (i = 0; i < temp_num_all_rows; ++i) {
        //alloc(S_new[i],num_cols_S_new,int);
        is_maximal = true;
        for (j = 0; j < temp_num_all_rows && is_maximal; ++j) {
            if (j != i) {
                is_subset = true;
                // Check if the indices of zeros in all_rows[i] is a subset of all_rows[j]
                for (k = 0; k < num_cols_S_new && is_subset; ++k)
                    if (all_rows[i][k] == 0) is_subset = (all_rows[j][k] == 0);
                is_maximal = !(is_subset);
            }
        }
        if (is_maximal) {
            assign_array(S_new[num_rows_S_new],all_rows[i],num_cols_S_new);
            num_rows_S_new++;
        }
    }
    
    for (i = 0; i < temp_num_all_rows; ++i) free(all_rows[i]);
    free(all_rows);
    
    // rearranging rows of S_new
    int n_row = 0;
    alloc(temp_row,num_cols_S_new,int);
    for (i = n_row+1; i < num_rows_S_new && n_row < D; ++i) {
        accept = true;
        for (j = 0; j < num_cols_S && accept; ++j)
            accept = (S_new[i][j+1] == S[n_row][j]);
        if (accept) {
            assign_array(temp_row,S_new[i],num_cols_S_new);
            assign_array(S_new[i],S_new[n_row+1],num_cols_S_new);
            assign_array(S_new[n_row+1],temp_row,num_cols_S_new);
            n_row++;
        }
    }
    free(temp_row);
}

bool checksimplicialcore(int ** S, const int D) {
    for (int i = 0; i < D; ++i) {
        if (S[i][i] != 1) return false;
        for (int j = i+1; j < D; ++j) if (S[i][j] != 0) return false;
    }
    return true;
}

void extractM(int ** S, int ** M, const int D) {
    int i,j;
    for (i = 0; i < D; ++i) {
        for (j = 0; j < D; ++j)
            if (i == 0 && j == 0) M[i][j] = 1;
            else if (i == 0 || j == 0) M[i][j] = 0;
            else M[i][j] = S[i-1][j-1];
    }
}

// invert the DxD matrix M
void invertM(int ** M, int ** Minv, const int D) {
    int i,j,k;
    
    int ** W = M;
    alloc(W,D,int*);
    for (i = 0; i < D; ++i) {
        alloc(W[i],D,int);
        std::memcpy(W[i],M[i],D * sizeof(int));
    }

    generate_id_matrix(D,Minv);

    int s;
    // Start Gaussian inversion
    for (i=0; i<D; ++i) {
        // subtract the right multiple of the i-th row of [W | Minv] from the kth row of [W | Minv], where k > i
        for (k=i+1; k<D; ++k) {
            s = W[k][i];
            if (s != 0) {
                for (j=0; j<D; ++j) {
                    W[k][j] = W[k][j] - s * W[i][j];
                    Minv[k][j] = Minv[k][j] - s * Minv[i][j];
                }
            }
        }
    }
    for (i = 0; i < D; ++i) free(W[i]);
    free(W);
}

void hash2size(const int S_hash,int & num_rows,int & num_cols, const int D) {
    num_rows = 1 + (((1 << D) - 1) & S_hash);
    num_cols = 1 + (((1 << D) - 1) & (S_hash >> D));
}

void canonicize(int ** S,const int num_rows,const int num_cols, const int n, const int m, setword * cg_vec) {
    // Initializations for Nauty
    DYNALLSTAT(int,lab,lab_sz);
    DYNALLSTAT(int,ptn,ptn_sz);
    DYNALLSTAT(int,orbits,orbits_sz);
    DYNALLSTAT(int,map,map_sz);
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
    DYNALLOC1(int,lab,lab_sz,n,"malloc");
    DYNALLOC1(int,ptn,ptn_sz,n,"malloc");
    DYNALLOC1(int,orbits,orbits_sz,n,"malloc");
    DYNALLOC1(int,map,map_sz,n,"malloc");
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

bool is_listed(setword ** list_2L,int * list_hash,const int list_length,setword * canonical_S,const int hash_S,const size_t length_canonical_S) {
    for (int i = 0; i < list_length; ++i) {
        if (hash_S == list_hash[i]) 
            if (is_equal(canonical_S,list_2L[i],length_canonical_S)) return true;
    }
    return false;
}

// Checks whether a given 0-1 matrix is the slack matrix of a D-dimensional 2-level polytope,
// by using the list of (D-1)-dimensional 2-level polytopes.
bool istwolevelpolytope(int ** S_new,const int num_rows_S_new,const int num_cols_S_new,int * atoms_hash,setword ** atoms_cg,const int n_atoms, const int D) {
    int i, j, k, h, l;
    // First test: check that every column contains at least D zeros
    // by construction, every row of S contains at least D zeros
    int num_facets_contain;
    for (j = 0; j < num_cols_S_new; ++j) {
        num_facets_contain = 0;
        for (i = 0; i < num_rows_S_new; ++i) if (S_new[i][j] == 0) num_facets_contain += 1;
        if (num_facets_contain < D) return false;
    }
    
    int ** zero_indices;
    int * num_zero_indices;
    alloc(zero_indices,num_rows_S_new,int*);
    alloc(num_zero_indices,num_rows_S_new,int);
    
    for (i = 0; i < num_rows_S_new; ++i) {
        k = 0;
        alloc(zero_indices[i],num_cols_S_new,int);
        zero_indices[i][0] = 0;
        for (j = 0; j < num_cols_S_new; ++j){
            if (S_new[i][j] == 0) {
                zero_indices[i][k] = j;
                ++k;
            }
        }
        num_zero_indices[i] = k;
    }
    
    int ** rows_S_Fi;
    int * num_rows_S_Fi;
    alloc(rows_S_Fi,num_rows_S_new,int*);
    alloc(num_rows_S_Fi,num_rows_S_new,int);
    
    bool is_maximal, is_subset;
    for (i = 0; i < num_rows_S_new; ++i) {
        alloc(rows_S_Fi[i],num_rows_S_new,int);
        l = 0; // current number of rows of S_Fi
        for (j = 0; j < num_rows_S_new; ++j) {
            if (j != i) {
                // the only rows in S_Fi are the one containing the maximal sets of zeros
                is_maximal = true;
                for (k = 0; k < num_rows_S_new && is_maximal; ++k) {
                    if ((k != j) && (k != i)) {
                        // Check if the set of zeros of S_new[j][.] intersected with the one of S_new[i][.]
                        // is a subset of the one S_new[k][.] intersected with the one of S_new[i][.]
                        is_subset = true;
                        for (h = 0; h < num_zero_indices[i] && is_subset; ++h) {
                            if (is_in_array(zero_indices[j],zero_indices[i][h],num_zero_indices[j]))
                                is_subset = is_in_array(zero_indices[k],zero_indices[i][h],num_zero_indices[k]);
                        }
                        is_maximal = !is_subset;
                    }
                }
                if (is_maximal) {
                    rows_S_Fi[i][l] = j;
                    ++l;
                }
            }
        }
        num_rows_S_Fi[i] = l;
    }
    
    // Go through all the rows and build the corresponding submatrix for each of them. If the input is a slack matrix,
    // this will compute the slack matrix of the corresponding facet and test is it appears in the list L_{D-1}
    int ** S_Fi;
    bool found = true;
    int n,m;

    for (i = 0; i < num_rows_S_new && found; ++i) {
        alloc(S_Fi,num_rows_S_Fi[i],int *);
        for (j = 0; j < num_rows_S_Fi[i]; ++j) {
            alloc(S_Fi[j],num_zero_indices[i],int);
            for (k = 0; k < num_zero_indices[i]; ++k)
                S_Fi[j][k] = S_new[rows_S_Fi[i][j]][zero_indices[i][k]];
        }
        
        setword * canonical_S_Fi;
        n = num_rows_S_Fi[i] + num_zero_indices[i];
        m = SETWORDSNEEDED(n);
        alloc(canonical_S_Fi,m*n,setword);
        canonicize(S_Fi,num_rows_S_Fi[i],num_zero_indices[i],n,m,canonical_S_Fi);
        int hash_S_Fi = ((num_zero_indices[i]-1) << D) + num_rows_S_Fi[i] - 1;
        
        found = is_listed(atoms_cg,atoms_hash,n_atoms,canonical_S_Fi,hash_S_Fi,m*n);

        // found = false;
        // for (it = 0; it < n_atoms && !found; it++) {
        //     hash2size(atoms_hash[it],num_rows_atom,num_cols_atom,D);
        //     if ((num_rows_S_Fi[i] == num_rows_atom) && (num_zero_indices[i] == num_cols_atom))
        //         found = is_equal(canonical_S_Fi,atoms_cg[it],m*n);
        // }
        
        free(canonical_S_Fi);
        for (j = 0; j < num_rows_S_Fi[i]; ++j) free(S_Fi[j]);
        free(S_Fi);
    }
    
    for (i = 0; i < num_rows_S_new; ++i) free(rows_S_Fi[i]);
    free(rows_S_Fi);
    free(num_rows_S_Fi);
    
    for (i = 0; i < num_rows_S_new; ++i) free(zero_indices[i]);
    free(zero_indices);
    free(num_zero_indices);
    
    return found;
}


// compute the power of an int, x base, p exponent
int my_pow(int x, unsigned int p) {
    int tot = 1;
    for (int i = 1; i <= p; ++i) tot *= x;
    return tot;
}

DYNALLSTAT(int,allp,allp_sz);
DYNALLSTAT(int,id,id_sz);

// Recursive routine used by allgroup.
static void my_groupelts(levelrec *lr, int n, int level, void (*action)(int*,int,std::vector<std::vector<int> >&,int &),int *before, int *after, int *id,std::vector<std::vector<int> >& automorphism_base, int & num_tot) {
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
void my_allgroup(grouprec *grp, void (*action)(int*,int,std::vector<std::vector<int> >&,int &),std::vector<std::vector<int> >& automorphism_base, int & num_tot) {
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


void writeautom(int *p, int n,std::vector<std::vector<int> >& automorphism_base, int & num_tot) {
    std::vector<int> permutation(n);
    for (int i = 0; i < n; ++i) permutation[i] = p[i];
    automorphism_base.push_back(permutation);
    permutation.clear();
    num_tot++;
}

void construct_automorphism_base(int ** S,const int num_rows,const int num_cols, const int n, const int m, std::vector<std::vector<int> >& automorphism_base, int & num_autom_base) {
    
    int i,j;
    // Initializations for Nauty
    DYNALLSTAT(int,lab,lab_sz);
    DYNALLSTAT(int,ptn,ptn_sz);
    DYNALLSTAT(int,orbits,orbits_sz);
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
    DYNALLOC1(int,lab,lab_sz,n,"malloc");
    DYNALLOC1(int,ptn,ptn_sz,n,"malloc");
    DYNALLOC1(int,orbits,orbits_sz,n,"malloc");
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

// test if the 2-level polytope having slack-matrix S_new is a suspension
bool is_susp(int ** S_new,int num_rows_S_new,int num_cols_S_new) {
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
       int * zeros_idx, * ones_idx;
        alloc(zeros_idx,num_cols_S_new,int); 
        alloc(ones_idx,num_cols_S_new,int);
        int num_zeros, num_ones;
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
                int * char_F1;
                alloc(char_F1,num_cols_S_new,int);
                for (l = 0; l < num_cols_S_new; ++l)
                    char_F1[l] = 1;
                is_contained = true;
                for (l = 0; l < num_ones && is_contained; ++l) {
                    int * translated_F1_point;
                    alloc(translated_F1_point,num_rows_S_new,int);
                    for (h = 0; h < num_rows_S_new; ++h)
                        translated_F1_point[h] = S_new[h][ones_idx[l]] + S_new[h][zeros_idx[k]] - S_new[h][ones_idx[j]];
                    found = false;
                    for (h = 0; h < num_zeros && !found; ++h) {
                        int * F0_point;
                        alloc(F0_point,num_rows_S_new,int);
                        for (t = 0; t < num_rows_S_new; t++)
                            F0_point[t] = S_new[t][zeros_idx[h]];
                        found = is_equal(translated_F1_point,F0_point,num_rows_S_new);
                        free(F0_point);
                        if (found)
                            char_F1[zeros_idx[h]] = 0;
                    }
                    free(translated_F1_point);
                    is_contained &= found;  
                }
                
                if (is_contained) {
                    int * intersect_rows_containing_F1;
                    alloc(intersect_rows_containing_F1,num_cols_S_new,int);
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
                    flag = is_equal(intersect_rows_containing_F1,char_F1,num_cols_S_new);
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


// Test if the polar of the 2-level polytope having slack-matrix S_new is a still 2_level
void is_polar(int ** S_new,int hash_S_new, setword * canonical_S_new, int num_rows_S_new,int num_cols_S_new,const int n, const int m,int * LD_hash,setword ** LD,int current_LD,const int D) {
    int i,j;
    int ** S_T;
    alloc(S_T,num_cols_S_new,int*);
    for (i = 0; i < num_cols_S_new; ++i) {
        alloc(S_T[i],num_rows_S_new,int);
        for (j = 0; j < num_rows_S_new; ++j) S_T[i][j] = S_new[j][i];
    }

    int hash_S_T = ((num_rows_S_new-1) << D) + num_cols_S_new - 1;
    setword * canonical_S_T;

    alloc(canonical_S_T,m*n,setword);
    canonicize(S_T,num_cols_S_new,num_rows_S_new,n,m,canonical_S_T);

    for (i = 0; i < num_cols_S_new; ++i) free(S_T[i]);
    free(S_T);

    if (hash_S_new == hash_S_T) {
        if (is_equal(canonical_S_T,canonical_S_new,m*n)) n_polar += 1; // self-polar
    }
    else if (is_listed(LD,LD_hash,current_LD,canonical_S_T,hash_S_T,m*n))
        n_polar += 2;
}


// check if the slack matrix S is already listed in LD; if not, add it to LD //int *& LD_hash, setword **& LD
void to_list(int **& S_new,int & num_rows_S_new,int & num_cols_S_new,FILE * my_outputfile,int *& LD_hash,setword **& LD,int & current_LD, const int D, int & verbose) {
    unsigned int i, j;
    int n, m;
    int hash_S_new = ((num_cols_S_new-1) << D) + num_rows_S_new - 1;
    
    setword * canonical_S_new;
    n = num_rows_S_new + num_cols_S_new;
    m = SETWORDSNEEDED(n);
    alloc(canonical_S_new,m*n,setword);
    canonicize(S_new,num_rows_S_new,num_cols_S_new,n,m,canonical_S_new);

    // Browse through all nonincidence graphs that have the same hash to see if one of them
    // is isomorphic to the current nonincidence graph
    if (!is_listed(LD,LD_hash,current_LD,canonical_S_new,hash_S_new,m*n)) {
        alloc(LD[current_LD],m*n,setword);
        assign_array(LD[current_LD],canonical_S_new,m*n);
        LD_hash[current_LD] = hash_S_new;
        current_LD++;

        for (i = 0; i < num_rows_S_new; ++i){
            for (j = 0; j < num_cols_S_new; ++j) fprintf(my_outputfile,"%d",S_new[i][j]);
            fprintf(my_outputfile,"\n");
        }
        fprintf(my_outputfile,"-\n");

        // tests for subclasses of 2-level polytopes
        if (verbose != 0) {
            bool has_simplicial = false;
            int num_zeros;
            for (i = 0; i < num_rows_S_new && !has_simplicial; ++i) {
                num_zeros = 0;
                for (j = 0; j < num_cols_S_new; ++j) if (S_new[i][j] == 0) num_zeros++;
                has_simplicial = (num_zeros == D);
            }
            if (has_simplicial) simplicial_facet++;
            
            bool STAB = false;
            for (i = 0; i < num_cols_S_new && !STAB; ++i) {
                num_zeros = 0;
                for (j = 0; j < num_rows_S_new; ++j)
                    if (S_new[j][i] == 0) num_zeros++;
                STAB = (num_zeros == D);
            }
            if (STAB) stab++;
            
            bool CS = true;
            int num_ones;
            for (i = 0; i < num_rows_S_new && CS; ++i) {
                num_ones = 0;
                for (j = 0; j < num_cols_S_new; ++j)
                    if (S_new[i][j] == 1) num_ones++;
                CS = (num_ones == num_cols_S_new/2);
            }
            if (CS) cs++;
            
            if (is_susp(S_new,num_rows_S_new,num_cols_S_new)) n_suspensions++;
            
            is_polar(S_new,hash_S_new,canonical_S_new,num_rows_S_new,num_cols_S_new,n,m,LD_hash,LD,current_LD,D);
        }
    }
    free(canonical_S_new);
}



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
    
    begin_tot_time = my_clock::now();
    
    FILE * my_inputfile, * my_outputfile;
    
    char inputfile_Name[6],outputfile_Name[20];
    sprintf(inputfile_Name,"%dd.txt",D-1);
    
    my_inputfile = fopen(inputfile_Name,"r");
    
    int i, j, k, h, it; // iterators
    
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
    
    it = 0;
    
    printf("Preprocessing sizes of slack matrices... ");
    i = 0;
    j = 0;
    char temp_s;
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
    printf("OK\n");
    
    rewind(my_inputfile);
    
    int num_rows, num_cols;
    // allocating exact memory for each slack matrix of 2-level (D-1)-polytope
    for (i = 0; i < n_atoms; ++i) {
        hash2size(atoms_hash[i],num_rows,num_cols,D);
        alloc(atoms[i],num_rows,int*);
        for (j = 0; j < num_rows; ++j) alloc(atoms[i][j],num_cols,int);
    }
    
    printf("Reading all %d-dimensional 2-level polytopes... ",D-1);
    if (verbose == 2) printf("\n");
    i = 0;
    j = 0;
    it = 0;
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
    printf("OK\nNumber of polytopes read = %d\n",it);
    fclose(my_inputfile);
    
    printf("Computing canonical forms for all nonincidence graphs... ");
    setword ** atoms_cg;
    alloc(atoms_cg,n_atoms,setword *);
    
    int n,m;
    for (i = 0; i < n_atoms; ++i) {
        // Get the number of rows and columns of current atom
        hash2size(atoms_hash[i],num_rows,num_cols,D);
        n = num_rows + num_cols;
        m = SETWORDSNEEDED(n);
        alloc(atoms_cg[i],m*n,setword);
        canonicize(atoms[i],num_rows,num_cols,n,m,atoms_cg[i]);
    }
    printf("OK\n");
    printf("Processing bases...\n");
    

    if ((first_base == 0) && (last_base == n_atoms - 1)) sprintf(outputfile_Name,"%dd.txt",D);
    else sprintf(outputfile_Name,"%dd_%d-%d.txt",D,first_base,last_base);
    my_outputfile = fopen(outputfile_Name,"w+");
    
    int n_simplicial = 0;
    int current_LD = 0;
    // first attempt to use the characterization of simplicial 2-level polytopes
    if (first_base == 0) {
        printf("\nBase #1:\n");
        printf("Constructing slack-matrices of simplicial 2-level polytopes... ");
        begin_simplicial = my_clock::now();
        int K,N;
        int ** S_new;
        int num_rows_S_new,num_cols_S_new;

        for (auto N : factor(D)) {
            K = D/N;
            slack_matrix_simplicial_2L(K,N,S_new,num_rows_S_new,num_cols_S_new);
            push_simplicial_core(S_new,num_rows_S_new,num_cols_S_new,atoms[0],D);
            to_list(S_new,num_rows_S_new,num_cols_S_new,my_outputfile,LD_hash,LD,current_LD,D,verbose);
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
        hash2size(atoms_hash[it],num_rows_S,num_cols_S,D);
        
        n_base = it + 1;
        printf("\nBase #%d:\n",n_base);
        
        printf("Simplicial core? ");
        if (checksimplicialcore(atoms[it],D)) printf("OK\n");
        else {
            printf("Fail\n");
            return 1;
        }
        
        // Extract embedding transformation matrix M_d(0)
        int ** M, ** Minv;
        alloc(M,D,int*);
        for (i = 0; i < D; ++i) alloc(M[i],D,int);
        
        // extract the top (D-1)x(D-1) matrix M_{d-1} and extend it to M_d(0)
        extractM(atoms[it],M,D);
        
        if (verbose == 2) {
            printf("M_d(0) = \n");
            for (i = 0; i < D; ++i) {
                printf("[");
                for (j = 0; j < D; ++j) {
                    printf("%d", M[i][j]);
                    if (j != D-1) printf(" ");
                }
                printf("]\n");
            }
        }
        //printf("OK\n");
        
        alloc(Minv,D,int*);
        for (i = 0; i < D; ++i) alloc(Minv[i],D,int);
        
        invertM(M,Minv,D);
        
        if (verbose == 2) {
            printf("M_d^{-1}(0) = \n");
            for (i = 0; i < D; ++i) {
                printf("[");
                for (j = 0; j < D; ++j) {
                    printf("%d", Minv[i][j]);
                    if (j != D-1) printf(" ");
                }
                printf("]\n");
            }
        }
        
        printf("Constructing H-embedding of facets of the base... ");
        int ** facets_base;
        int * E;
        alloc(E,D,int);
        alloc(facets_base,num_rows_S,int*);
        int num_facets_base = 0; // number of element currently in facets_base
        for (i = 0; i < num_rows_S; ++i) {
            alloc(facets_base[i],D,int);
            if (atoms[it][i][D-1] == 1) for (j = 0; j < D-1; ++j) E[j+1] = 1-atoms[it][i][j];
            else for (j = 0; j < D-1; ++j) E[j+1] = atoms[it][i][j];
            
            bool found = false;
            for  (j = 0; j < num_facets_base && !found; ++j)
                found = is_equal(facets_base[j],E,D);
            if (!found) {
                if (verbose != 0) {
                    for (j = 0; j < D; ++j) printf("%d",E[j]);
                    printf(" ");
                }
                assign_array(facets_base[num_facets_base],E,D);
                num_facets_base++;
            }
        }
        printf("OK\n");
        
        printf("Constructing automorphism group of the base and extending it to R^D... ");
        n = num_rows_S + num_cols_S;
        m = SETWORDSNEEDED(n);
        //int ** automorphism_base;
        std::vector<std::vector<int> > automorphism_base;
        int num_autom_base = 0;
        
        construct_automorphism_base(atoms[it],num_rows_S,num_cols_S,n,m,automorphism_base,num_autom_base);

        int ** d_aut_collection;
        alloc(d_aut_collection,num_autom_base,int *);
        
        //printf("\n");
        for (i = 0; i < num_autom_base; ++i) {
            alloc(d_aut_collection[i],D,int);
            for  (j = num_rows_S; j < num_rows_S + D; ++j) {
                d_aut_collection[i][j- num_rows_S] = automorphism_base[i][j] - num_rows_S;
                //printf("%d",d_aut_collection[i][j- num_rows_S]);
            }
            //printf("\n");
        }
        automorphism_base.clear();
        printf("OK\n");
        
        // Create the set Vert(P_0) (in V-embedding)
        printf("Building V-embedding of base... ");
        int ** base_V;
        alloc(base_V,num_cols_S,int *);
        
        // Loop through all vertices
        for (i = 0; i < num_cols_S; ++i) {
            alloc(base_V[i],D,int);
            // Create a point whose first coordinate is 0, and the others are the D-1 first bits
            // of the jth column of the slack matrix S
            base_V[i][0] = 0;
            for (j = 0; j < D-1; ++j) base_V[i][j+1] = atoms[it][j][i];
            
            if (verbose != 0) {
                // Print point
                printf("[");
                for (j = 0; j < D; ++j){
                    printf("%d",base_V[i][j]);
                    if (j != D-1) printf(",");
                }
                printf("] ");//
            }
        }
        printf("OK\n");
        
        // Create Vert(P_0) (H-embedding this time), the set of fixed points
        printf("Building H-embedding of base... ");
        int ** base_H;
        alloc(base_H,num_cols_S,int *);
        
        for (i = 0; i < num_cols_S; ++i) {
            alloc(base_H[i],D,int);
            my_matrix_prod(Minv,base_V[i],base_H[i],D,D);
            if (verbose != 0) {
                // Print point
                printf("[");
                for (j = 0; j < D; ++j) {
                    printf("%d",base_H[i][j]);
                    if (j != D-1) printf(",");
                }
                printf("] ");//
            }
        }
        printf("OK\n");
        
        // Create the V-embedding of the reduced ground set (by means of translations)
        printf("Building V-embedding of the ground set... ");
        int ** ground_V;
        int size_ground_V = (my_pow(3,D-1)+1)/2;
        alloc(ground_V,size_ground_V,int *);
        
        int * count;
        alloc(count,D+1,int);
        // initialize count to 0
        std::memset(count,0,(D+1) * sizeof(int));
        bool carry;
        
        alloc(ground_V[0],D,int);
        ground_V[0][0] = 1;
        std::memset(ground_V[0]+1,0,(D-1) * sizeof(int));
        
        if (verbose != 0) {
            // Print point
            printf("[");
            for (i = 0; i < D; ++i){
                printf("%d",ground_V[0][i]);
                if (i != D-1) printf(",");
            }
            printf("] ");//
        }
        
        k = 1;
        for (i = D-1; i > 0; --i) {
            std::memset(count,0,D * sizeof(int));
            
            while (count[i] == 0) {
                alloc(ground_V[k],D,int);
                ground_V[k][0] = 1;
                std::memset(ground_V[k]+1,0,(i-1) * sizeof(int));
                ground_V[k][i] = 1;
                std::memset(ground_V[k]+i+1,0,(D-i-1) * sizeof(int));
                
                // Extract a vector in {-1,0,1}^{D-i-1} to fill the vector
                for (j = i+1; j < D; ++j) ground_V[k][j] = count[j] - 1;
                
                if (verbose != 0) {
                    // Print point
                    printf("[");
                    for (j = 0; j < D; ++j) {
                        printf("%d",ground_V[k][j]);
                        if (j != D-1) printf(",");
                    }
                    printf("] ");//
                }
                ++k;
                // Increase counter, by performing mod-3 computation
                j = D-1;
                do {
                    carry = (count[j] == 2);
                    count[j] = (count[j] + 1) % 3;
                    j--;
                } while (carry && j >= 0);
            }
        }
        printf("OK\n");
        
        // Create ground set
        printf("Building H-embedding of the reduced ground set... ");
        int ** ground_H;
        alloc(ground_H,size_ground_V,int *);
        int size_ground_H = 0;
        bool accept;
        int xE;
        
        for (i = 0; i < size_ground_V; ++i) {
            int * point;
            alloc(point,D,int);
            alloc(ground_H[i],D,int);
            my_matrix_prod(Minv,ground_V[i],point,D,D);
            
            // Facet reduction of the ground set:
            // we can throw away all the points x of the ground set where we do not have x(E) in {-1,0,1}
            // for x(E) >= 0, x(E) <= 1 facet of the base, E subset of {2,...,d}
            accept = true;
            for (j = 0; j < num_facets_base && accept; ++j) {
                xE = 0;
                for (k = 1; k < D; ++k)
                    if (facets_base[j][k] != 0) xE += point[k];
                accept = ((xE == -1) || (xE == 0) || (xE == 1));
            }
            
            if (accept) {
                if (verbose != 0) {
                    // Print
                    printf("[");
                    for (j = 0; j < D; ++j) {
                        printf("%d",point[j]);
                        if (j != D-1) printf(",");
                    }
                    printf("] ");//
                }
                assign_array(ground_H[size_ground_H],point,D);
                size_ground_H++;
            }
            free(point);
        }
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
        
        // printf("\n");
        alloc(orbits[0],num_autom_base,int*);
        for (j = 0; j < num_autom_base; ++j) {
            alloc(orbits[0][j],D,int);
            assign_array(orbits[0][j],ground_H[0],D);
            // printf("[");
            // for (k = 0; k < D; ++k) {
            //     printf("%d",orbits[0][j][k]);
            //     if (k != D-1)
            //         printf(",");
            // }
            // printf("] ");
        }
        // printf("\n");
        
        bool found_first_non_zero,reject;
        
        for (i = 1; i < size_ground_H; ++i) {
            alloc(orbits[i],num_autom_base,int*);
            alloc(orbits[i][0],D,int);
            assign_array(orbits[i][0],ground_H[i],D);
            
            // printf("[");
            // for (k = 0; k < D; ++k) {
            //     printf("%d",orbits[i][0][k]);
            //     if (k != D-1)
            //         printf(",");
            // }
            // printf("] ");
            
            for (j = 1; j < num_autom_base; ++j) {
                alloc(orbits[i][j],D,int);
                orbits[i][j][0] = 1;
                found_first_non_zero = false;
                reject = false;
                for (k = 1; k < D && !reject; ++k) {
                    orbits[i][j][k] = 0;
                    for (h = 1; h < D; ++h)
                        orbits[i][j][k] += ground_H[i][h]*(base_H[d_aut_collection[j][h-1]][k] - base_H[d_aut_collection[j][D-1]][k]);
                    if (orbits[i][j][k] != 0 && !found_first_non_zero) {
                        if (orbits[i][j][k] == -1) reject = true;
                        found_first_non_zero = true;
                    }
                }
                if (reject) std::memset(orbits[i][j],0,D * sizeof(int));
                // printf("[");
                // for (k = 0; k < D; ++k) {
                //     printf("%d",orbits[i][j][k]);
                //     if (k != D-1)
                //         printf(",");
                // }
                // printf("] ");
            }
            // printf("\n");
        }
        printf("OK\n");
        
        // Compute the slabs: inequalities x(E) <= 1, x(E) >= 0 that are valid for the base_H
        count[0] = 1;
        std::memset(count+1,0,D * sizeof(int));
        
        printf("Building slabs... ");
        int ** slabs;
        alloc(slabs,1 << D,int *);
        int s;
        int num_slabs = 0;
        
        int * normal_vector;
        alloc(normal_vector,D,int);
        
        while (count[D] == 0) {
            std::memcpy(normal_vector, count, D * sizeof(int));
            
            accept = true;
            for (j = 0; j < num_cols_S && accept; ++j) {
                my_inner_prod(normal_vector,base_H[j],s,D);
                accept = ((s == 0) || (s == 1));
            }
            
            // Add normal vector of slab to the list if it contains all points of the base
            if (accept) {
                if (verbose != 0) {
                    // Print normal vector
                    for (j = 0; j < D; ++j) printf("%d",normal_vector[j]);
                    printf(" ");
                }
                alloc(slabs[num_slabs],D,int);
                assign_array(slabs[num_slabs],normal_vector,D);
                num_slabs++;
            }
            
            // Increase counter, by performing mod-2 computations
            i = 0;
            do {
                carry = (count[i] == 1);
                count[i] = (count[i] + 1) % 2;
                ++i;
            } while (carry && i <= D);
        }
        printf("OK\n");
        free(count);
        
        int ** slab_points_sat;
        alloc(slab_points_sat,size_ground_H,int*);
        
        printf("Building incidences between points and slabs... ");
        // Check points versus slabs incidence (for each point, list the slabs containing it)
        for (i = 0; i < size_ground_H; ++i) {
            alloc(slab_points_sat[i],num_slabs,int);
            for (j = 0; j < num_slabs; ++j) {
                my_inner_prod(ground_H[i],slabs[j],s,D);
                if ((s == 0) || (s == 1)) slab_points_sat[i][j] = 1;
                else slab_points_sat[i][j] = 0;
                //if (verbose != 0) printf("%d",slab_points_sat[i][j]);
            }
            //if (verbose != 0) printf(" ");
        }
        printf("OK\n");
        
        
        // Construct the incompatibility matrix
        printf("Constructing the incompatibility matrix... ");
        int ** incompatibility_adjM;
        alloc(incompatibility_adjM,size_ground_H,int*);
        bool is_incompat;
        int s_i, s_j;
        
        for (i = 0; i < size_ground_H; ++i) {
            alloc(incompatibility_adjM[i],i,int);
            for (j = 0; j < i; ++j) {
                is_incompat = false;
                for (k = 0; k < num_facets_base && !is_incompat; ++k) {
                    s_i = 0;
                    s_j = 0;
                    for (h = 1; h < D; ++h) {
                        if (facets_base[k][h] != 0) {
                            s_i += ground_H[i][h];
                            s_j += ground_H[j][h];
                        }
                    }
                    is_incompat = (s_i*s_j == -1);
                }
                if (is_incompat) incompatibility_adjM[i][j] = 1;
                else incompatibility_adjM[i][j] = 0;
                //printf("%d",incompatibility_adjM[i][j]);
            }
            //printf("\n");
        }
        printf("OK\n");
        
        
        printf("Lauching Ganter's next-closure algorithm and checking 2-levelness... ");
        if (verbose != 0) printf("\n");
        
        int N_closed_sets_current_base = 0;
        int N_2level_current_base = 0;
        
        if (verbose != 0) printf("%*c  | 2-level | next_cl      | slack-matrix | 2-lev_time\n", size_ground_H, ' ');
        
        int * A;
        alloc(A,size_ground_H,int);
        std::memset(A,0,size_ground_H * sizeof(int));
        
        int * dchcl, * inccl, * I, * CI;
        alloc(dchcl,size_ground_H,int);
        alloc(inccl,size_ground_H,int);
        alloc(I,size_ground_H,int);
        alloc(CI,size_ground_H,int);
        int * B;
        alloc(B,num_slabs,int);
        
        while (!is_all_ones(A,size_ground_H)) {
            i = 0;
            if (verbose != 0) begin_nextcl = my_clock::now();
            do {
                while(A[i] == 1) ++i;
                inc(A,i,I,size_ground_H); // I = inc(A,i)
                discreteconvexhull_cl(I,B,dchcl,slab_points_sat,size_ground_H,num_slabs);
                incompatibility_cl(dchcl,inccl,incompatibility_adjM,size_ground_H);
                lexmax_symmetric(inccl,CI,ground_H,size_ground_H,orbits,num_autom_base,D);
                ++i;
            } while (!is_sqsubseteq(I,CI,size_ground_H));
            if (verbose != 0) {
                end_nextcl = my_clock::now();
                time_nextcl = end_nextcl-begin_nextcl;
            }
            
            assign_array(A,CI,size_ground_H);
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
            alloc(S_new,2*length_B,int *);
            
            if (verbose != 0) begin_slack_matrix = my_clock::now();
            
            construct_slack_matrix(base_H,ground_H,A,B,slabs,atoms[it],S_new,size_ground_H,num_slabs,length_A,length_B,num_rows_S,num_cols_S,num_rows_S_new,num_cols_S_new,D);
            
            if (verbose != 0) {
                end_slack_matrix = my_clock::now();
                time_slack_matrix = end_slack_matrix - begin_slack_matrix;
            }
            
            
            bool istwolevel = false;
            accept = true;
            int num_vertices_Fi;
            if (verbose != 0) begin_skip_test = my_clock::now();
            for (i = 0; i < num_rows_S_new && accept; ++i) {
                num_vertices_Fi = 0;
                for (j = 0; j < num_cols_S_new; ++j)
                    if (S_new[i][j] == 0) num_vertices_Fi++;
                accept = (num_vertices_Fi <= num_cols_S);
            }
            if (verbose != 0) {
                end_skip_test = my_clock::now();
                time_skip_test = end_skip_test - begin_skip_test;
            }
            
            if (accept) {
                if (verbose != 0) begin_2L_test = my_clock::now();
                istwolevel = istwolevelpolytope(S_new,num_rows_S_new,num_cols_S_new,atoms_hash,atoms_cg,n_atoms,D);
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
                to_list(S_new,num_rows_S_new,num_cols_S_new,my_outputfile,LD_hash,LD,current_LD,D,verbose);
                N_2level_current_base++;
                total_2level++;
            }
            
            for (i = 0; i < 2*length_B; ++i) free(S_new[i]);
            free(S_new);
        }
        
        free(inccl);
        free(dchcl);
        free(CI);
        free(I);
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
    
    printf("\nTotal #closed sets found = %d\n", N_closed_sets);
    printf("#2-level polytopes tested = %d\n", total_2level);
    printf("#non-isomorphic 2-level polytopes found = %d\n", current_LD);
    printf("Total elapsed time = %lfs\n", tot_elapsed_time.count());
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
        hash2size(atoms_hash[i],num_rows,num_cols,D);
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