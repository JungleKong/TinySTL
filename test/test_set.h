#ifndef KTINYSTL_TEST_SET_H
#define KTINYSTL_TEST_SET_H

#include <iostream>

#include "../lib/set.h"

namespace stl
{
    void test_set1(){
        stl::set<int> s;
        s.insert(10);
        s.insert(7);
        s.insert(8);
        s.insert(15);
        s.insert(5);
        s.insert(6);
        s.insert(11);
        s.insert(13);
        s.insert(12);

        for (auto v: s) {
            std::cout << v << '\t';
        }
        std::cout << std::endl;

        s.erase(s.begin());
        for (auto v: s) {
            std::cout << v << '\t';
        }
        std::cout << std::endl;
    }
    
    
    void test_set() {
        test_set1();
    }
}


#endif //KTINYSTL_TEST_SET_H
