#include <iostream>
#include <vector>
#include <set>
#include <utility>

#include "tl/loadall.hpp"
#include "tl/load.hpp"
#include "tl/dump.hpp"
#include "tl/Polytope.hpp"
#include "tl/CanonicalGraph.hpp"
#include "array/LexicographicOrder.hpp"

int main () {

    std::ios_base::sync_with_stdio(false);

    //if (argc < 1 || argc > 2) {
        //std::cerr << "usage: dedup [<verbosity>]" << std::endl;
        //return 1;
    //}

    std::vector<tl::Polytope<int>> polytopes;
    std::vector<tl::CanonicalGraph<int>> graphs;

    array::LexicographicOrder<setword*> comp;
    std::set<std::pair<setword*,setword*>,array::LexicographicOrder<setword*>> cgs(comp);
    while ( true ) {

        if ( !tl::load(std::cin, polytopes) ) break ;

        auto& polytope = polytopes[0]; // ugly we use a length-1 vector atm

        tl::CanonicalGraph<int> graph(polytope);

        std::pair<setword*,setword*> pair(graph.begin, graph.end);
        if ( cgs.count(pair) == 0 ) {
            tl::dump(std::cout, polytope);
            graphs.push_back(graph);
            cgs.insert(pair);
        }

        else {
            graph.teardown();
        }

        polytope.teardown();
        polytopes.clear(); // ugly hack atm, we use a vector containing a single element

    }

    for (auto& graph : graphs) graph.teardown();
    graphs.clear();
    cgs.clear();

    return 0;

}
