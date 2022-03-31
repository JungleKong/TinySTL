#ifndef KTINYSTL_DEQUE_H
#define KTINYSTL_DEQUE_H

#include <cstddef>
#include <initializer_list>
#include <algorithm>

#include "iterator.h"
#include "allocator.h"
#include "global_func.h"
namespace stl
{
    inline size_t _deque_buf_size(size_t _size)
    { return (_size < 32
              ? size_t(32 / _size) : size_t(1)); }

    template<class T, class Ref, class Ptr>
    struct _deque_iterator{
        typedef _deque_iterator<T, T&, T*> iterator;
        typedef _deque_iterator<T, const T&, const T*> const_iterator;
        typedef _deque_iterator<T, Ref, Ptr> self;
        static size_t buffer_size() {return _deque_buf_size(sizeof(T)); }

        typedef stl::random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef Ref reference;
        typedef Ptr pointer;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
        typedef T** map_pointer;

        T* cur;
        T* first;
        T* last;
        map_pointer node;

        _deque_iterator()
        :cur(0), first(0), last(0), node(0){}

        explicit _deque_iterator(T* x, map_pointer y)
        :cur(x), first(*y), last(*y + difference_type(buffer_size())), node(y){}

        _deque_iterator(const iterator& x)
        :cur(x.cur), first(x.first), last(x.last), node(x.node){}

        void set_node(map_pointer new_node) {
            node = new_node;
            first = *new_node;
            last = first + difference_type(buffer_size());
        }

        reference operator*() const {return *cur; }
        pointer operator->() const {return &(operator*()); }

        self& operator++() {  // ++i
            ++cur;
            if (cur == last) {  // 到达缓冲区的尾端
                set_node(node + 1);  // node++?
                cur = first;
            }
            return *this;
        }
        self operator++(int) {  // i++
            self temp = *this;
            ++*this;
            return temp;
        }
        self& operator--() {  // --i
            if (cur == first) {
                set_node(node - 1);
                cur = last;
            }
            --cur;
            return *this;
        }
        self operator--(int) {  // i--
            self temp = *this;
            --*this;
            return temp;
        }

        self& operator+=(difference_type n) {
            difference_type offset = n + (cur - first);
            if (offset >= 0 && offset < difference_type(buffer_size()))  // in current buffer
                cur += n;
            else {
                difference_type node_offset = offset > 0 ?
                                              offset / difference_type(buffer_size()) :
                                              -difference_type((-offset - 1) / buffer_size() - 1);  // for -=
                set_node(node + node_offset);
                cur = first + (offset - node_offset * difference_type(buffer_size()));  // 应该可以借助整除操作
            }
            return *this;
        }
        self& operator-=(difference_type n) {return *this += -n; }
        self operator+(difference_type n) const {
            self temp = *this;
            return temp += n;
        }
        self operator-(difference_type n) const {
            self temp = *this;
            return temp -= n;
        }

        difference_type operator-(const self& x) const {
            return difference_type(buffer_size()) * (node - x.node - 1) + (cur - first) + (x.last - x.cur);
        }

        reference operator[](difference_type n) const {return *(*this + n); }

        bool operator==(const self& x) const {return cur == x.cur; }
        bool operator!=(const self& x) const {return !(*this == x); }
        bool operator<(const self& x) const {
            return (node ==x.node) ? (cur < x.cur) : (node < x.node);
        }

    };
    template<class T>
    class deque {
    public:
        typedef T value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        typedef _deque_iterator<T, T&, T*> iterator;
        typedef _deque_iterator<T, const T&, const T*> const_iterator;

        typedef allocator<T> data_allocator;
        typedef allocator<T*> map_allocator;
    private:
        typedef T** map_pointer;
        enum {_initial_map_size = 8 };


    private:
        iterator _start;
        iterator _finish;
        map_pointer _map;
        size_type _map_size;

    public:
        deque() {initialize_map_and_nodes(0); }
        explicit deque(size_type n, const value_type& value);

        template <class InputIterator>
        deque(InputIterator first, InputIterator last);
        deque(const deque& x);
        deque& operator=(const deque& x);

        ~deque();

        // element access
        reference at(size_type pos) {return _start[difference_type(pos)]; }
        const_reference at(size_type pos) const {return _start[difference_type(pos)]; }
        reference operator[](size_type pos) {return _start[difference_type(pos)]; }
        const_reference operator[](size_type pos) const {return _start[difference_type(pos)]; }
        reference front() {return *_start; }
        const_reference front() const {return *_start; }
        reference back() {iterator temp = _finish; temp--; return *temp; }
        // reference back() {return *(_finish - 1); }  random_access_iterator_tag 重写了operate-(int n), 为什么不正确？
        const_reference back() const {iterator temp = _finish; temp--; return *temp; }

