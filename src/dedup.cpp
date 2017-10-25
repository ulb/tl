#include <iostream>
#include <vector>
#include <set>
#include <utility>

#include "tl/loadall.hpp"
#include "tl/load.hpp"
#include "tl/dump.hpp"
#include "tl/Polytope.hpp"
#include "array/LexicographicOrder.hpp"

int main () {

    std::ios_base::sync_with_stdio(true);

    //if (argc < 1 || argc > 2) {
        //std::cerr << "usage: dedup [<verbosity>]" << std::endl;
        //return 1;
    //}

    std::vector<tl::Polytope<int>> polytopes;
    std::vector<tl::Polytope<int>> uniques;

    array::LexicographicOrder<setword*> comp;
    std::set<std::pair<setword*,setword*>,array::LexicographicOrder<setword*>> cgs(comp);
    while ( true ) {

        if ( !tl::load(std::cin, polytopes) ) break ;

        auto& polytope = polytopes[0]; // ugly we use a length-1 vector atm

        std::pair<setword*,setword*> pair(polytope.cg, polytope.cg_end);
        if ( cgs.count(pair) == 0 ) {
            tl::dump(std::cout, polytope);
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
