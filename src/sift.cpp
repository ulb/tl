#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "nauty.h"
#include "twolvl/loadall.hpp"
#include "twolvl/load.hpp"
#include "twolvl/dump.hpp"
#include "base/Atom.hpp"
#include "twolvl/istwolevelpolytope.hpp"
#include "array/LexicographicOrder.hpp"

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

    std::vector<std::pair<setword*,setword*>> cgs;
    cgs.reserve(facets.size());
    for (auto& facet : facets) cgs.emplace_back(facet.cg, facet.cg_end);
    array::LexicographicOrder<setword*> comp;
    std::sort(cgs.begin(), cgs.end(), comp);

    // sift through popcorn
    std::vector<base::Atom<int>> polytopes;
    while ( true ) {

        if ( !twolvl::load(std::cin, polytopes) ) break ;

        auto& polytope = polytopes[0]; // ugly we use a length-1 vector atm

        if ( twolvl::istwolevelpolytope(comp, cgs, polytope) ) twolvl::dump(std::cout, polytope);

        polytope.teardown();
        polytopes.clear(); // ugly hack atm, we use a vector containing a single element

    }

    cgs.clear();
    for (auto& facet : facets) facet.teardown();
    facets.clear();

    return 0;

}
