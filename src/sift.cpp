#include <iostream>
#include <fstream>
#include <vector>

#include "twolvl/loadall.hpp"
#include "twolvl/load.hpp"
#include "twolvl/dump.hpp"
#include "base/Atom.hpp"
#include "twolvl/istwolevelpolytope.hpp"
#include "search/BTrie.hpp"

int main (int argc, const char* argv[]) {

    std::ios_base::sync_with_stdio(true);

    if (argc < 2 || argc > 3) {
        std::cerr << "usage: sift <kernel> [<verbosity>]" << std::endl;
        return 1;
    }

    std::string kernel_filename(argv[1]);
    //int verbose = (argc > 2) ? atoi(argv[2]) : 0; // not use ATM

    // read kernel file
    std::cerr << "Reading kernel file..." << std::endl;
    std::vector<base::Atom<int>> facets;
    std::ifstream kernel(kernel_filename, std::ifstream::in);
    twolvl::loadall(kernel, facets);
    kernel.close();
    std::cerr << "Done. Loaded " << facets.size() << " facets." << std::endl;
    search::BTrie<uint_fast8_t, 256> trie;
    for (auto& facet : facets) {
        trie.insert(facet.xpt, facet.xend);
        facet.teardown();
    }
    facets.clear();

    // sift through popcorn
    std::vector<base::Atom<int>> polytopes;
    while ( true ) {

        if ( !twolvl::load(std::cin, polytopes) ) break ;

        auto& polytope = polytopes[0]; // ugly we use a length-1 vector atm

        if ( twolvl::istwolevelpolytope(trie, polytope) ) twolvl::dump(std::cout, polytope);

        polytope.teardown();
        polytopes.clear(); // ugly hack atm, we use a vector containing a single element

    }

    return 0;

}
