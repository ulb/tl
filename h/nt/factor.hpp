#ifndef H_NT_FACTOR
#define H_NT_FACTOR

#include <vector>

namespace nt {
	// factor an integer
	template <class T>
	std::vector<T> factor(T n) {
	    std::vector<T> factors;   
	    for (T d = 1; d <= n; ++d) if (n % d == 0) factors.push_back(d);
	    return factors;
	}
}

#endif // H_NT_FACTOR