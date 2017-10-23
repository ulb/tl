#include <iostream>
#include <fstream>
#include <vector>

#include "twolvl/loadall.hpp"
#include "twolvl/load.hpp"
#include "base/Atom.hpp"
#include "twolvl/is_listed.hpp"
#include "twolvl/dump.hpp"
#include "nauty.h"

int main (int argc, const char* argv[]) {

    std::ios_base::sync_with_stdio(true);

    if (argc < 1 || argc > 2) {
        std::cerr << "usage: dedup [<verbosity>]" << std::endl;
        return 1;
    }

    //int verbose = (argc > 2) ? atoi(argv[2]) : 0; // not use ATM

    // sift through popcorn
    std::vector<base::Atom<int>> unique;
    std::vector<base::Atom<int>> polytopes;
    while ( true ) {

        if ( !twolvl::load(std::cin, polytopes) ) break ;

        auto& polytope = polytopes[0]; // ugly we use a length-1 vector atm

        if ( !twolvl::is_listed(unique, polytope.cg, polytope.rows, polytope.columns, polytope.cg_length) ) {
            twolvl::dump(std::cout, polytope);
            unique.push_back(polytope);
        }
        else polytope.teardown();

        polytopes.clear(); // ugly hack atm, we use a vector containing a single element

    }

    for (auto& polytope : unique) polytope.teardown();
    unique.clear();

    return 0;

}
