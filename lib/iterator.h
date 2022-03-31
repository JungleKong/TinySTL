#ifndef KTINYSTL_ITERATOR_H
#define KTINYSTL_ITERATOR_H

#include <cstddef>

namespace stl
{
    // 五种迭代器类型
    struct input_iterator_tag{};
    struct output_iterator_tag{};
    struct forward_iterator_tag: input_iterator_tag{};
    struct bidirectional_iterator_tag: forward_iterator_tag{};
    struct random_access_iterator_tag: bidirectional_iterator_tag{};

    // iterator模板
    template <class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
    struct iterator {
        typedef Category    iterator_category;
        typedef T           value_type;
        typedef Pointer     pointer;
        typedef Reference   reference;
        typedef Distance    difference_type;
    };

    // 萃取迭代器泛化
    template <class T>
    struct iterator_traits {
        typedef typename T::iterator_category   iterator_category;
        typedef typename T::value_type          value_type;
        typedef typename T::pointer             pointer;
        typedef typename T::reference           reference;
        typedef typename T::difference_type     difference_type;
    };

    // 针对指针的萃取迭代器偏特化版
    template <class T>
    struct iterator_traits<T*> {
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef T*                          pointer;
        typedef T&                          reference;
        typedef ptrdiff_t                   difference_type;
    };

    // 针对常量指针的萃取迭代器偏特化版
    template <class T>
    struct iterator_traits<const T*> {
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef const T*                    pointer;
        typedef const T&                    reference;
        typedef ptrdiff_t                   difference_type;
    };

    // 萃取某个迭代器的category
    template<class iterator>
    inline typename iterator_traits<iterator>::iterator_category
    iterator_category(const iterator& it) {
        typedef typename iterator_traits<iterator>::iterator_category Category;
        return Category();
    }

    // 萃取某个迭代器的value_type*
    template<class iterator>
    inline typename iterator_traits<iterator>::value_type*
    value_type(const iterator& it) {
        return static_cast<typename iterator_traits<iterator>::value_type*>(0);
    }

    // 萃取某个迭代器的distance*
    template<class iterator>
    inline typename iterator_traits<iterator>::difference_type*
    difference_type(const iterator& it) {
        return static_cast<typename iterator_traits<iterator>::difference_type*>(0);
    }
    template<class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    __distance(InputIterator first, InputIterator last, input_iterator_tag){
        typename iterator_traits<InputIterator>::difference_type n = 0;
        while (first != last) {
            ++first;
            ++n;
        }
        return n;
    }
    template<class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    __distance(InputIterator first, InputIterator last, random_access_iterator_tag){
        return last - first;
    }

    template<class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    distance(InputIterator first, InputIterator last){
        typedef typename iterator_traits<InputIterator>::iterator_category category;
        return __distance(first, last, category());
    }


    template<class InputIterator, class Distance>
    inline void __advance(InputIterator& i, Distance n, input_iterator_tag) {
        while (n--)
            ++i;
    }
    template<class InputIterator, class Distance>
    inline void __advance(InputIterator& i, Distance n, bidirectional_iterator_tag) {
        if (n >= 0)
            while (n--) ++i;
        else
            while (n++) --i;

    }
    template<class InputIterator, class Distance>
    inline void __advance(InputIterator& i, Distance n, random_access_iterator_tag) {
        i += n;
    }

    template<class InputIterator, class Distance>
    inline void advance(InputIterator& i, Distance n) {
        __advance(i, n, iterator_category(i));
    }
}

#endif //KTINYSTL_ITERATOR_H
