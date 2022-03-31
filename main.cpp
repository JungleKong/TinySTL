#include <iostream>
#include <vector>
#include <ctime>
#include <list>
#include <algorithm>

void test_allocator();
void test_array();

// #include "test/test_array.h"
// #include "test/test_vector.h"
// #include "test/test_list.h"
// #include "test/test_deque.h"
// #include "test/test_rb_tree.h"
// #include "test/test_set.h"
#include "test/test_hashtable.h"


int main() {
    // stl::test_array();
    // stl::test_vector();
    // stl::test_list();
    // stl::test_deque();
    // stl::test_rb_tree();
    // stl::test_set();
    stl::test_hashtable();
    return 0;

}



#include "lib/array.h"
void test_array() {
    stl::array<int, 10> arr{};
    for (int i = 0; i < 10; i++) {
        arr[i] = -i;
    }
    for (int i = 0; i < 10; i++) {
        std::cout << arr[i] << "\t";
    }
    std::sort(arr.begin(), arr.end());
    for (int i = 0; i < 10; i++) {
        std::cout << arr[i] << "\t";
    }
}


#include "lib/allocator.h"
void test_allocator() {
    const int SIZE = 500000;

    clock_t tic = clock();
    std::vector<int, stl::allocator<int>> vec_stl;
    for (int i = 0; i < SIZE; i++)
        vec_stl.push_back(i);
    std::cout << "stl:" << clock() - tic << std::endl;

    tic = clock();
    std::vector<int, std::allocator<int>> vec_std;
    for (int i = 0; i < SIZE; i++)
        vec_stl.push_back(i);
    std::cout << "std:" << clock() - tic << std::endl;
}
