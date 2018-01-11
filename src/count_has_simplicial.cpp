#include <iostream>
#include <vector>

#include "tl/load.hpp"
#include "tl/Polytope.hpp"
#include "subcl/has_simplicial.hpp"

int main () {

    std::ios_base::sync_with_stdio(true);

    std::vector<tl::Polytope<int>> polytopes;
    size_t n_has_simplicial = 0 ;
    while ( true ) {

        if ( !tl::load(std::cin, polytopes) ) break ;

        auto& polytope = polytopes[0];

        if (subcl::has_simplicial(polytope)) ++n_has_simplicial;

        for (auto& polytope : polytopes) polytope.teardown();
        polytopes.clear();
    }

    std::cout << n_has_simplicial << std::endl ;

    return 0;
}