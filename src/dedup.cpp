#include <iostream>
#include <vector>

#include "twolvl/loadall.hpp"
#include "twolvl/load.hpp"
#include "twolvl/dump.hpp"
#include "base/Atom.hpp"
#include "search/Trie.hpp"

int main (int argc, const char* argv[]) {

    std::ios_base::sync_with_stdio(true);

    if (argc < 1 || argc > 2) {
        std::cerr << "usage: dedup [<verbosity>]" << std::endl;
        return 1;
    }

    //int verbose = (argc > 2) ? atoi(argv[2]) : 0; // not use ATM

    // sift through popcorn
    std::vector<base::Atom<int>> polytopes;
    search::Trie<int> trie;
    while ( true ) {

        if ( !twolvl::load(std::cin, polytopes) ) break ;

        auto& polytope = polytopes[0]; // ugly we use a length-1 vector atm

        if ( !trie.insert(polytope.cg_pt, polytope.cg_end) ) twolvl::dump(std::cout, polytope);
        
        polytope.teardown();

        polytopes.clear(); // ugly hack atm, we use a vector containing a single element

    }

    return 0;

}
