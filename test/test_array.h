#ifndef KTINYSTL_TEST_ARRAY_H
#define KTINYSTL_TEST_ARRAY_H

#include <array>
#include <cassert>

#include "../lib/array.h"
#include "test_utils.h"

namespace stl
{
    template<class Os, class V> Os& operator<<(Os& os, const V& v);

    void test_begin_end_at() {
        std::array<int, 10> arr_std = {5, 2, 1, 5, 7, 8, 4, 3, 9, 10};
        stl::array<int, 10> arr_stl = {5, 2, 1, 5, 7, 8, 4, 3, 9, 10};;
        assert(arr_stl[0] == arr_std[0]);
        assert(arr_stl.at(2) == arr_std.at(2));
        assert(arr_stl.back() == arr_std.back());
        assert(arr_stl.front() == arr_std.front());
        assert(*arr_stl.begin() == *arr_std.begin());
        assert(*(arr_stl.end() - 1) == *(arr_std.end() - 1));
        assert(arr_stl.empty() == arr_std.empty());
        assert(arr_stl.size() == arr_std.size());
        assert(arr_stl.max_size() == arr_std.max_size());
        arr_stl[0] = 3;
        std::cout << arr_stl[0] << std::endl;
    }

//    void test_fill() {
//        constexpr std::size_t xy = 4;
//        using Cell = stl::array<unsigned char, 8>;
//        stl:array<Cell, xy * xy> board;
//        board.fill({ {0xE2, 0x96, 0x84, 0xE2, 0x96, 0x80, 0, 0} }); // "▄▀";
//        int count = 1;
//        for (Cell c: board)
//        {
//            std::cout << c.data() << ((++count % xy) ? "" : "\n");
//        }
//    }


    void test_swap() {
        stl::array<int, 3> a1{1, 2, 3}, a2{4, 5, 6};

        auto it1 = a1.begin();
        auto it2 = a2.begin();
        int& ref1 = a1[1];
        int& ref2 = a2[1];

        std::cout << a1 << a2 << *it1 << ' ' << *it2 << ' ' << ref1 << ' ' << ref2 << '\n';
        a1.swap(a2);
        std::cout << a1 << a2 << *it1 << ' ' << *it2 << ' ' << ref1 << ' ' << ref2 << '\n';
    }

    void test_array() {
//        test_begin_end_at();
//        test_fill();
        test_swap();
    }

    template<class Os, class V> Os& operator<<(Os& os, const V& v) {
        os << "{";
        for (auto i : v) os << ' ' << i;
        return os << " } ";
    }
}












#endif //KTINYSTL_TEST_ARRAY_H
