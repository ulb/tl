#include <iostream>
#include <vector>

#include "twolvl/loadall.hpp"
#include "twolvl/load.hpp"
#include "twolvl/dump.hpp"
#include "base/Atom.hpp"
#include "search/BTrie.hpp"

int main () {

    std::ios_base::sync_with_stdio(true);

    //if (argc < 1 || argc > 2) {
        //std::cerr << "usage: dedup [<verbosity>]" << std::endl;
        //return 1;
    //}

    std::vector<base::Atom<int>> polytopes;
    search::BTrie<uint_fast8_t, 4> trie;
    while ( true ) {

        if ( !twolvl::load(std::cin, polytopes) ) break ;

        auto& polytope = polytopes[0]; // ugly we use a length-1 vector atm

        if ( !trie.insert(polytope.xpt, polytope.xend) ) twolvl::dump(std::cout, polytope);

        polytope.teardown();
        polytopes.clear(); // ugly hack atm, we use a vector containing a single element

    }

    return 0;

}
