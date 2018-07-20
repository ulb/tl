#ifndef H_EMB_CONSTRUCT_BIG_GROUND_V
#define H_EMB_CONSTRUCT_BIG_GROUND_V

namespace emb {

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

        auto end = rec_ground_3(begin, d, j-1);
        while (begin != end) *((*begin++)+d-j) = 1;

        return end;

    }

    template <typename T, typename SIZE>
    void construct_big_ground_V(T ** ground_V, const SIZE d) {
        rec_ground(ground_V, d, d);
    }

}

#endif // H_EMB_CONSTRUCT_BIG_GROUND_V
