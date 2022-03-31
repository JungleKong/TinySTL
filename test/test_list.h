#ifndef KTINYSTL_TEST_LIST_H
#define KTINYSTL_TEST_LIST_H

#include "../lib/list.h"

#include <iostream>
#include <list>

namespace stl
{
    void test_list1() {
        stl::list<int> lst_stl;
        // lst_stl.push_back(1);
        // lst_stl.push_back(2);
        // lst_stl.push_back(3);
        lst_stl.insert(lst_stl.cend(), 4, 3);
        for(auto a: lst_stl) {
            std::cout << a << '\t';
        }
    }

    void test_list2() {
        stl::list<int> lst_stl(5, 5);
        stl::list<int> lst_stl2(lst_stl);
        for(auto a: lst_stl2) {
            std::cout << a << '\t';
        }
    }
    void test_list3() {
        stl::list<int> lst_stl(5, 10);
        lst_stl.resize(10, 8);
        for(auto a: lst_stl) {
            std::cout << a << '\t';
        }
        std::cout << '\n';
        lst_stl.resize(4);
        for(auto a: lst_stl) {
            std::cout << a << '\t';
        }
        std::cout << '\n';
        lst_stl.resize(8);
        for(auto a: lst_stl) {
            std::cout << a << '\t';
        }
        std::cout << '\n';
    }

    void test_list4() {
        stl::list<int> lst_stl1(5, 1);
        for(auto a: lst_stl1) {
            std::cout << a << '\t';
        }
        std::cout << '\n';
        stl::list<int> lst_stl2(5, 2);
        for(auto a: lst_stl2) {
            std::cout << a << '\t';
        }
        std::cout << '\n';
        lst_stl1.swap(lst_stl2);
        for(auto a: lst_stl1) {
            std::cout << a << '\t';
        }
        std::cout << '\n';
        for(auto a: lst_stl2) {
            std::cout << a << '\t';
        }
        std::cout << '\n';
    }

    void test_list5() {
        stl::list<int> lst_stl1 = {1, 2, 4, 5};
        stl::list<int> lst_stl2 = {3, 6, 7};
        // lst_stl1.transfer(lst_stl1.begin(), lst_stl2.begin(), ++lst_stl2.begin());
        lst_stl1.merge(lst_stl2);
        for(auto a: lst_stl1) {
            std::cout << a << '\t';
        }
        std::cout << '\n';
        lst_stl1.reverse();
        for(auto a: lst_stl1) {
            std::cout << a << '\t';
        }
        std::cout << '\n';
        lst_stl1.sort();
        for(auto a: lst_stl1) {
            std::cout << a << '\t';
        }
        std::cout << '\n';
    }

    void test_list() {
        test_list5();
    }
}



#endif //KTINYSTL_TEST_LIST_H
