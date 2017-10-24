#include <iostream>
#include <vector>
#include <set>
#include <utility>

#include "twolvl/loadall.hpp"
#include "twolvl/load.hpp"
#include "twolvl/dump.hpp"
#include "base/Atom.hpp"
#include "array/LexicographicOrder.hpp"

int main () {

    std::ios_base::sync_with_stdio(true);

    //if (argc < 1 || argc > 2) {
        //std::cerr << "usage: dedup [<verbosity>]" << std::endl;
        //return 1;
    //}

    std::vector<base::Atom<int>> polytopes;
    std::vector<base::Atom<int>> uniques;

    array::LexicographicOrder<setword*> comp;
    std::set<std::pair<setword*,setword*>,array::LexicographicOrder<setword*>> cgs(comp);
    while ( true ) {

        if ( !twolvl::load(std::cin, polytopes) ) break ;

        auto& polytope = polytopes[0]; // ugly we use a length-1 vector atm

        std::pair<setword*,setword*> pair(polytope.cg, polytope.cg_end);
        if ( cgs.count(pair) == 0 ) {
            twolvl::dump(std::cout, polytope);
            uniques.push_back(polytope);
            cgs.insert(pair);
        }

        else {
            polytope.teardown();
        }

        polytopes.clear(); // ugly hack atm, we use a vector containing a single element

    }

    for (auto& unique : uniques) unique.teardown();
    uniques.clear(); // should only use the setword vector and throw away the rest

    return 0;

}
