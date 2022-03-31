#ifndef KTINYSTL_TEST_UTILS_H
#define KTINYSTL_TEST_UTILS_H

#include <iostream>
#include <iterator>
#include <string>

namespace stl
{
    namespace test
    {
        template<class Container>
        void print_container(Container& container, const std::string& name = ""){  // 不是每一个容器都有const_iterator
            std::cout << "Container " << name << " :";
            for (auto& val : container){
                std::cout << val << " ";
            }
            std::cout << std::endl;
        }

        template<class Container1, class Container2>
        bool container_equal(const Container1& con1, const Container2& con2){  // 不是每一个容器都有const_iterator
            auto first1 = std::begin(con1), last1 = std::end(con1);
            auto first2 = std::begin(con2), last2 = std::end(con2);
            for (; first1 != last1 && first2 != last2; ++first1, ++first2){
//                std::cout << "*first1:" << *first1 << "*first2:" << *first2 << '\n';
                if (*first1 != *first2)
                    return false;
            }
            return (first1 == last1 && first2 == last2);
        }

    }
}


#endif //KTINYSTL_TEST_UTILS_H
