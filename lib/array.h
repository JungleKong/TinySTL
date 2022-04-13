#ifndef KTINYSTL_ARRAY_H
#define KTINYSTL_ARRAY_H

#include <cstddef>
#include <stdexcept>
#include <iostream>

#include "stlexcept.h"

namespace stl
{
    template<class T, size_t num>
    class array {
    public:
        typedef T               value_type;
        typedef T*              pointer;
        typedef const T*        const_pointer;
        typedef T&              reference;
        typedef const T&        const_reference;
        typedef size_t          size_type;
        typedef pointer         iterator;  // pure pointer iterator
        typedef const pointer   const_iterator;

        value_type arr[num ? num : 1];  // avoid num=0

//        iterator begin() const {
//            return iterator(&arr[0]);
//        }

//        iterator end() const {
//            return iterator(&arr[num]);
//        }

//        reference operator[](size_t n) {
//            return *(begin() + n);
//        }

        // element access
        reference at(size_type pos);  // access specified element with bounds checking
        constexpr const_reference at(size_type pos) const noexcept;

        reference operator[](size_type pos);  // Returns a reference to the element at specified location pos
        constexpr const_reference operator[](size_type pos) const noexcept;

        reference front();  // Returns a reference to the first element in the container
        constexpr const_reference front() const noexcept;

        reference back();  // Returns a reference to the first element in the container
        constexpr const_reference back() const noexcept;

        pointer data() noexcept;  //Returns pointer to the underlying array serving as element storage
        constexpr const_pointer data() const noexcept;

        // Iterators
        iterator begin();  // Returns an iterator to the first element of the array
        constexpr const_iterator begin() const noexcept;
        constexpr const_iterator cbegin() const noexcept;

        iterator end();  // Returns an iterator to the last element of the array
        constexpr const_iterator end() const noexcept;
        constexpr const_iterator cend() const noexcept;

        // not implement the reverse iterator now
        // constexpr iterator rbegin() noexcept;  // Returns a reverse iterator to the first element of the array
        // constexpr const_iterator rbegin() const noexcept;
        // constexpr const_iterator crbegin() const noexcept;

        // constexpr iterator rend() noexcept;  // Returns a reverse iterator to the last element of the array
        // constexpr const_iterator rend() const noexcept;
        // constexpr const_iterator crend() const noexcept;


        // capacity
        constexpr bool empty() const noexcept;  // checks whether the container is empty
        constexpr size_type size() const noexcept;  // returns the number of elements
        constexpr size_type max_size() const noexcept;  // returns the maximum possible number of elements

        // operations
        void fill(const value_type& value);
        void swap( array& other ) noexcept;


    };

    template<class T, size_t num>
    T& array<T, num>::at(size_t pos) {
        // THROW_OUT_OF_RANGE_IF(pos > size(), "pos out of range");
        if (pos >= size())
            throw std::out_of_range("pos out of range: %d > %d");  // 格式化字符串像python一样简单就好了
        return arr[pos];
    }

    template<class T, size_t num>
    constexpr const T& array<T, num>::at(const size_t pos) const noexcept{
        if (pos > size())
            throw std::out_of_range("pos out of range");
        return arr[pos];
    }

    template<class T, size_t num>
    constexpr bool array<T, num>::empty() const noexcept {
        return false;
    }

    template<class T, size_t num>
    constexpr size_t array<T, num>::size() const noexcept {
        return num;
    }

    template<class T, size_t num>
    constexpr size_t array<T, num>::max_size() const noexcept {
        return num;
    }

    template<class T, size_t num>
    T& array<T, num>::operator[](size_t pos) {
        return *(begin() + pos);
    }

    template<class T, size_t num>
    constexpr const T& array<T, num>::operator[](size_t pos) const noexcept {
        return *(begin() + pos);;
    }

    template<class T, size_t num>
    T& array<T, num>::front() {
        return *begin();
    }

    template<class T, size_t num>
    constexpr const T& array<T, num>::front() const noexcept {
        return arr[0];
    }

    template<class T, size_t num>
    T& array<T, num>::back() {
        return num ? arr[num - 1] : arr[0];
    }

    template<class T, size_t num>
    constexpr const T& array<T, num>::back() const noexcept {
        return num ? arr[num - 1] : arr[0];
    }

    template<class T, size_t num>
    T* array<T, num>::data() noexcept {
        return arr;
    }

    template<class T, size_t num>
    constexpr const T* array<T, num>::data() const noexcept {
        return arr;
    }

    template<class T, size_t num>
    typename array<T, num>::iterator
    array<T, num>::begin() {
        return iterator(&arr[0]);
    }

    template<class T, size_t num>
    constexpr typename array<T, num>::const_iterator
    array<T, num>::begin() const noexcept {
        return iterator(&arr[0]);
    }

    template<class T, size_t num>
    constexpr typename array<T, num>::const_iterator
    array<T, num>::cbegin() const noexcept {
        return iterator(&arr[0]);
    }

    template<class T, size_t num>
    typename array<T, num>::iterator
    array<T, num>::end() {
        return iterator(&arr[0] + num);
    }

    template<class T, size_t num>
    constexpr typename array<T, num>::const_iterator
    array<T, num>::end() const noexcept {
        return const_iterator(&arr[0] + num);
    }

    template<class T, size_t num>
    constexpr typename array<T, num>::const_iterator
    array<T, num>::cend() const noexcept {
        return const_iterator(&arr[0] + num);
    }

    template<class T, size_t num>
    void array<T, num>::fill(const value_type &value) {
        for(int i = 0; i < num; i++)
            *(begin() + i) = value;
    }

    template<class T, size_t num>
    void array<T, num>::swap(array &other) noexcept {
        for (int i = 0; i < num; i++) {
            T temp = *(begin() + i);
            *(begin() + i) = other[i];
            other[i] = temp;
        }
    }

}



#endif //KTINYSTL_ARRAY_H
