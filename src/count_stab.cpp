#include <iostream>
#include <vector>

#include "tl/load.hpp"
#include "tl/Polytope.hpp"
#include "subcl/is_stab.hpp"

int main () {

    std::ios_base::sync_with_stdio(true);

    std::vector<tl::Polytope<int>> polytopes;
    size_t n_stab = 0 ;
    while ( true ) {

        if ( !tl::load(std::cin, polytopes) ) break ;

        auto& polytope = polytopes[0];

        if (subcl::is_stab(polytope)) ++n_stab;

        for (auto& polytope : polytopes) polytope.teardown();
        polytopes.clear();
    }

    std::cout << n_stab << std::endl ;

    return 0;
}