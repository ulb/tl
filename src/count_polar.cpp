#include <iostream>
#include <vector>
#include <set>

#include "tl/load.hpp"
#include "tl/Polytope.hpp"
#include "subcl/inc_polar.hpp"
#include "array/LexicographicOrder.hpp"

int main () {

    std::ios_base::sync_with_stdio(true);

    //if (argc < 1 || argc > 2) {
        //std::cerr << "usage: dedup [<verbosity>]" << std::endl;
        //return 1;
    //}

    std::vector<tl::Polytope<int>> polytopes;
    array::LexicographicOrder<setword*> comp;
    std::set<std::pair<setword*,setword*>,array::LexicographicOrder<setword*>> cgs(comp);
    size_t n_polar = 0 ;
    while ( true ) {

        if ( !tl::load(std::cin, polytopes) ) break ;

        auto& polytope = polytopes[polytopes.size()-1];

        std::pair<setword*,setword*> pair(polytope.cg, polytope.cg_end);
        cgs.insert(pair);

        subcl::inc_polar(cgs, polytope, n_polar);

    }

    std::cout << n_polar << std::endl ;

    for (auto& polytope : polytopes) polytope.teardown();
    polytopes.clear(); // should only use the setword vector and throw away the rest

    return 0;
}