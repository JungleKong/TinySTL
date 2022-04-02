#ifndef KTINYSTL_VECTOR_H
#define KTINYSTL_VECTOR_H

#include "allocator.h"
#include "global_func.h"

#include <assert.h>

namespace stl
{
    template<class T, class Alloc = stl::allocator<T>>
    class vector {
    public:
        typedef T           type_value;
        typedef T*          pointer;
        typedef const T*    const_pointer;
        typedef T&          reference;
        typedef const T&    const_reference;
        typedef T*          iterator;
        typedef const T*    const_iterator;
        typedef size_t      size_type;
        typedef ptrdiff_t	difference_type;

    private:
        pointer _start;  // 使用空间的头
        pointer _finish;  // 使用空间的尾
        pointer _end_of_storage;  // 可用空间的尾

        typedef Alloc data_allocator;

    public:
        // ctor dtor
        vector():_start(0), _finish(0), _end_of_storage(0){}
        explicit vector(const size_type n) {fill_initialize(n, T()); }
        vector(const size_type n, const_reference value) {fill_initialize(n, value); }
        vector(const int n, const_reference value) {fill_initialize(n, value); }
        vector(const long n, const_reference value) {fill_initialize(n, value); }
        vector(const vector& v) {allocator_and_copy(v._start, v._finish);}
        vector(vector&& v) {}
        vector& operator=(const vector& v) {
            if (this != v)
                allocator_and_copy(v._start, v._finish);
            return *this;
        }
        vector& operator=(vector&& v) {}

        /*****************************************************************************/
        // 与迭代器有关的函数-iterator
        /*****************************************************************************/
        iterator begin() { return _start; }
        const_iterator begin() const {return _start; }
        iterator cbegin() {return _start; }

        iterator end() {return _finish; }
        const_iterator end() const { return _finish; }
        const_iterator cend() const {return _finish; }

        /*****************************************************************************/
        // 访问元素
        /*****************************************************************************/
        reference operator[](size_type n) {assert(n < size()); return *(_start + n); }
        const_reference operator[](size_type n) const {assert(n < size()); return *(_start + n); }
        reference at(size_type n) {assert(n < size()); return *(_start + n); }
        const_reference at(size_type n) const {assert(n < size()); return *(_start + n); }
        reference front() {assert(!empty()); return *_start; }
        const_reference front() const {assert(!empty()); return *_start; }
        reference back() {assert(!empty()); return *(_finish - 1); }
        const_reference back() const {assert(!empty()); return *(_finish - 1); }
        pointer data() {return begin(); }
        const_pointer data() const {return begin(); }


        /*****************************************************************************/
        // 与容量相关-capacity
        /*****************************************************************************/
        bool empty() const { return begin() == end(); }
        size_type size() const { return static_cast<size_type>(end() - begin()); }
        constexpr size_type max_size() const {return std::distance(begin() - end()); }  // 内存最大容量，怎么实现？
        void reserve(size_type new_cap) {
            if (new_cap < capacity()) return;
            iterator new_start = data_allocator::allocate(new_cap);  // 申请空间
            iterator new_finish = stl::uninitialized_copy(_start, _finish, new_start);  // 将内容拷贝过去
            destroy_and_deallocate();  // 释放当前占用的内存

            _start = new_start;
            _finish = new_finish;
            _end_of_storage = _start + new_cap;
        }
        size_type capacity() const { return static_cast<size_type>(_end_of_storage - begin()); }
        void shrink_to_fit() {
            data_allocator::deallocate(_finish, _end_of_storage - _finish);  // 释放没有用的空间
            _end_of_storage = _finish;
            // 也可以采用重新申请内存的方式，不过应该效率不高
            // iterator new_start = static_cast<iterator>(data_allocator::allocate(size()));  // 这个类型转换可要可不要？
            // iterator new_finish = stl::uninitialized_copy(_start, _finish, new_start);
            // destroy_and_deallocate();
            // _start = new_start;
            // _end_of_storage = _finish = new_finish;
        }

        /*****************************************************************************/
        // 与修改相关-modify
        /*****************************************************************************/
        void clear() noexcept {
            stl::destroy(_start, _finish);
            _finish = _start;
        }
        void push_back(const_reference value) {
            if (_finish != _end_of_storage) {
                stl::construct(_finish, value);
                ++_finish;
            } else {
                insert_aux(end(), value);
            }
        }

        // 先使用空返回值的形式, 
        void insert(iterator pos, const_reference value) {insert_aux(pos, 1, value); }
        void insert(iterator pos, size_type n, const_reference value) {insert_aux(pos, n, value); }

        template<class InputIterator>
        void insert(iterator pos, InputIterator first, InputIterator last) {insert_aux(pos, first, last); }


