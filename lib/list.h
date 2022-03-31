#ifndef KTINYSTL_LIST_H
#define KTINYSTL_LIST_H

#include "allocator.h"
#include "global_func.h"
#include <list>

namespace stl
{
    template<class T>
    struct _node {
        typedef _node*      pointer;
        pointer prev;
        pointer next;
        T data;
        _node(const T& value): data(value){}
        _node(T& value): data(value){}
    };

    // 定义三个变量的原因是使用到const iterator的时候方便，不需要重新定义一个
    template<class T, class Ref, class Ptr>
    struct _list_iterator {
        typedef _list_iterator<T, T&, T*>   iterator;
        typedef _list_iterator<T, Ref, Ptr>   self;
        typedef _list_iterator<T, const T&, const T*>   const_iterator;

        typedef bidirectional_iterator_tag  iterator_category;
        typedef T                           type_value;
        typedef T*                          pointer;
        typedef const T*                    const_pointer;
        typedef T&                          reference;
        typedef _node<T>*                   link_type;
        typedef size_t                      size_type;
        typedef ptrdiff_t                   difference;

        link_type _node;  // 迭代器内部的数据

        // ctor
        _list_iterator()= default;
        explicit _list_iterator(link_type x): _node(x) {}
        _list_iterator(const iterator& x): _node(x._node) {}

        bool operator==(const self& x) const {return _node == x._node; }
        bool operator!=(const self& x) const {return _node != x._node; }

        reference operator*() const {return _node->data; }  // 取节点的值
        pointer operator->() const {return &(operator*()); }  // 成员存取

        self& operator++() {  // 前++
            _node = (link_type)((*_node).next);
            return *this;
        }
        self operator++(int) {  // 后++
            self temp = *this;
            ++*this;
            return temp;
        }

        self& operator--() {  // 前--
            _node = (link_type)((*_node).prev);
            return *this;
        }
        self operator--(int) {
            self temp = *this;
            --*this;
            return temp;
        }

        iterator _const_cast() {
            return iterator(const_cast<stl::_node<T>*>(_node));
        }
    };


    template<class T>
    class list {
    public:
        typedef T                                           value_type;
        typedef T*                                          pointer;
        typedef const T*                                    const_pointer;
        typedef T&                                          reference;
        typedef const T&                                    const_reference;
        typedef _node<T>*                                   link_type;
        typedef _list_iterator<T, T&, T*>                   iterator;
        // typedef _list_iterator<T, T&, T*>                   const_iterator;
        typedef _list_iterator<T, const T&, const T*>       const_iterator;
        typedef size_t                                      size_type;

    private:
        typedef stl::allocator<_node<T>>                   node_allocator;
        link_type _node;  // 循环链表，初始化为尾结点

    public:
        //*******************构造和析构函数***************************//
        list() {empty_init(); }
        explicit list(size_type n, const_reference value = value_type()) {empty_init(); insert(begin(), n, value); }

        template<class InputIterator>
        list(InputIterator first, InputIterator last) {empty_init(); insert(begin(), first, last); }
        list(const list& lst);
        list& operator=(const list& lst);
        list(std::initializer_list<T> init);
        ~list();

        //*******************元素获取***************************//
        reference front() {return *begin(); }  // iterator重载版本的 * ，好谜
        const_reference front() const  {return *begin(); }
        reference back() {return *(--end()); }
        const_reference back() const {return *(--end()); }

        //*******************迭代器***************************//  _node->next == (*_node).data ????
        iterator begin() {return iterator(_node->next); }  // return _node->next ????
        const_iterator begin() const {return const_iterator(_node->next); }
        const_iterator cbegin() const {return const_iterator(_node->next); }
        iterator end() {return iterator(_node); }
        const_iterator end() const {return const_iterator(_node); }
        const_iterator cend() const {return const_iterator(_node); }

        //*******************迭代器***************************//
        bool empty() const {return _node->next == _node; }
        size_type size() const;
        size_type max_size() const {return size_type(-1); }

        //*******************修改器***************************//
        void clear();
        iterator insert(const_iterator pos, const_reference value);
        // iterator insert(const_iterator pos, T&& value) {insert(pos, value); }
        iterator insert(const_iterator pos, size_type count, const_reference value);

        template<class InputIterator>
        iterator insert(const_iterator pos, InputIterator first, InputIterator last);
        iterator erase(const_iterator pos);
        iterator erase(const_iterator first, const_iterator last);

        template<class... Args>
        iterator emplace(const_iterator pos, Args &&... args);
        void push_back(const_reference value) {insert(cend(), value); }
        void push_back(T&& value) {insert(end(), value); }
        void pop_back() {iterator temp = end(); erase(--temp); }
        void push_front(const_reference value) {insert(begin(), value_type()); }
        void push_front(T&& value) {insert(begin(), value); }
        void pop_front() {erase(begin()); }
        void resize(size_type count) {resize(count, value_type()); }
        void resize(size_type count, const_reference value);
        void swap(list& lst) {stl::swap(_node, lst._node); }

