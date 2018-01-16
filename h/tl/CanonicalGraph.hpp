#ifndef H_TL_CANONICALGRAPH
#define H_TL_CANONICALGRAPH

#include "nauty.h"
#include "alloc.hpp"
#include "tl/canonicize.hpp"
#include "tl/Polytope.hpp"

namespace tl {

	template <typename T>
		class CanonicalGraph {

			public:

				setword* begin;
				setword* end;

				CanonicalGraph(const Polytope<T>& polytope) {

					int n(polytope.rows + polytope.columns);
					int m(SETWORDSNEEDED(n));
					setword* cg;
					alloc(cg, m*n, setword);
					tl::canonicize(polytope.matrix, polytope.rows, polytope.columns, n, m, cg);
					this->begin = cg;
					this->end = cg + m*n;

				}

				void teardown ( ) {
					free(this->begin);
				}

		} ;

}

#endif // H_TL_CANONICALGRAPH
