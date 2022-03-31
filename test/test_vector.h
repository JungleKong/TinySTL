#ifndef KTINYSTL_TEST_VECTOR_H
#define KTINYSTL_TEST_VECTOR_H

#include <vector>
#include <algorithm>
#include <ctime>
#include <string>

#include "../lib/vector.h"
#include "test_utils.h"

namespace stl
{
    struct person {
        int age;
        std::string name;
    };

    void test_begin_at() {
//        std::vector<int> vec_std(8, 8);
        stl::vector<int> vec_stl(8, 8);
        stl::vector<int> vec_stl1(vec_stl);
//        std::vector<int> vec_std1 = vec_std;
        for (auto& a: vec_stl1)
            std::cout << a << '\t';
    }

    void test_time() {
        const size_t SIZE = 1000000;
        time_t start = clock();
        std::vector<person> vec_std(SIZE, {10, "kk"});
        std::cout << "std:" << clock() - start << '\t';
        start = clock();
        std::vector<person> vec_stl(SIZE, {10, "kk"});
        std::cout << "stl:" << clock() - start << '\t';

    }

    void test_push_back() {
        stl::vector<int> vec_stl;
        for (int i = 0; i < 20; ++i) {
            vec_stl.push_back(i);
            std::cout << vec_stl.size() << '\t';
            std::cout << vec_stl.capacity() << '\n';
        }
        for (auto a: vec_stl)
            std::cout << a << '\t';
    }
    void test_insert() {
        stl::vector<int> vec_stl;
        stl::vector<int> vec_stl2(8, 8);
        vec_stl.push_back(1);
        vec_stl.push_back(2);
        vec_stl.push_back(3);
        vec_stl.push_back(4);
        vec_stl.push_back(5);
        // vec_stl.push_back(6);
        vec_stl.insert(vec_stl.begin() + 3, vec_stl2.begin() + 2, vec_stl2.begin() + 6);
        vec_stl.erase(vec_stl.begin());
        for (auto a: vec_stl)
            std::cout << a << '\t';
    }

    void test_vector() {
        // test_begin_at();
        // test_time();
        // test_push_back();
        test_insert();
    }
}

#endif //KTINYSTL_TEST_VECTOR_H