        //*******************Operations***************************//
        void merge(list& lst);  // 必须使用排好序的list

        template<class Compare>
        void merge(list& lst, Compare comp);

        void splice(const_iterator pos, list& lst);  // 将lst插入到pos之前
        void splice(const_iterator pos, list& lst, const_iterator it);  // 将it指向的元素插入到pos之前， 可指向同一个list
        void splice(const_iterator pos, list& lst, const_iterator first, const_iterator last);  // 将[first, last)元素插入到pos之前， 可指向同一个list, pos not in [first, last)
        void reverse();
        void sort();  // n*log(n)

        template<class Compare>
        void sort(Compare comp);  // QuickSort

        //***************<=>******************************//
        bool operator==(list<T> lst) {return _node == lst._node; }
        bool operator!=(list<T> lst) {return _node != lst._node; }




    private:
        link_type get_node() {return node_allocator::allocate(); }  // 配置一个节点并返回
        void put_node(link_type p) {node_allocator::deallocate(p); }  // 释放一个节点

        // 配置并构造一个节点
        link_type create_node(const_reference value) {
            link_type p = get_node();
            construct(&p->data, value);
            return p;
        }
        // 释放并析构一个节点
        void destroy_node(link_type p) {
            destroy(&p->data);
            put_node(p);
        }

        template<class InputIterator>
        iterator insert_aux(list::const_iterator pos, InputIterator first, InputIterator last, std::__true_type) {
            // InputIterator为int, 相当于insert(pos, n, value),
            insert(pos, static_cast<size_type>(first), static_cast<value_type>(last));
        }
        template<class InputIterator>
        iterator insert_aux(list::const_iterator pos, InputIterator first, InputIterator last, std::__false_type) {
            for ( ; first != last; ++first)
                insert(pos, *first);
        }
        void empty_init() {
            _node = get_node();
            _node->next = _node;
            _node->prev = _node;
        }

        // 辅助函数，将[first,last)中的所有元素移动到pos之前.技术上属于节点间的指针移动
        void transfer(iterator pos, iterator first, iterator last) {
            // Remove [first, last) from its old position.
            last._node->prev->next = pos._node;
            first._node->prev->next = last._node;
            pos._node->prev->next = first._node;

            // Splice [first, last) into its new position.
            link_type temp = pos._node->prev;
            pos._node->prev = last._node->prev;
            last._node->prev = first._node->prev;
            first._node->prev = temp;
        }
    };

    template<class T>
    list<T>::list(const list &lst) {
        empty_init();
        if (this != &lst) {
            iterator first1 = begin();
            iterator last1 = end();
            const_iterator first2 = lst.begin();
            const_iterator last2 = lst.end();
            while(first1 != last1 && first2 != last2)
                *(first1++) = *(first2++);
            if (first2 == last2)  // end of lst
                erase(first1, last1);  //
            else
                insert(last1, first2, last2);
        }
    }
    template<class T>
    list<T> &list<T>::operator=(const list &lst) {
        empty_init();
        if (this != &lst) {
            iterator first1 = begin();
            iterator last1 = end();
            const_iterator first2 = lst.begin();
            const_iterator last2 = lst.end();
            while(first1 != last1 && first2 != last2)
                *(first1++) = *(first2++);
            if (first2 == last2)  // end of lst
                erase(first1, last1);  //
            else
                insert(last1, first2, last2);
        }
    }
    template<class T>
    list<T>::list(std::initializer_list<T> init) {
        empty_init();
        typename std::initializer_list<T>::iterator first = init.begin();
        typename std::initializer_list<T>::iterator last = init.end();
        for (; first != last; ++first)
            push_back(*first);
    }

    template<class T>
    list<T>::~list() {
        link_type cur = _node->next;
        while (cur != _node) {
            link_type temp = cur;
            cur = cur->next;
            destroy_node(temp);
        }
        destroy_node(cur);
    }

    template<class T>
    typename list<T>::size_type list<T>::size() const {
        size_type n = 0;
        while (begin() != end()) {
            ++begin();
            ++n;
        }
        return n;
    }

    template<class T>
    void list<T>::clear() {
        link_type cur = _node->next;
        while (cur != _node) {
            link_type temp = cur;
            cur = cur->next;
            destroy_node(temp);
        }
        _node->next = _node;
        _node->prev = _node;
    }

    template<class T>
    typename list<T>::iterator list<T>::insert(const_iterator pos, const_reference value) {
        link_type temp = create_node(value);
        temp->next = pos._node;
        temp->prev = pos._node->prev;
        (pos._node->prev)->next = temp;
        pos._node->prev = temp;
        return iterator(temp);
    }
    template<class T>
    typename list<T>::iterator list<T>::insert(const_iterator pos, size_type count, const_reference value) {
        for (; count > 0; --count) {
            insert(pos, value);
        }
    }

