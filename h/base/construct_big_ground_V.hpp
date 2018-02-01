#ifndef H_BASE_CONSTRUCT_BIG_GROUND_V
#define H_BASE_CONSTRUCT_BIG_GROUND_V

#include <cstring>

#include "mem/alloc.hpp"

namespace base {

    template <typename T>
    void construct_ground_i(T ** ground_V,const T D,T * count,const T N,T & k) {
        bool carry;
        T i, j;
        std::memset(count,0,D * sizeof(T));
        while (count[1] == 0) {
            ground_V[k][0] = 1;
            ground_V[k][1] = N;
            for (j = 2; j < D; ++j) ground_V[k][j] = count[j] - 1;
            ++k;
            i = D-1;
            do {
                carry = (count[i] == 2);
                count[i] = (count[i] + 1) % 3;
                --i;
            } while (carry && i >= 1);
        }
    }

    template <typename T>
    void construct_big_ground_V(T ** ground_V, const T D) {
        T * count;
        mem::alloc(count,D);
        int k = 0;
        construct_ground_i(ground_V,D,count,0,k);
        construct_ground_i(ground_V,D,count,1,k);
        free(count);
    }

    template<typename T, typename SIZE>
    T ** rec_ground_3(T ** begin, const SIZE d, const SIZE j) {
        if (j == 0) return begin+1;

        auto end = rec_ground_3(begin, d, j-1);
        while (begin != end) *((*begin++)+d-j) = -1;
        end = rec_ground_3(begin, d, j-1);
        while (begin != end) *((*begin++)+d-j) = 0;
        end = rec_ground_3(begin, d, j-1);
        while (begin != end) *((*begin++)+d-j) = 1;

        return end;

    }

    template<typename T, typename SIZE>
    T ** rec_ground_2(T ** begin, const SIZE d, const SIZE j) {
        if (j == 0) return begin+1;

        auto end = rec_ground_3(begin, d, j-1);
        while (begin != end) *((*begin++)+d-j) = 0;
        end = rec_ground_3(begin, d, j-1);
        while (begin != end) *((*begin++)+d-j) = 1;

        return end;

    }

    template<typename T, typename SIZE>
    T ** rec_ground(T ** begin, const SIZE d, const SIZE j) {
        if (j == 0) return begin+1;

        auto end = rec_ground_2(begin, d, j-1);
        while (begin != end) *((*begin++)+d-j) = 1;

        return end;

    }

    template <typename T, typename SIZE>
    void construct_big_ground_V_rec(T ** ground_V, const SIZE d) {
        rec_ground(ground_V, d, d);
    }

}

#endif // H_BASE_CONSTRUCT_BIG_GROUND_V