        // iterator
        iterator begin() {return _start; }
        const_iterator begin() const {return _start; }
        const_iterator cbegin() const {return _start; }
        iterator end() {return _finish; }
        const_iterator end() const {return _finish; }
        const_iterator cend() const {return _finish; }

        // capacity
        bool empty() const {return _start == _finish; }
        size_type size() const {return _finish - _start; }
        size_type max_size() const {return size_type(-1); }
        void shrink_to_fit();

        // modifier
        void clear();
        iterator insert(const_iterator pos, const T& value);
        iterator insert(const_iterator pos, size_type count, const T& value);

        template<class InputIterator>
        iterator insert(const_iterator pos, InputIterator first, InputIterator last);
        iterator insert(const_iterator pos, std::initializer_list<T> ilist);

        template<class... Args>
        iterator emplace(const_iterator, Args&&... args);

        iterator erase(iterator pos);
        iterator erase(iterator first, iterator last);

        void push_back(const T& value);
        void pop_back();
        void push_front(const T& value);
        void pop_front();
        void resize(size_type count);
        void resize(size_type count, const T& value);
        void swap(deque& other);

    private:
        // 节点构造 配置 析构  map配置，
        pointer allocate_node() {
            return data_allocator::allocate(_deque_buf_size(sizeof(T)));
        }
        void deallocate_node(pointer ptr) {
            data_allocator::deallocate(ptr);
        }
        void create_nodes(map_pointer nstart, map_pointer nfinish) {
            map_pointer cur;
            try {
                for (cur = nstart; cur < nfinish; cur++)
                    *cur = allocate_node();
            } catch (...) {
                destroy_nodes(nstart, cur);
            }
        }
        void destroy_nodes(map_pointer nstart, map_pointer nfinish) {
            for (map_pointer cur = nstart; cur < nfinish; ++cur)
                deallocate_node(*cur);
        }
        map_pointer allocate_map(size_type size) {
            return map_allocator::allocate(size);
        }
        void deallocate_map(map_pointer map, size_type size) {
            map_allocator::deallocate(map, size);
        }

        // 初始化数据结构
        void initialize_map_and_nodes(size_type num_elements) {
            // 需要节点数，刚好整除会多配置一个
            size_type node_size = _deque_buf_size(sizeof(T));
            const size_type num_nodes = num_elements / _deque_buf_size(sizeof(T)) + 1;
            // 一个map管理节点数，最少8个，前后会各留一个供扩充使用
            _map_size = std::max(static_cast<size_type>(_initial_map_size), num_nodes + 2);
            _map = allocate_map(_map_size);

            // 指向中央
            map_pointer nstart = _map + (_map_size - num_nodes) / 2;
            map_pointer nfinish = nstart + num_nodes;
            try {
                create_nodes(nstart, nfinish);
            } catch (...) {
                deallocate_map(_map, _map_size);
                _map = 0;
                _map_size = 0;
            }

            // 设置_start 和 _finish
            _start.set_node(nstart);
            _finish.set_node(nfinish - 1);
            _start.cur = _start.first;
            _finish.cur = _finish.first + num_elements % _deque_buf_size(sizeof(T));
        }

        // 给初始化的数据结构赋初值
        void fill_initialize(const value_type& value) {
            map_pointer cur;
            try {
                for (cur = _start.node; cur < _finish.node; ++cur)
                    stl::uninitialized_fill(*cur, *cur + _deque_buf_size(sizeof(T)), value);
                stl::uninitialized_fill(_finish.first, _finish.cur, value);
            } catch (...) {
                stl::destroy(_start, iterator(*cur, cur));
            }
        }

        // 先这样写吧
        template<class Integer>
        void deque_dispatch(Integer n, Integer value, std::__true_type);
        template<class InputIterator>
        void deque_dispatch(InputIterator first, InputIterator last, std::__false_type);

        template <class InputIterator>
        void range_initialize(InputIterator first, InputIterator last, input_iterator_tag);

        template <class ForwardIterator>
        void range_initialize(ForwardIterator first, ForwardIterator last, forward_iterator_tag);

        iterator insert_aux(const_iterator pos, const value_type& value) {
            // 主要判断插入元素之后整体向前移动还是向后移动
            difference_type index = pos - _start;
            value_type value_copy = value;
            if (index < size() / 2) {  // 插入点之前元素较少，向前移动
                push_front(front());
                iterator front1 = _start;
                ++front1;
                iterator front2 = front1;
                ++front2;
                pos = _start + index;  // ???
                iterator pos1 = pos;
                stl::copy(front2, pos1, front1);
            } else {
                push_back(back());
                iterator back1 = back();
                --back1;
                iterator back2 = back1;
                --back2;
                stl::copy_backward(pos, back2, back1);
            }
            *pos = value_copy;
            return pos;
        }