        iterator erase(const_iterator pos) {
            // auto position = begin() + (pos - cbegin());  // 也是去除常量性，作用同const_cast<iterator>(pos)
            if (pos + 1 != end())
                stl::copy(pos + 1, cend(), const_cast<iterator>(pos));
                // stl::copy(pos + 1, end(), const_cast<iterator>(pos));  // 不行, 为什么要传const哦？？？
                // stl::copy(pos + 1, _finish, const_cast<iterator>(pos));  // 不行
            --_finish;
            stl::destroy(_finish);
            return const_cast<iterator>(pos);
        }
        iterator erase(iterator first, iterator last) {
            iterator i = copy(last, _finish, first);
            stl::destroy(i, _finish);
            _finish = _finish - (last - first);
            return first;
        }
        void pop_back() {--_finish; stl::destroy(_finish); }
        void swap(vector<T> vec) {
            stl::swap(_start, vec._start);
            stl::swap(_finish, vec._finish);
            stl::swap(_end_of_storage, vec._end_of_storage);
        }
    private:
        void deallocate() {
            if (_start)
                data_allocator::deallocate(_start, _end_of_storage - _start);
        }

        void fill_initialize(size_type n, const T& value) {
            _start = allocator_and_fill(n, value);
            _finish = _start + n;
            _end_of_storage = _finish;
        }

        iterator allocator_and_fill(size_type n, const T& x) {
            iterator result = data_allocator::allocate(n);
            stl::uninitialized_fill_n(result, n, x);  // global_func.h
            return result;
        }
        void allocator_and_copy(iterator first, iterator last) {
            _start = data_allocator::allocate(last - first);
            _finish = uninitialized_copy(first, last, _start);
            _end_of_storage = _finish;
        }
        void destroy_and_deallocate() {  // 执行析构类似的功能
            stl::destroy(_start, _finish);
            deallocate();
        }
        void insert_aux(iterator position, const_reference value) {
            if (_finish != _end_of_storage) {
                stl::construct(_finish, *(_finish - 1));
                ++_finish;
                type_value value_copy = value;
                copy_backward(position, _finish - 2, _finish - 1);
                *position = value_copy;
            } else {
                const size_type old_size = size();
                const size_type len = old_size != 0 ? 2 * old_size : 1;  // 0则为1，其他为2倍

                iterator new_start = data_allocator::allocate(len);
                iterator new_finish = new_start;  // 元素为空，首等于尾
                try {
                    new_finish = stl::uninitialized_copy(_start, position, new_start);  // 拷贝[start, position)到新vector
                    stl::construct(new_finish, value);
                    ++new_finish;
                    new_finish = stl::uninitialized_copy(position, _finish, new_finish);  // 拷贝[position, finish)到新vector
                } catch (...) {
                    // 有异常回滚操作（rollback）
                    stl::destroy(new_start, new_finish);  // 析构
                    data_allocator::deallocate(new_start, len);  // 释放空间
                    throw;
                }
                // 析构并释放原vector
                destroy_and_deallocate();

                // 调整迭代器
                _start = new_start;
                _finish = new_finish;
                _end_of_storage = _start + len;
            }
        }
        void insert_aux(iterator position, size_type n, const_reference value) {
            if (n != 0) {
                if (static_cast<size_type>(_end_of_storage - _finish) >= n) {  // 剩余空间足够
                    // 感觉这一部分《STL源码剖析》中给的有点麻烦，简化版如下，暂时没有发现bug，也没有给出很完善的测试案例
                    type_value value_copy = value;
                    iterator old_finish = _finish;
                    stl::copy_backward(position, old_finish, old_finish + n);
                    stl::uninitialized_fill_n(position, n, value_copy);
                    _finish += n;
                } else {  // 扩容，递归也可以，不过性能差点
                    const size_type old_size = size();
                    const size_type len = old_size + std::max(old_size, n);
                    iterator new_start = data_allocator::allocate(len);
                    iterator new_finish = new_start;
                    try {
                        new_finish = stl::uninitialized_copy(_start, position, new_start);
                        new_finish = stl::uninitialized_fill_n(new_finish, n, value);
                        new_finish = stl::uninitialized_copy(position, _finish, new_finish);
                    } catch (...) {
                        destroy(new_start, new_finish);
                        data_allocator::deallocate(new_start, len);
                        throw;
                    }
                    destroy_and_deallocate();
                    // 调整迭代器
                    _start = new_start;
                    _finish = new_finish;
                    _end_of_storage = _start + len;
                }
            }
        }

        template<class InputIterator>
        void insert_aux(iterator position, InputIterator first, InputIterator last) {
            if (first != last) {
                difference_type n = last - first;
                if (static_cast<difference_type>(_end_of_storage - _finish) >= n) {
                    iterator old_finish = _finish;
                    stl::copy_backward(position, old_finish, old_finish + n);
                    stl::copy(first, last, position);
                    _finish += n;
                } else {
                    const size_type old_size = size();
                    const size_type len = old_size + std::max(old_size, static_cast<size_type>(n));
                    iterator new_start = data_allocator::allocate(len);
                    iterator new_finish = new_start;
                    try {
                        new_finish = stl::uninitialized_copy(_start, position, new_start);
                        new_finish = stl::copy(first, last, new_finish);
                        new_finish = stl::uninitialized_copy(position, _finish, new_finish);
                    } catch (...) {
                        destroy(new_start, new_finish);
                        data_allocator::deallocate(new_start, len);
                        throw;
                    }
                    destroy_and_deallocate();
                    // 调整迭代器
                    _start = new_start;
                    _finish = new_finish;
                    _end_of_storage = _start + len;
                }
            }
        }

    public:
        ~vector() {
            stl::destroy(_start, _finish);
            deallocate();
        }
    };
}


#endif //KTINYSTL_VECTOR_H
