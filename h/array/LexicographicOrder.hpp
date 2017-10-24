#ifndef H_ARRAY_LEXICOGRAPHICORDER
#define H_ARRAY_LEXICOGRAPHICORDER

#include <utility>

namespace array {

    template<typename I>
    class FixedLexicographicOrder {
    private:
        const int dimension;

    public:

        FixedLexicographicOrder (const int dimension):dimension(dimension) {}

        bool operator() (I a, I b) {
            const int dimension = this->dimension;
            for (int i = 0; i < dimension; ++i) {
                if (a[i] < b[i]) return true;
                if (a[i] > b[i]) return false;
            }
            return false;
        }

    };

    template<typename I>
    class LexicographicOrder {
    public:
        LexicographicOrder () {}

        bool operator() (const std::pair<I,I>& a, const std::pair<I,I>& b) const {

            const size_t m = a.second - a.first ;
            const size_t n = b.second - b.first ;

            if (m < n) return true;
            if (n < m) return false;

            I x = a.first;
            I end = a.second;
            I y = b.first;

            while ( x != end ) {
                if (*x < *y) return true;
                if (*x > *y) return false;
                ++x; ++y;
            }

            return false;

        }

    };

}

#endif // H_ARRAY_LEXICOGRAPHICORDER
