#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "nauty.h"
#include "tl/loadall.hpp"
#include "tl/load.hpp"
#include "tl/dump.hpp"
#include "tl/Polytope.hpp"
#include "tl/istwolevelpolytope.hpp"
#include "array/LexicographicOrder.hpp"

int main (int argc, const char* argv[]) {

    std::ios_base::sync_with_stdio(false);

    if (argc < 2 || argc > 3) {
        std::cerr << "usage: sift <kernel> [<verbosity>]" << std::endl;
        return 1;
    }

    std::string kernel_filename(argv[1]);
    //int verbose = (argc > 2) ? atoi(argv[2]) : 0; // not use ATM

    // read kernel file
    std::cerr << "Reading kernel file..." << std::endl;
    std::vector<tl::Polytope<int>> facets;
    std::ifstream kernel(kernel_filename, std::ifstream::in);
    tl::loadall(kernel, facets);
    kernel.close();
    std::cerr << "Done. Loaded " << facets.size() << " facets." << std::endl;

    std::vector<tl::CanonicalGraph<int>> graphs;
    std::vector<std::pair<setword*,setword*>> cgs;
    graphs.reserve(facets.size());
    cgs.reserve(facets.size());
    for (auto& facet : facets) {
        graphs.emplace_back(facet);
        auto& graph = graphs[graphs.size()-1];
        cgs.emplace_back(graph.begin, graph.end);
    }

    for (auto& facet : facets) facet.teardown();
    facets.clear();

    array::LexicographicOrder<setword*> comp;
    std::sort(cgs.begin(), cgs.end(), comp);

    // sift through popcorn
    std::vector<tl::Polytope<int>> polytopes;
    while ( true ) {

        if ( !tl::load(std::cin, polytopes) ) break ;

        auto& polytope = polytopes[0]; // ugly we use a length-1 vector atm

        if ( tl::istwolevelpolytope(comp, cgs, polytope) ) tl::dump(std::cout, polytope);

        polytope.teardown();
        polytopes.clear(); // ugly hack atm, we use a vector containing a single element

    }

    cgs.clear();
    for (auto& graph : graphs) graph.teardown();
    graphs.clear();

    return 0;

}