    template<class T>
    template<class InputIterator>
    typename list<T>::iterator list<T>::insert(list::const_iterator pos, InputIterator first, InputIterator last) {
        typedef typename std::__is_integer<InputIterator>::__type _Integral;
        insert_aux(pos, first, last, _Integral());
    }

    template<class T>
    typename list<T>::iterator list<T>::erase(list::const_iterator pos) {
        link_type prev_node = pos._node->prev;
        link_type next_node = pos._node->next;
        prev_node->next = next_node;
        next_node->prev = prev_node;
        destroy_node(pos._node);
        return iterator(next_node);
    }

    template<class T>
    typename list<T>::iterator list<T>::erase(list::const_iterator first, list::const_iterator last) {
        while (first != last)
            erase(first++);
        // C++11标准为什么要传const，后面又要转换为non-const传出去
        return iterator(const_cast<stl::_node<T>*>(last._node));
    }

    template<class T>
    void list<T>::resize(list::size_type count, const_reference value) {
        iterator first = begin();
        size_type len = 0;
        for (; first != end() && len < count; ++first)
            ++len;
        if (len == count)
            erase(first, end());
        else
            insert(end(), count - len, value);
    }

    template<class T>
    void list<T>::merge(list& lst) {
        if (*this != lst) {
            iterator first1 = begin();
            iterator last1 = end();
            iterator first2 = lst.begin();
            iterator last2 = lst.end();
            while (first1 != last1 && first2 != last2) {
                if (*first2 < *first1) {
                    iterator next = first2;
                    transfer(first1, first2, ++next);
                    first2 = next;
                } else
                    ++first1;
            }
            if (first2 != last2)
                transfer(last1, first2, last2);
        }
    }

    template<class T>
    template<class Compare>
    void list<T>::merge(list &lst, Compare comp) {
        if (*this != lst) {
            iterator first1 = begin();
            iterator last1 = end();
            iterator first2 = lst.begin();
            iterator last2 = lst.end();
            while (first1 != last1 && first2 != last2) {
                if (comp(*first2, *first1)) {
                    iterator next = first2;
                    transfer(first1, first2, ++next);
                    first2 = next;
                } else
                    ++first1;
            }
            if (first2 != last2)
                transfer(last1, first2, last2);
        }
    }

    template<class T>
    void list<T>::splice(list::const_iterator pos, list &lst) {
        if(!lst.empty() && *this != lst)
            transfer(pos._const_cast(), lst.begin(), lst.end());
    }

    template<class T>
    void list<T>::splice(list::const_iterator pos, list &lst, list::const_iterator it) {
        iterator jt = it._const_cast();
        ++jt;
        if (pos == it || pos == jt) return;
        transfer(pos._const_cast(), it._const_cast(), jt._const_cast());
    }

    template<class T>
    void list<T>::splice(list::const_iterator pos, list &lst, list::const_iterator first, list::const_iterator last) {
        if (first != last)
            transfer(pos._const_cast(), first._const_cast(), last._const_cast());
    }

    template<class T>
    void list<T>::reverse() {
        if (_node->next == _node || _node->next->next == _node) return;
        iterator first = begin();
        while (first != end()) {
            iterator old = first;
            ++first;
            transfer(begin(), old, first);
        }

    }
    template<class T>
    void list<T>::sort() {
        if (_node->next == _node || _node->next->next == _node) return;
        list<T> carry;
        list<T> counter[64];  // 2**64, enough to carry
        int fill = 0;
        while (!empty()) {
            carry.splice(carry.begin(), *this, begin());
            int i = 0;
            while (i < fill && !counter[i].empty()) {
                counter[i].merge(carry);
                carry.swap(counter[i++]);
            }
            carry.swap(counter[i]);
            if (i == fill) ++fill;
        }
        for (int i = 1; i < fill; ++i)
            counter[i].merge(counter[i - 1]);
        swap(counter[fill - 1]);
    }

    template<class T>
    template<class Compare>
    void list<T>::sort(Compare comp) {
        if (_node->next == _node || _node->next->next == _node) return;
        list<T> carry;
        list<T> counter[64];  // 2**64, enough to carry
        int fill = 0;
        while (!empty()) {
            carry.splice(carry.begin(), *this, begin());
            int i = 0;
            if (i < fill && !counter[i].empty()) {
                counter[i].merge(carry, comp);
                carry.swap(counter[i++]);
            }
            carry.swap(counter[i]);
            if (i == fill) ++fill;
        }
        for (int i = 1; i < fill; ++i)
            counter[i].merge(counter[i - 1], comp);
        swap(counter[fill - 1]);
    }


}



#endif //KTINYSTL_LIST_H
