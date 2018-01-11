#include <iostream>
#include <vector>

#include "tl/load.hpp"
#include "tl/Polytope.hpp"
#include "subcl/is_cs.hpp"

int main () {

    std::ios_base::sync_with_stdio(true);

    std::vector<tl::Polytope<int>> polytopes;
    size_t n_cs = 0 ;
    while ( true ) {

        if ( !tl::load(std::cin, polytopes) ) break ;

        auto& polytope = polytopes[0];

        if (subcl::is_cs(polytope)) ++n_cs;

        for (auto& polytope : polytopes) polytope.teardown();
    	polytopes.clear();
    }

    std::cout << n_cs << std::endl ;

    return 0;
}