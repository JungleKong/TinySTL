#ifndef KTINYSTL_TEST_RB_TREE_H
#define KTINYSTL_TEST_RB_TREE_H

#include <iostream>
#include <utility>
#include <functional>
#include <set>

#include "lib/rb_tree.h"

namespace stl
{
    void test_rb_tree1(){
        rb_tree<int, int, std::_Identity<int>, std::less<>> rb_tre;
        std::cout << rb_tre.size() << std::endl;
        rb_tre.insert_unique(10);
        rb_tre.insert_unique(7);
        rb_tre.insert_unique(8);
        rb_tre.insert_unique(15);
        rb_tre.insert_unique(5);
        rb_tre.insert_unique(6);
        rb_tre.insert_unique(11);
        rb_tre.insert_unique(13);
        rb_tre.insert_unique(12);
        std::cout << rb_tre.size() << std::endl;

        stl::rb_tree<int, int, std::_Identity<int>, std::less<>>::iterator iter1 = rb_tre.begin();
        stl::rb_tree<int, int, std::_Identity<int>, std::less<>>::iterator iter2 = rb_tre.end();
        for (; iter1 != iter2; ++iter1) {
            std::cout << *iter1 << '\t';
        }
        std::cout << std::endl;
        auto temp = rb_tre.begin();
        for (int i = 0; i < 3; ++i)
            temp++;
        rb_tre.erase(temp);

        iter1 = rb_tre.begin();
        iter2 = rb_tre.end();
        for (; iter1 != iter2; ++iter1) {
            std::cout << *iter1 << '\t';
        }
        std::cout << std::endl;
    }
    void test_rb_tree2() {
        rb_tree<int, int, std::_Identity<int>, std::less<>> rb_tre;
        std::cout << rb_tre.size() << std::endl;
        rb_tre.insert_equal(10);
        rb_tre.insert_equal(7);
        rb_tre.insert_equal(8);
        rb_tre.insert_equal(15);
        rb_tre.insert_equal(5);
        rb_tre.insert_equal(8);
        rb_tre.insert_equal(11);
        rb_tre.insert_equal(13);
        rb_tre.insert_equal(8);

        std::cout << rb_tre.size() << std::endl;

        rb_tree<int, int, std::_Identity<int>, std::less<>>::iterator iter1 = rb_tre.begin();
        rb_tree<int, int, std::_Identity<int>, std::less<>>::iterator iter2 = rb_tre.end();
        for (; iter1 != iter2; ++iter1) {
            std::cout << *iter1 << '\t';
        }
        std::cout << std::endl;

        rb_tre.erase(11);
        std::cout << rb_tre.size() << std::endl;


        for (iter1 = rb_tre.begin(); iter1 != rb_tre.end(); ++iter1) {
            std::cout << *iter1 << '\t';
        }
        std::cout << std::endl;

    }

    void test_rb_tree3() {
        rb_tree<int, int, std::_Identity<int>, std::less<>> rb_tre;
        std::cout << rb_tre.size() << std::endl;
        rb_tre.insert_equal(10);
        rb_tre.insert_equal(7);
        rb_tre.insert_equal(8);
        rb_tre.insert_equal(15);
        rb_tre.insert_equal(5);
        rb_tre.insert_equal(8);
        rb_tre.insert_equal(11);
        rb_tre.insert_equal(13);
        rb_tre.insert_equal(8);

        std::cout << rb_tre.count(8) << std::endl;
    }


    void test_rb_tree() {
        test_rb_tree3();
    }
}



#endif //KTINYSTL_TEST_RB_TREE_H