        // 若没有使用map一半空间，则说明map使用偏back或者front，将之移动到中央
        // 反之，申请两倍空间
        void reallocate_map(size_type nodes_to_add, bool add_at_front) {
            size_type old_num_nodes = _finish.node - _start.node + 1;
            size_type new_num_nodes = old_num_nodes + nodes_to_add;

            map_pointer new_nstart;
            if (_map_size > 2 * new_num_nodes) {  // map空间足够，移到最中央
                new_nstart = _map + (_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
                if (new_nstart < _start.node)
                    copy(_start.node, _finish.node + 1, new_nstart);
                else
                    copy_backward(_start.node, _finish.node + 1, new_nstart + old_num_nodes);
            } else {  // map空间不足，重新申请空间
                size_type new_map_size = _map_size + std::max(_map_size, nodes_to_add) + 2;
                map_pointer new_map = allocate_map(new_map_size);
                new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
                copy(_start.node, _finish.node + 1, new_nstart);
                deallocate_map(_map, _map_size);
                _map = new_map;
                _map_size = new_map_size;
            }
            _start.set_node(new_nstart);
            _finish.set_node(new_nstart + old_num_nodes -  1);
        }

        void reserve_map_at_back(size_type nodes_to_add = 1) {
            if (nodes_to_add + 1 > _map_size - (_finish.node - _map))
                reallocate_map(nodes_to_add, false);
        }
        void reserve_map_at_front(size_type nodes_to_add = 1) {
            if (nodes_to_add > _start.node - _map)
                reallocate_map(nodes_to_add, true);
        }

        void push_back_aux(const value_type& value) {
            value_type value_copy = value;
            reserve_map_at_back();
            *(_finish.node + 1) = allocate_node();  // 申请新节点

            try {
                construct(_finish.cur, value_copy);  // 给上个节点最后一个位置设值
                _finish.set_node(_finish.node + 1);  // 给迭代器配置新缓冲区
                _finish.cur = _finish.first;   // 改变_finish的状态
            } catch (...) {
                deallocate_node(*(_finish.node + 1));
            }
        }
        void push_front_aux(const value_type& value) {
            value_type value_copy = value;
            reserve_map_at_front();
            *(_start.node - 1) = allocate_node();

            try {
                _start.set_node(_start.node - 1);  // 前闭后开，与back处理稍有不同
                _start.cur = _start.last - 1;
                construct(_start.cur, value_copy);
            } catch (...) {
                _start.set_node(_start.node);
                _start.cur = _start.first;
                deallocate_node(*(_start.node - 1));
            }

        }

    };

    template<class T>
    deque<T>::deque(deque::size_type n, const value_type &value) {
        initialize_map_and_nodes(n);
        fill_initialize(value);
        // map_pointer cur;
        // try {
        //     for (cur = _start.node; cur < _finish.node; ++cur)
        //         stl::uninitialized_fill(*cur, *cur + _deque_buf_size(sizeof(T)), value);
        //     stl::uninitialized_fill(_finish.first, _finish.cur, value);
        // } catch (...) {
        //     stl::destroy(_start, iterator(*cur, cur));
        // }
    }

    template<class T>
    template<class Integer>
    void deque<T>::deque_dispatch(Integer n, Integer value, std::__true_type) {
        initialize_map_and_nodes(n);
        fill_initialize(value);
    }

    template<class T>
    template<class InputIterator>
    void deque<T>::range_initialize(InputIterator first, InputIterator last, input_iterator_tag) {
        // TODO
    }
    template<class T>
    template<class ForwardIterator>
    void deque<T>::range_initialize(ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
        // TODO
    }
    template<class T>
    template<class InputIterator>
    void deque<T>::deque_dispatch(InputIterator first, InputIterator last, std::__false_type) {
        range_initialize(first, last, stl::iterator_category(first));
    }
    template<class T>
    template<class InputIterator>
    deque<T>::deque(InputIterator first, InputIterator last) {
        typedef typename std::__is_integer<InputIterator>::__type _Integral;
        deque_dispatch(first, last, _Integral());
    }
    template<class T>
    deque<T>::deque(const deque &x) {
        // TODO
    }
    template<class T>
    deque<T>& deque<T>::operator=(const deque &x) {
        // TODO
        return deque();
    }

    template<class T>
    deque<T>::~deque() {
        if (_map) {
            stl::destroy(_start, _finish);  // 析构
            destroy_nodes(_start.node, _finish.node + 1);  // 释放节点
            deallocate_map(_map, _map_size);  // 释放map
        }
    }

