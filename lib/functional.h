//
// Created by 孔俊宇 on 2022/4/13.
//
// 因为各家的C++ stl实现的不同，有些仿函数并没有实现，例如hash, identity等
// 这里尽量自己实现使用自己的
#ifndef KTINYSTL_FUNCTIONAL_H
#define KTINYSTL_FUNCTIONAL_H

namespace stl{
    // 一切的基础, 单个参数的
    template <class Arg, class Result>
    struct unary_function {
        typedef Arg argument_type;
        typedef Result result_type;
    };

    // 一切的基础，两个参数的
    template<class Arg1, class Arg2, class Result>
    struct binary_function
    {
        typedef Arg1 	first_argument_type;
        typedef Arg2 	second_argument_type;
        typedef Result 	result_type;
    };
}




#endif //KTINYSTL_FUNCTIONAL_H
