#ifndef KTINYSTL_TEST_DEQUE_H
#define KTINYSTL_TEST_DEQUE_H

#include <deque>
#include <iostream>

#include "../lib/deque.h"

namespace stl
{
    void test_deque1() {
        std::deque<int> deq_std(20, 4);
        stl::deque<int> deq_stl(20, 4);
        for (auto deq: deq_std)
            std::cout << deq;
        std::cout << '\n';
        for (auto deq: deq_stl)
            std::cout << deq;
        std::cout << deq_stl.back() << '\n';
        for (int i = 0; i < deq_stl.size(); i++)
            std::cout << deq_stl[i];
        std::cout << deq_stl.at(deq_stl.size() - 1) << '\n';
        deq_stl.clear();
        // for (int i : deq_stl)
        //     std::cout << i;
        // std::cout << deq_stl.size() << '\n';
    }

    void test_deque2() {
        stl::deque<int> deq_stl(60, 4);
        std::cout << deq_stl.size() << '\n';
        for (int i = 60; i < 111; i++) {
            deq_stl.push_back(i);
        }
        // deq_stl.push_back(61);
        // deq_stl.push_back(62);
        // deq_stl.push_back(63);
        // deq_stl.push_back(64);
        // deq_stl.push_back(65);
        std::cout << deq_stl.size() << '\n';
        for (auto deq: deq_stl)
            std::cout << deq << '\t';
        std::cout << '\n';
        deq_stl.pop_back();
        for (auto deq: deq_stl)
            std::cout << deq << '\t';
        std::cout << '\n';
        deq_stl.pop_front();
        for (auto deq: deq_stl)
            std::cout << deq << '\t';
        std::cout << '\n';

    }
    void test_deque() {
        test_deque2();
    }
}


#endif //KTINYSTL_TEST_DEQUE_H