    template<class T>
    void deque<T>::shrink_to_fit() {
        // TODO
    }
    template<class T>
    void deque<T>::clear() {
        for (map_pointer node = _start.node + 1; node < _finish.node; ++node) {
            stl::destroy(*node, *node + _deque_buf_size(sizeof(T)));  // 析构
            deallocate_node(*node);  // 释放内存
        }
        if (_start.node != _finish.node) {
            stl::destroy(_start.cur, _start.last);
            stl::destroy(_finish.first, _finish.cur);
            // deallocate_node(_start.first);  // 不能释放
            deallocate_node(_finish.first);
        } else
            destroy(_start.cur, _finish.cur);
        _finish = _start;
    }

    template<class T>
    typename deque<T>::iterator deque<T>::insert(deque::const_iterator pos, const T &value) {
        if (pos.cur == _start.cur) {
            push_front(value);
            return _start;
        } else if (pos.cur == _finish.cur) {
            push_back(value);
            iterator temp = _finish;
            --temp;
            return temp;
        } else {
            return insert_aux(pos, value);
        }
    }

    template<class T>
    typename deque<T>::iterator deque<T>::insert(deque::const_iterator pos, deque::size_type count, const T &value) {
        //  TODO
        return stl::deque<T>::iterator();
    }

    template<class T>
    template<class InputIterator>
    typename deque<T>::iterator deque<T>::insert(deque::const_iterator pos, InputIterator first, InputIterator last) {
        // TODO
        return stl::deque<T>::iterator();
    }

    template<class T>
    typename deque<T>::iterator deque<T>::insert(deque::const_iterator pos, std::initializer_list<T> ilist) {
        // TODO
        return stl::deque<T>::iterator();
    }

    template<class T>
    typename deque<T>::iterator deque<T>::erase(deque::iterator pos) {
        iterator next = pos;
        ++next;
        difference_type index = pos - _start;
        if (index < (size() >> 1)) {  // 清除点之前的元素少
            copy_backward(_start, pos, next);
            pop_front();
        } else {
            copy(next, _finish, pos);
            pop_back();
        }
        return _start + index;
    }

    template<class T>
    typename deque<T>::iterator deque<T>::erase(deque::iterator first, deque::iterator last) {
        // TODO
        // 可以循环调用erase(pos), 但不同于链表，会重复复制效率低
        return stl::deque<T>::iterator();
    }

    template<class T>
    void deque<T>::push_back(const T &value) {
        // std::cout << _map_size << std::endl;
        if (_finish.cur != _finish.last - 1) {  // 尚有两个备用空间，若有一个，在插入一个，则last跑到了下一个缓冲区的开始
            stl::construct(_finish.cur, value);  // 直接在备用空间构造元素
            ++_finish.cur;
        } else {
            push_back_aux(value);
        }
    }

    template<class T>
    void deque<T>::pop_back() {
        if (_finish.cur != _finish.first) {
            --_finish.cur;
            stl::destroy(_finish.cur);
        } else {
            deallocate_node(_finish.first);  // 释放最后一个缓冲区
            _finish.set_node(_finish.node - 1);
            _finish.cur = _finish.last - 1;  // 指向上一个缓冲区的最后一个元素
            stl::destroy(_finish.cur);
        }
    }

    template<class T>
    void deque<T>::push_front(const T &value) {
        if (_start.cur != _start.first) {
            construct(_start.first - 1, value);
            --_start.cur;
        } else {
            push_front_aux(value);
        }
    }

    template<class T>
    void deque<T>::pop_front() {
        if (_start.cur != _start.last - 1) {  // 第一个缓冲区有两个及以上元素
            stl::destroy(_start.cur);
            ++_start.cur;
        } else {  // 第一个缓冲区只有一个元素
            stl::destroy(_start.cur);  // 析构
            deallocate_node(_start.first);
            _start.set_node(_start.node + 1);
            _start.cur = _start.first;
        }
    }

    template<class T>
    void deque<T>::resize(deque::size_type count) {
        resize(count, value_type());
    }

    template<class T>
    void deque<T>::resize(deque::size_type count, const T &value) {
        const size_type len = size();
        if (count < len)
            erase(_start + count, _finish);
        else
            insert(_finish, count - len, value);
    }

    template<class T>
    void deque<T>::swap(deque &other) {
        stl::swap(_start, other._start);
        stl::swap(_finish, other._finish);
        stl::swap(_map, other._map);
        stl::swap(_map_size, other._map_size);
    }
}

#endif //KTINYSTL_DEQUE_H


















