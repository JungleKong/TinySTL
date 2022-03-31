#ifndef KTINYSTL_ALGORITHM_H
#define KTINYSTL_ALGORITHM_H

#include <cstring>
#include <utility>

#include "iterator.h"

namespace stl
{
    /**************************************************************************
     * fill:用指定的值填充指定的地址
     * O(n)
     * @param first 地址的起始
     * @param last 地址的结尾（前闭后开）
     * @param value 填充初值
     **************************************************************************/
    template<class ForwordIterator, class T>
    inline void fill(ForwordIterator first, ForwordIterator last, const T& value) {
        for (;  first != last ; ++first) {
            *first = value;  // 采用赋值assignment操作
        }
    }
    // 针对char*和wchar_t*的特化版，追求更快的速度
    inline void fill(char* first, char* last, const char& value) {
        // cstring: 将first开始的位置填充value， 填充长度的last-first
        memset(first, static_cast<unsigned char>(value), last - first);
    }
    // 针对char*和wchar_t*的特化版，追求更快的速度
    inline void fill(wchar_t* first, wchar_t* last, const wchar_t& value) {
        // cstring: 将first开始的位置填充value， 填充长度的last-first
        memset(first, static_cast<unsigned char>(value), last - first);
    }

    /**************************************************************************
     * fill_n: 用指定的值填充开始地址后n个位置的空间，包括两个偏特化版本
     * O(n)
     * @param first 地址的起始
     * @param n 填充长度
     * @param value 填充初值
     **************************************************************************/
    template<class ForwardIterator, class Size, class T>
    inline ForwardIterator fill_n(ForwardIterator first, Size n, const T& value)
    {
        for (; n > 0; --n, ++first)
            *first = value;
        return first;
    }
    template<class Size>
    inline char *fill_n(char *first, Size n, const char& value)
    {
        memset(first, static_cast<unsigned char>(value), n);
        return first + n;
    }
    template<class Size>
    inline wchar_t *fill_n(wchar_t *first, Size n, const wchar_t& value)
    {
        memset(first, static_cast<unsigned char>(value), n * sizeof(wchar_t));
        return first + n;
    }

    /**************************************************************************
     * copy: 将区间[first, last)中的元素复制到[result, result+(last-first))中
     * 标准库采用各种技巧无所不用其极的提高效率，这个实现比较复杂，copy_backward()就直接搞个简化版的了
     * O(n)
     * @param first 输入端的起始位置
     * @param last 输入端的结束位置
     * @param result 输出端的起始位置
     * @return result+(last-first)
     ***************************************************************************/
    // input_iterator 以迭代器是否等同判断循环是否继续进行，速度慢
    template<class InputIterator, class OutputIterator>
    inline OutputIterator _copy(InputIterator first, InputIterator last, OutputIterator result, stl::input_iterator_tag) {
        for(; first != last; ++first, ++result)
            *result = *first;
        return result;
    }
    // random_access_iterator 以n来决定循环的次数，速度快
    template<class InputIterator, class OutputIterator>
    inline OutputIterator _copy(InputIterator first, InputIterator last, OutputIterator result, stl::random_access_iterator_tag) {
        // _copy_d(first, last, result, stl::difference_type(first));  // 就不抽取成一个函数了
        for (auto n = last - first; n > 0; --n, ++first, ++result)
            *result = *first;
        return result;
    }

    template<class T>
    inline T* _copy_t(const T* first, const T* last, T* result, _true_type) {
        memmove(result, first, sizeof(T) * (last - first));
        return result + (last - first);
    }
    template<class T>
    inline T* _copy_t(const T* first, const T* last, T* result, _false_type) {
        for (auto n = last - first; n > 0; --n, ++first, ++result)
            *result = *first;
        return result;
    }

    template<class InputIterator, class OutputIterator>
    inline OutputIterator _copy_dispatch(InputIterator first, InputIterator last, OutputIterator result) {
        return _copy(first, last, result, stl::iterator_category(first));
    }

    template<class T>
    inline T* _copy_dispatch(T* first, T* last, T* result) {
        typedef typename stl::_type_traits<T>::has_trivial_assignment_operator t;
        return _copy_t(first, last, result, t());
    }
    template<class T>
    inline T* _copy_dispatch(const T* first, const T* last, T* result) {
        typedef typename stl::_type_traits<T>::has_trivial_assignment_operator t;
        return _copy_t(first, last, result, t());
    }

