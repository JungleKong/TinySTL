#ifndef KTINYSTL_UNINITIALIZED_H
#define KTINYSTL_UNINITIALIZED_H

#include <new>
#include <cstring>

#include "type_traits.h"
#include "iterator.h"
#include "algorithm.h"


namespace stl
{

    /************************************************************************
     * 五个全局函数之一，负责对象的构造
     * @tparam T
     * @param ptr
     */
    template<class T>
    inline void construct(T* ptr) {
        new(ptr) T();
    }

    template<class T1, class T2>
    inline void construct(T1 *ptr1, const T2& value){
        new(ptr1) T1(value);
    }

    /************************************************************************
     * 五个全局函数之一，负责对象的析构
     * @tparam T
     * @param ptr
     */
    template<class T>
    inline void destroy(T *ptr){
        ptr->~T();
    }

    template<class ForwardIterator>
    inline void _destroy(ForwardIterator first, ForwardIterator last, _true_type){
//        for (; first != last; ++first){
//            destroy(&*first);
//        }
    }

    template<class ForwardIterator>
    inline void _destroy(ForwardIterator first, ForwardIterator last, _false_type){
        for (; first != last; ++first){
            destroy(&*first);
        }
    }

    template<class ForwardIterator>
    inline void destroy(ForwardIterator first, ForwardIterator last){
        typedef typename _type_traits<ForwardIterator>::is_POD_type is_POD_type;
        _destroy(first, last, is_POD_type());
    }


    /*******************************************************************************/
    template<class InputIterator, class ForwardIterator>
    ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _true_type){
        //memcpy(result, first, (last - first) * sizeof(*first));  // cstring, 内存拷贝函数，从源内存地址的起始位置开始拷贝若干个字节到目标内存地址中
        stl::copy(first, last, result);  // STL algorithm
        return result + (last - first);
    }
    template<class InputIterator, class ForwardIterator>
    ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _false_type){
        int i = 0;
        for (; first != last; ++first, ++i){
            construct((result + i), *first);
        }
        return (result + i);
    }
    /**
     * 五个全局函数之一，对应于copy(),将first和last之间的对象复制一份.
     * 对内建数据类型进行性能优化，即不适用构造函数，而直接用最有效的手法
     * @tparam InputIterator
     * @tparam ForwardIterator
     * @param first 输入端的起始位置
     * @param last 输入端的结束位置 前闭后开
     * @param result 输出端的起始位置
     * @return
     */
    template<class InputIterator, class ForwardIterator>
    ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result){
        // 萃取出迭代器的value_type信息。判断是不是内建类型
        typedef typename _type_traits<typename iterator_traits<InputIterator>::value_type>::is_POD_type isPODType;
        return _uninitialized_copy_aux(first, last, result, isPODType());
    }


    /***************************************************************************/
    template<class ForwardIterator, class T>
    void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, _true_type){
        stl::fill(first, last, value);  // STL algorithm
    }
    template<class ForwardIterator, class T>
    void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, _false_type){
        for(; first != last; ++first){
            construct(first, value);
        }
    }
    /**
     * 五个全局函数之一，
     * @tparam ForwardIterator
     * @tparam T
     * @param first 指向输出端（初始化空间）的起始位置
     * @param last 指向输出端（初始化空间）的结束位置：前闭后开
     * @param value 初始值
     */
    template<class ForwardIterator, class T>
    void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value){
        typedef typename _type_traits<T>::is_POD_type isPODType;
        _uninitialized_fill_aux(first, last, value, isPODType());
    }

    /***************************************************************************/
    template<class ForwardIterator, class Size, class T>
    ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& value, _true_type){
        return stl::fill_n(first, n, value);  // STL algorithm
    }
    template<class ForwardIterator, class Size, class T>
    ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& value, _false_type){
        int i = 0;
        for (; i != n; ++i){
            construct((T*)(first + i), value);
        }
        return (first + i);
    }
    /**
     * 五个全局函数之一
     * @tparam ForwardIterator
     * @tparam Size
     * @tparam T
     * @param first y欲初始化空间的起始处
     * @param n 欲初始化空间的大小
     * @param value 初始值
     * @return
     */
    template<class ForwardIterator, class Size, class T>
    inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& value){
        typedef typename _type_traits<T>::is_POD_type isPODType;
        return _uninitialized_fill_n_aux(first, n, value, isPODType());
    }



}


#endif //KTINYSTL_UNINITIALIZED_H
