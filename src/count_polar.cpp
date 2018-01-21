#include <iostream>
#include <vector>
#include <set>

#include "tl/load.hpp"
#include "tl/Polytope.hpp"
#include "tl/CanonicalGraph.hpp"
#include "subcl/inc_polar.hpp"
#include "array/LexicographicOrder.hpp"

int main () {

    std::ios_base::sync_with_stdio(false);

    std::vector<tl::Polytope<int>> polytopes;
    std::vector<tl::CanonicalGraph<int>> graphs;
    array::LexicographicOrder<setword*> comp;
    std::set<std::pair<setword*,setword*>,array::LexicographicOrder<setword*>> cgs(comp);
    size_t n_polar = 0 ;
    while ( true ) {

        if ( !tl::load(std::cin, polytopes) ) break ;

        auto& polytope = polytopes[0];

        graphs.emplace_back(polytope);

        const auto& graph = graphs[graphs.size()-1];

        std::pair<setword*,setword*> pair(graph.begin, graph.end);
        cgs.insert(pair);

        auto dualpolytope = polytope.dual();

        tl::CanonicalGraph<int> dualgraph(dualpolytope);

        n_polar += subcl::inc_polar(cgs, graph, dualgraph);

        polytope.teardown();
        dualpolytope.teardown();
        dualgraph.teardown();
        polytopes.clear();

    }

    std::cout << n_polar << std::endl ;

    for (auto& graph : graphs) graph.teardown();
    graphs.clear();

    return 0;
}
