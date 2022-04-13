#ifndef KTINYSTL_TEST_HASHTABLE_H
#define KTINYSTL_TEST_HASHTABLE_H
#include "../lib/hashtable.h"

#include <iostream>

namespace stl
{
    void test_hashtable1() {
        std::cout << stl::next_prime(399999) << std::endl;
    }

    void test_hashtable2() {
        stl::hashtable<int, int, std::hash<int>, std::_Identity<int>, std::equal_to<int>> ht(10);
        ht.insert_equal(1);
        ht.insert_equal(2);
        ht.insert_equal(3);
        ht.insert_equal(4);
        for (auto val: ht) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
        ht.insert_equal(4);
        for (auto val: ht) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
        // ht.erase(3);
        // for (auto val: ht) {
        //     std::cout << val << " ";
        // }
        // std::cout << std::endl;
        // ht.erase(ht.begin());
        // for (auto val: ht) {
        //     std::cout << val << " ";
        // }
        // std::cout << std::endl;

        ht.erase(ht.begin(), ht.end());
        for (auto val: ht) {
            std::cout << val << " ";
        }
        std::cout << "size: " << ht.size() << std::endl;
    }


    void test_hashtable() {
        test_hashtable2();
    }

}

#endif //KTINYSTL_TEST_HASHTABLE_H