    template<class InputIterator, class OutputIterator>
    OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
        return _copy_dispatch(first, last, result);
    }

    // 两个特化版本char* 和 wchar_t* 感觉这两个特化和const T* 版本之一重复了
    char* copy(const char* first, const char* last, char* result) {
        std::memmove(result, first, last - first);
        return result + (last - first);
    }
    wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
        std::size_t n = sizeof(wchar_t) * (last - first);
        if (n > 0)
            std::memmove(result, first, n);
        return result + n;
    }

    /**************************************************************************************
     * copy_backward:将[first, last)中的元素，逆行的方式复制到[result-(last-first))内
     * O(n). 简化版本
     * @param first 输入端的起始 正向起始
     * @param last 输入端的结尾 正向结尾
     * @param result 正向结尾，逆向起始
     * @return result-(last-first)
     **************************************************************************************/
    // input_iterator 以迭代器是否等同判断循环是否继续进行，速度慢
    template<class InputIterator, class OutputIterator>
    inline OutputIterator _copy_backward(InputIterator first, InputIterator last, OutputIterator result, stl::bidirectional_iterator_tag) {
        while(first != last)
            *(--result) = *(--last);
        return result;
    }
    // random_access_iterator 以n来决定循环的次数，速度快
    template<class InputIterator, class OutputIterator>
    inline OutputIterator _copy_backward(InputIterator first, InputIterator last, OutputIterator result, stl::random_access_iterator_tag) {
        // _copy_d(first, last, result, stl::difference_type(first));  // 就不抽取成一个函数了
        for (auto n = last - first; n > 0; --n)
            *(--result) = *(--last);
        return result;
    }

    template<class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2 copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result) {
        return _copy_backward(first, last, result, stl::iterator_category(first));
    }
    // 两个特化版本char* 和 wchar_t*
    char* copy_backward(const char* first, const char* last, char* result) {
        std::memmove(result, first, last - first);  // memmove若地址重叠会从后向前复制， memcpy则不会
        return result - (last - first);
    }
    wchar_t* copy_backward(const wchar_t* first, const wchar_t* last, wchar_t* result) {
        std::size_t n = last - first;
        if (n > 0)
            std::memmove(result, first, n * sizeof(wchar_t ));
        return result - n;
    }

    template<class T>
    inline void swap(T& a, T& b) {
        T temp = a;
        a = b;
        b = temp;
    }


    /*********************************************************************************************
     * 二分查找的一个版本，在不改变排序状态的情况下，返回可插入值的第一个位置
     * @tparam ForwardIterator
     * @tparam T
     * @param first
     * @param last
     * @param value
     * @return
     ********************************************************************************************/
    template<class ForwarfIterator, class T, class Distance>
    inline ForwarfIterator _lower_bound(ForwarfIterator first, ForwarfIterator last, const T& value,
                                        Distance*, forward_iterator_tag) {
        Distance len = 0;
        distance(first, last, len);  // 获取区间长度
        Distance half;
        ForwarfIterator middle;

        while (len > 0) {
            half = len >> 1;  // 除以2
            middle = first;
            advance(middle, half);  // 令middle指向中间位置
            if (*middle < value) {
                first = middle;
                ++first;
                len = len - half - 1;
            } else
                len = half;
        }
        return first;
    }

    template<class RandomAccessIterator, class T, class Distance>
    inline RandomAccessIterator _lower_bound(RandomAccessIterator first, RandomAccessIterator last, const T& value,
                                        Distance*, random_access_iterator_tag) {
        Distance len = last - first;
        Distance half;
        RandomAccessIterator middle;

        while (len > 0) {
            half = len >> 1;
            middle = first + half;
            if (*middle < value) {
                first = middle + 1;
                len = len - half - 1;
            } else
                len = half;
        }
        return first;
    }

    template <class ForwardIterator, class T>
    inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value) {
        return _lower_bound(first, last, value, difference_type(first), iterator_category(first));
    }
}


#endif //KTINYSTL_ALGORITHM_H
