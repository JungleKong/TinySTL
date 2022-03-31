#ifndef KTINYSTL_SET_H
#define KTINYSTL_SET_H

#include "rb_tree.h"

#include <utility>
#include <functional>

namespace stl
{
    template<class Key, class Compare = std::less<Key>>
    class set{
    public:
        typedef Key key_type;
        typedef Key value_type;
        typedef Compare key_compare;
        typedef Compare value_compare;
    private:
        typedef rb_tree<key_type, value_type, std::_Identity<value_type>, value_compare> tree_type;
        tree_type t;
    public:
        typedef typename tree_type::const_pointer pointer;
        typedef typename tree_type::const_pointer const_pointer;
        typedef typename tree_type::const_reference reference;
        typedef typename tree_type::const_reference const_reference;
        typedef typename tree_type::const_iterator iterator;  // 不允许通过迭代器修改值
        typedef typename tree_type::const_iterator const_iterator;
        typedef typename tree_type::size_type size_type;
        typedef typename tree_type::difference_type difference_type;

        set(): t(Compare()) {}
        // 构造函数就不多写了。复合关系，会自动调用t的析构函数

        key_compare key_comp() const { return t.key_comp(); }
        value_compare value_comp() const { return t.key_comp(); }

        iterator begin() const { return t.begin(); }
        iterator end() const { return t.end(); }
        bool empty() const { return t.empty(); }
        size_type size() const { return t.size(); }
        size_type max_size() const { return t.max_size(); }
        void swap(set<Key,Compare>& __x) { t.swap(__x._M_t); }

        std::pair<iterator, bool> insert(const value_type& v) {
            std::pair<typename tree_type::iterator, bool> p = t.insert_unique(v);
            return std::pair<iterator, bool>((iterator)p.first, p.second);
        }

        iterator erase(iterator pos) {
            typedef typename tree_type::iterator tree_iterator;
            return t.erase((tree_iterator&)pos);
        }

        // 其他的函数都是调用t的函数来完成

    };
}


#endif //KTINYSTL_SET_H
