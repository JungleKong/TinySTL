#ifndef KTINYSTL_RB_TREE_H
#define KTINYSTL_RB_TREE_H

#include "iterator.h"
#include "allocator.h"
#include "global_func.h"
#include <algorithm>

#include <utility>

namespace stl
{
    typedef bool rb_tree_color_type;
    const rb_tree_color_type rb_tree_red = false;
    const rb_tree_color_type rb_tree_black = true;

    // struct rb_tree_node_base {
    //     typedef rb_tree_color_type  color_type;
    //     typedef rb_tree_node_base*       base_ptr;
    //
    //     color_type color;
    //     base_ptr parent;
    //     base_ptr left;
    //     base_ptr right;
    //
    //     static base_ptr minimum(base_ptr x) {
    //         while (x->left != nullptr) x = x->left;  // 一直往左就会找到最小值
    //         return x;
    //     }
    //
    //     static base_ptr maximum(base_ptr x) {
    //         while (x->right != nullptr) x = x->right;  // 一直往右走
    //         return x;
    //     }
    // };

    template<class T>
    struct rb_tree_node {
        typedef rb_tree_color_type          color_type;
        // typedef rb_tree_node_base*          base_ptr;
        typedef rb_tree_node*               link_type;
        T data;

        color_type color;
        link_type parent;
        link_type left;
        link_type right;

        static link_type minimum(link_type x) {
            while (x->left != nullptr) x = x->left;  // 一直往左就会找到最小值
            return x;
        }

        static link_type maximum(link_type x) {
            while (x->right != nullptr) x = x->right;  // 一直往右走
            return x;
        }

    };

    // // 迭代器
    // struct rb_tree_base_iterator {
    //     typedef rb_tree_node_base::base_ptr base_ptr;
    //     typedef bidirectional_iterator_tag iterator_category;
    //     typedef ptrdiff_t difference_type;
    //
    //     base_ptr node;
    //
    //     // rb_tree的视线中header节点是一个哑结点
    //     // header左节点指向min节点，即最左的那一个节点
    //     // header右节点指向max节点，即最右的那一个节点
    //     // header父节点指向根节点
    //     // operator++
    //     void increment() {
    //         if (node->right != nullptr) {  // 有右子节点
    //             node = node->right;
    //             while (node->left != nullptr)
    //                 node = node->left;
    //         } else {
    //             base_ptr temp = node->parent;
    //             while (node == temp->right) {  // 是父节点的右节点
    //                 node = temp;
    //                 temp = temp->parent;
    //             }
    //             if (node->right != temp)  // 考虑到header节点的情况。
    //                 node = temp;
    //         }
    //     }
    //
    //     void decrement() {
    //         if (node->color == rb_tree_red && node->parent->parent == node)
    //             node = node->right;  // 根节点， 考虑header
    //         else if (node->left != nullptr) {  // 有左子节点
    //             base_ptr temp = node->left;
    //             while (temp->right != nullptr)
    //                 temp = temp->right;
    //             node = temp;
    //         } else {  // 无左子节点
    //             base_ptr temp = node->parent;
    //             while (node == temp->left) { // 最左节点情况，考虑header, 即--之后为header节点
    //                 node = temp;
    //                 temp = temp->parent;
    //             }
    //             node = temp;
    //         }
    //     }
    // };

    // 不使用继承关系了
    template<class T, class Ref, class Ptr>
    struct rb_tree_iterator {
        typedef bidirectional_iterator_tag                  iterator_category;
        typedef T                                           value_type;
        typedef Ref                                         reference;
        typedef Ptr                                         pointer;
        typedef rb_tree_iterator<T, T&, T*>                 iterator;
        typedef rb_tree_iterator<T, const T&, const T*>     const_iterator;
        typedef rb_tree_iterator<T, Ref, Ptr>               self;
        typedef rb_tree_node<T>*                            link_type;
        typedef ptrdiff_t                                   difference_type;

        link_type node;

        rb_tree_iterator() = default;
        explicit rb_tree_iterator(link_type x) {node = x; }
        rb_tree_iterator(const iterator& it) {node = it.node; }

    public:
        bool operator==(const self& x) const {return node == x.node; }
        bool operator!=(const self& x) const {return node != x.node; }

        reference operator*() const {return node->data; }
        pointer operator->() const {return &(operator*()); }

        self& operator++() {
            increment();
            return *this;
        }
        self operator++(int) {
            self temp = *this;
            increment();
            return temp;
        }
        self& operator--() {
            decrement();
            return *this;
        }
        self operator--(int) {
            self temp = *this;
            decrement();
            return temp;
        }

        // rb_tree的视线中header节点是一个哑结点
        // header左节点指向min节点，即最左的那一个节点
        // header右节点指向max节点，即最右的那一个节点
        // header父节点指向根节点
        // operator++
        void increment() {
            if (node->right != nullptr) {  // 有右子节点
                node = node->right;
                while (node->left != nullptr)
                    node = node->left;
            } else {
                link_type temp = node->parent;
                while (node == temp->right) {  // 是父节点的右节点
                    node = temp;
                    temp = temp->parent;
                }
                if (node->right != temp)  // 考虑到header节点的情况。
                    node = temp;
            }
        }

        void decrement() {
            if (node->color == rb_tree_red && node->parent->parent == node)
                node = node->right;  // 根节点， 考虑header
            else if (node->left != nullptr) {  // 有左子节点
                link_type temp = node->left;
                while (temp->right != nullptr)
                    temp = temp->right;
                node = temp;
            } else {  // 无左子节点
                link_type temp = node->parent;
                while (node == temp->left) { // 最左节点情况，考虑header, 即--之后为header节点
                    node = temp;
                    temp = temp->parent;
                }
                node = temp;
            }
        }
    };

    template<class T, class Ref, class Ptr>
    inline bool operator==(const rb_tree_iterator<T, Ref, Ptr>& __x, const rb_tree_iterator<T, Ref, Ptr>& __y) {
        return __x.node == __y.node;
    }

    template<class T, class Ref, class Ptr>
    inline bool operator!=(const rb_tree_iterator<T, Ref, Ptr>& __x, const rb_tree_iterator<T, Ref, Ptr>& __y) {
        return __x.node != __y.node;
    }

    template<class Key, class Value, class KeyOfValue, class Compare>
    class rb_tree {
    public:
        typedef Key                             key_type;
        typedef Value                           value_type;
        typedef Value*                          pointer;
        typedef const Value*                    const_pointer;
        typedef Value&                          reference;
        typedef const Value&                    const_reference;
        typedef rb_tree_node<Value>*            link_type;
        typedef size_t                          size_type;
        typedef ptrdiff_t                       difference_type;

        // typedef rb_tree_iterator<Value, Value&, Value*> iterator;
        typedef rb_tree_iterator<value_type , reference , pointer> iterator;
        typedef rb_tree_iterator<value_type , const_reference , const_pointer> const_iterator;

    private:
        typedef void*                           void_pointer;  // 这个是干嘛用的？
        typedef rb_tree_node<Value>             rb_tree_node;
        typedef allocator<rb_tree_node>         rb_tree_node_allocator;
        typedef rb_tree_color_type              color_type;

        // 只记录三个数据
        size_type node_count;  // 节点数量
        link_type header;  // 头结点，一个实现上的技巧，header左节点指向min节点，即最左的那一个节点；header右节点指向max节点，即最右的那一个节点；header父节点指向根节点
        Compare key_compare;  // 用以比较键值大小的仿函数

    public:
        // rb_tree(): node_count(0), key_compare() {
        //     init();
        // }
        explicit rb_tree(const Compare& comp = Compare())
        :node_count(0), key_compare(comp) {
            init();
        }
        ~rb_tree() {
            clear();
            put_node(header);
        }

        Compare key_comp() const {return key_compare; }
        iterator begin() {return iterator(leftmost()); }
        const_iterator begin() const {return iterator(leftmost()); }
        iterator end() {return iterator(header); }
        const_iterator end() const {return iterator(header); }
        bool empty() const {return node_count == 0; }
        size_type size() const {return node_count; }
        size_type max_size() const {return size_type(-1); }

        void clear() {
            if (node_count != 0) {
                erase_without_rebalance(root());
                leftmost() = header;
                root() = nullptr;
                rightmost() = header;
                node_count = 0;
            }
        }

        iterator insert_equal(const value_type& v) {
            link_type y = header;
            link_type x = root();

            while (x != nullptr) {  // 从根节点开始寻找插入点，遇大往左，与小或等于往右
                y = x;
                x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
            }
            return _insert(x, y, v);
        }

        std::pair<iterator, bool> insert_unique(const value_type& v) {
            link_type y = header;
            link_type x = root();
            bool comp = true;
            while (x != 0) {
                y = x;
                comp = key_compare(KeyOfValue()(v), key(x));
                x = comp ? left(x) : right(x);
            }
            iterator j = iterator(y);
            if (comp) {
                if (j == begin())  // 插入点父节点为最左侧节点，
                    return std::pair<iterator, bool>(_insert(x, y, v), true);
                else
                    --j;
            }
            if (key_compare(key(j.node), KeyOfValue()(v)))  // 键值不重复
                return std::pair<iterator, bool>(_insert(x, y, v), true);
            return std::pair<iterator, bool>(j, false);   // 重复
        }

        size_type erase(const value_type& value) {
            std::pair<iterator, iterator> p = equal_range(value);
            iterator p1 = p.first;
            iterator p2 = p.second;
            size_type old_size = size();
            erase(p1, p2);
            return old_size - size();
        }
        iterator erase(iterator first, iterator last) {
            if (first == begin() && last == end())
                clear();
            else
                while (first != last)
                    first = erase(first);
            return last;
        }

        iterator erase(iterator pos) {
            iterator result = pos;
            result++;
            // link_type y = rb_tree_erase_and_rebalance(pos.node);  // 这个函数还是有点问题。。。。
            link_type y = rb_tree_rebalance_for_erase(pos.node, header->parent, header->left, header->right);
            destroy_node(y);
            node_count--;
            return result;
        }

        iterator find(const value_type& value){
            link_type y = header;
            link_type x = root();
            while (x != nullptr) {
                if (!key_compare(key(x), value)) {
                    y = x;
                    x = left(x);
                } else
                    x = right(x);
            }
            iterator j = iterator(x);
            return (j == end() || key_compare(value, key(j.node))) ? end() : j;
        }

        size_type count(const value_type& v) {
            std::pair<iterator, iterator> p = equal_range(v);
            return stl::distance(p.first, p.second);
        }

        void swap(rb_tree<Key, Value, KeyOfValue, Compare>& tree) {
            stl::swap(header, tree.header);
            stl::swap(node_count, tree.node_count);
            stl::swap(key_compare, tree.key_compare);
        }

    private:
        // 三个函数方便取得header的成员
        link_type& root() const {return static_cast<link_type&>(header->parent); }  // 不转型应该问题也不大
        link_type& leftmost() const {return static_cast<link_type&>(header->left); }
        link_type& rightmost() {return static_cast<link_type&>(header->right); }

        // 以下六个函数方便取得节点的成员
        static link_type& left(link_type x) {return static_cast<link_type&>(x->left); }
        static link_type& right(link_type x) {return static_cast<link_type&>(x->right); }
        static link_type& parent(link_type x) {return static_cast<link_type&>(x->parent); }
        static reference value(link_type x) {return x->data; }
        static const Key& key(link_type x) {return KeyOfValue()(value(x)); }
        static color_type& color(link_type x) {return x->color; }

        // 求取极大值和极小值
        static link_type minimum(link_type x) {return rb_tree_node::minimum(x); }  // 搞不懂这里为什么不返回引用
        static link_type maximum(link_type x) {return rb_tree_node::maximum(x); }

        // 申请一个节点的空间
        link_type get_node() {
            return rb_tree_node_allocator::allocate();
        }
        // 申请并配置一个节点的空间
        link_type create_node(const value_type& value) {
            link_type temp = get_node();
            try {
                stl::construct(&temp->data, value);
            } catch (...) {
                put_node(temp);
            }
            return temp;
        }
        // 释放一个节点空间
        void put_node(link_type p) {
            rb_tree_node_allocator::deallocate(p);
        }
        // 析构并释放一个节点
        void destroy_node(link_type p) {
            stl::destroy(&p->data);
            put_node(p);
        }
        // 复制一个节点，值和颜色
        link_type clone(link_type x) {
            link_type temp = create_node(x->data);
            temp->color = x->color;
            temp->left = nullptr;
            temp->right = nullptr;
            return temp;
        }

        void init() {
            header = get_node();
            color(header) = rb_tree_red;

            root() = 0;
            leftmost() = header;
            rightmost() = header;
        }



    private:
        /**
         * 插入操作辅助函数
         * @param x_ 新值插入的节点
         * @param y_ 插入节点的父节点
         * @param v 插入值
         * @return 迭代器指向新增节点
         */
        iterator _insert(link_type x_, link_type y_, const value_type& v) {
            link_type x = x_;
            link_type y = y_;
            link_type z;

            if (y == header || x != nullptr || key_compare(KeyOfValue()(v), key(y))) {  // 特殊情况
                z = create_node(v);
                left(y) = z;
                if (y == header) {
                    root() = z;
                    rightmost() = z;
                } else if(y == leftmost())
                    leftmost() = z;
            } else {  // 一般情况
                z = create_node(v);
                right(y) = z;
                if (y == rightmost())
                    rightmost() = z;
            }
            parent(z) = y;  // z->parent = y
            left(z) = nullptr;  // z->left = nullptr
            right(z) = nullptr;  // z->right = nullptr

            // 调整红黑树，使满足条件
            rb_tree_rebalance(z, header->parent);  // 参数一为新增节点，参数二为root节点
            ++node_count;
            return iterator(z);
        }

        /**
         * 对二叉树左旋，就不写为全局函数了
         * @param x 旋转节点
         * @param root 根节点
         */
        inline void rb_tree_rotate_left(link_type x, link_type& root) {
            link_type y = x->right;  // y = right(x); 旋转节点的右子节点
            x->right = y->left;
            if(y->left != nullptr)
                y->left->parent = x;  // 设置父节点
            y->parent = x->parent;
            // 另 y 完全代替 x，必须将 x 的所有关系都弄过来
            if (x == root)
                root = y;
            else if (x == x->parent->left)
                x->parent->left = y;
            else
                x->parent->right = y;
            y->left = x;
            x->parent = y;
        }
        /**
         * 对二叉树右旋，就不写为全局函数了
         * @param x 旋转节点
         * @param root 根节点
         */
        inline void rb_tree_rotate_right(link_type x, link_type& root) {
            link_type y = x->left;
            x->left = y->right;
            if (y->right != nullptr)
                y->right->parent = x;
            y->parent = x->parent;

            if (x == root)
                root = y;
            else if (x == x->parent->left)
                x->parent->left = y;  // x->parent->left = y  ==? u = x->parent->left
            else
                x->parent->right = y;

            y->right = x;
            x->parent = y;
        }

        /**
         * 不写为全局函数了。插入之后进行红黑树的重新平衡操作。
         * 包括两部分  旋转 + 变色
         * 根据2-3-4树与红黑树的对应关系，
         * 由3节点变4节点有四种情况需要进行旋转，对应红黑树中：
         *      1）根左+左  根右+右 旋转一次
         *      2）根左+右  根右+左 旋转两次
         * 由4节点新增一个节点，中间节点上升变为父节点，对应红黑树中
         *      新增节点是红色 + 爷爷节点是黑丝，父节点和叔叔节点是红色 调整为：
         *      爷爷节点变为红色，父节点和叔叔节点变为黑丝，如果爷爷节点为root，则调整为黑色
         * @param x
         * @param root
         */
        inline void rb_tree_rebalance(link_type x, link_type& root) {
            x->color = rb_tree_red;  // 新增节点必为红
            while (x != root && x->parent->color == rb_tree_red) {
                if (x->parent == x->parent->parent->left){  // 父节点为祖父节点的左子节点
                    link_type y = x->parent->parent->right;  // y为叔叔节点
                    if (y != nullptr && y->color == rb_tree_red) {  // 叔叔节点存在且为红色
                        x->parent->color = rb_tree_black;
                        y->color = rb_tree_black;
                        x->parent->parent->color = rb_tree_red;
                        x = x->parent->parent;  // 将整体看为一个新插入的红节点，while不断向上调整，直到平衡
                    } else {  // 无叔叔节点（叔叔节点为黑，则父节点为黑，插入之后不需要调整）
                        if (x == x->parent->right) {
                            x = x->parent;
                            rb_tree_rotate_left(x, root);
                        }
                        x->parent->color = rb_tree_black;
                        x->parent->parent->color = rb_tree_red;
                        rb_tree_rotate_right(x->parent->parent, root);
                    }
                } else {  // 父节点为祖父节点的右子节点
                    link_type y = x->parent->parent->left;  // 叔叔节点
                    if (y && y->color == rb_tree_red) {
                        x->parent->color = rb_tree_black;
                        y->color = rb_tree_black;
                        x->parent->parent->color = rb_tree_red;
                        x = x->parent->parent;  // 继续往上层检查
                    } else {
                        if (x == x->parent->left) {
                            x = x->parent;
                            rb_tree_rotate_right(x, root);
                        }
                        x->parent->color = rb_tree_black;
                        x->parent->parent->color = rb_tree_red;
                        rb_tree_rotate_left(x->parent->parent, root);
                    }
                }
            }
            root->color = rb_tree_black;  // 根必黑
        }

        inline link_type
        rb_tree_rebalance_for_erase(link_type __z,
                                    link_type& __root,
                                    link_type& __leftmost,
                                    link_type& __rightmost)
        {
            // link_type __root = root();
            // link_type __leftmost = leftmost();
            // link_type __rightmost = rightmost();
            link_type __y = __z;
            link_type __x = 0;
            link_type __x_parent = 0;
            if (__y->left == 0)     // __z has at most one non-null child. y == z.
                __x = __y->right;     // __x might be null.
            else
            if (__y->right == 0)  // __z has exactly one non-null child. y == z.  // z有一个子节点
                __x = __y->left;    // __x is not null.
            else {                   // __z has two non-null children.  Set __y to  // z有两个子节点
                __y = __y->right;   //   __z's successor.  __x might be null.
                while (__y->left != 0)
                    __y = __y->left;
                __x = __y->right;
            }
            if (__y != __z) {          // relink y in place of z.  y is z's successor
                __z->left->parent = __y;
                __y->left = __z->left;
                if (__y != __z->right) {
                    __x_parent = __y->parent;
                    if (__x) __x->parent = __y->parent;
                    __y->parent->left = __x;      // __y must be a child of left
                    __y->right = __z->right;
                    __z->right->parent = __y;
                }
                else
                    __x_parent = __y;
                if (__root == __z)
                    __root = __y;
                else if (__z->parent->left == __z)
                    __z->parent->left = __y;
                else
                    __z->parent->right = __y;
                __y->parent = __z->parent;
                std::swap(__y->color, __z->color);
                __y = __z;
                // __y now points to node to be actually deleted
            }
            else {                        // __y == __z
                __x_parent = __y->parent;
                if (__x) __x->parent = __y->parent;
                if (__root == __z)
                    __root = __x;
                else
                if (__z->parent->left == __z)
                    __z->parent->left = __x;
                else
                    __z->parent->right = __x;
                if (__leftmost == __z) {
                    if (__z->right == 0)        // __z->left must be null also
                        __leftmost = __z->parent;
                        // makes __leftmost == header if __z == __root
                    else
                        __leftmost = rb_tree_node::minimum(__x);
                    }
                if (__rightmost == __z) {
                    if (__z->left == 0)         // __z->right must be null also
                        __rightmost = __z->parent;
                        // makes __rightmost == header if __z == __root
                    else                      // __x == __z->left
                        __rightmost = rb_tree_node::maximum(__x);
                }
            }
            if (__y->color != rb_tree_red) {
                while (__x != __root && (__x == 0 || __x->color == rb_tree_black))
                    if (__x == __x_parent->left) {
                        link_type __w = __x_parent->right;
                        if (__w->color == rb_tree_red) {
                            __w->color = rb_tree_black;
                            __x_parent->color = rb_tree_red;
                            rb_tree_rotate_left(__x_parent, __root);
                            __w = __x_parent->right;
                        }
                        if ((__w->left == 0 ||
                             __w->left->color == rb_tree_black) &&
                            (__w->right == 0 ||
                             __w->right->color == rb_tree_black)) {
                            __w->color = rb_tree_red;
                            __x = __x_parent;
                            __x_parent = __x_parent->parent;
                        } else {
                            if (__w->right == 0 ||
                                __w->right->color == rb_tree_black) {
                                if (__w->left) __w->left->color = rb_tree_black;
                                __w->color = rb_tree_red;
                                rb_tree_rotate_right(__w, __root);
                                __w = __x_parent->right;
                            }
                            __w->color = __x_parent->color;
                            __x_parent->color = rb_tree_black;
                            if (__w->right) __w->right->color = rb_tree_black;
                            rb_tree_rotate_left(__x_parent, __root);
                            break;
                        }
                    } else {                  // same as above, with right <-> left.
                        link_type __w = __x_parent->left;
                        if (__w->color == rb_tree_red) {
                            __w->color = rb_tree_black;
                            __x_parent->color = rb_tree_red;
                            rb_tree_rotate_right(__x_parent, __root);
                            __w = __x_parent->left;
                        }
                        if ((__w->right == 0 ||
                             __w->right->color == rb_tree_black) &&
                            (__w->left == 0 ||
                             __w->left->color == rb_tree_black)) {
                            __w->color = rb_tree_red;
                            __x = __x_parent;
                            __x_parent = __x_parent->parent;
                        } else {
                            if (__w->left == 0 ||
                                __w->left->color == rb_tree_black) {
                                if (__w->right) __w->right->color = rb_tree_black;
                                __w->color = rb_tree_red;
                                rb_tree_rotate_left(__w, __root);
                                __w = __x_parent->left;
                            }
                            __w->color = __x_parent->color;
                            __x_parent->color = rb_tree_black;
                            if (__w->left) __w->left->color = rb_tree_black;
                            rb_tree_rotate_right(__x_parent, __root);
                            break;
                        }
                    }
                if (__x) __x->color = rb_tree_black;
            }
            return __y;
        }

        /**
         * 删除z位节点
         * 1. 删除节点是叶子节点，直接删除
         * 2. 删除节点有一个子节点，用子节点代替当前节点
         * 3. 删除节点有两个子节点，找到当前节点的前驱节点或者后继节点，来代替当前节点
         *    再删除前驱或者后继节点，可以转化为1或者2的情况
         */
        inline link_type rb_tree_erase_and_rebalance(link_type z) {
            link_type y = z;  // 真正要删除的节点
            link_type x = nullptr;  // 真正要删除节点的子节点，可能为nullptr
            link_type x_parent = nullptr;
            // link_type root = header->parent;

            if (y->left == nullptr) {
                x = y->right;  // x might be nullptr
            } else {
                if (y->right == nullptr)
                    x = y->left;  // x not nullptr, is y.left
                else{  // 左右节点都存在, 找到后继节点
                    y = y->right;
                    while (y->left != nullptr)
                        y = y->left;
                    x = y->right;
                }
            }
            // 找到了后继节点，当然这个后继节点也可能是他本身
            // 是本身的话就是1、2情况，不是本身就是情况 3，relink操作，是指针操作直接赋值有问题？？
            if (y != z) {
                z->left->parent = y;
                y->left = z->left;  // 以上两行调整z左节点父子关系
                if (y != z->right) {
                    x_parent = y->parent;  // 让x_parent始终指向y的父节点，做一个记录
                    if (x != nullptr)  // x == null, 说明y为叶子结点，没有子节点，只需记录y.parent即可
                        x->parent = y->parent;
                    y->parent->left = x;
                    y->right = z->right;
                    z->right->parent = y;  // 以上两行调整z右节点父子关系
                } else {  // y是z的右节点。调整之后y本身就是x的父节点
                    x_parent = y;
                }
                if (root() == z) // 根节点情况
                    root() = y;
                else if (z->parent->left == z)
                    z->parent->left = y;
                else
                    z->parent->right = y;
                y->parent = z->parent;  // 以上两行（if语句看成一行）调整z父节点关系。至此，z的父，右，左节点关系都被y替代
                std::swap(y->color, z->color);
                y = z;  // y 为真正要删除的节点了
            } else {  // y == z
                x_parent = y->parent;
                if (x != nullptr)
                    x->parent = y->parent;
                if (root() == z) {
                    root() = x;
                } else if (z->parent->left == x) {
                    z->parent->left = x;
                } else {
                    z->parent->right = x;
                }
                if (leftmost() == z) {
                    leftmost() = z->right == nullptr ? z->parent : rb_tree_node::minimum(x);
                    // if (z->right == nullptr)
                    //     leftmost() = z->parent;
                    // else
                    //     leftmost() = rb_tree_node::minimum(x);
                }
                if (rightmost() == z) {
                    rightmost() = z->left == nullptr ? z->parent : rb_tree_node::maximum(x);
                    // if (z->left == nullptr)
                    //     rightmost() = z->parent;
                    // else
                    //     rightmost() = rb_tree_node::maximum(x);
                }
            }
            // 此时，y 指向要删除的节点，x 为替代节点，从 x 节点开始调整。
            // 如果删除的节点为红色，树的性质没有被破坏，否则按照以下情况调整（x 为左子节点为例）：
            // case 1: 兄弟节点为红色，令父节点为红，兄弟节点为黑，进行左（右）旋，继续处理
            // case 2: 兄弟节点为黑色，且两个子节点都为黑色或 NIL，令兄弟节点为红，父节点成为当前节点，继续处理
            // case 3: 兄弟节点为黑色，左子节点为红色或 NIL，右子节点为黑色或 NIL，
            //         令兄弟节点为红，兄弟节点的左子节点为黑，以兄弟节点为支点右（左）旋，继续处理
            // case 4: 兄弟节点为黑色，右子节点为红色，令兄弟节点为父节点的颜色，父节点为黑色，兄弟节点的右子节点
            //         为黑色，以父节点为支点左（右）旋，树的性质调整完成，算法结束
            if (y->color != rb_tree_red) {
                while (x != root() && (x == nullptr || x->color == rb_tree_black))
                    if (x == x_parent->left) {  // 左子节点
                        link_type w = x_parent->right;  // 兄弟节点
                        if (w->color == rb_tree_red) {  // case1,转换为2/3/4的一种
                            w->color = rb_tree_black;
                            x_parent->color = rb_tree_red;
                            rb_tree_rotate_left(x_parent, root());
                            w = x_parent->right;  // 处理后真正的兄弟节点
                        }
                        // case2
                        if ((w->left == nullptr || w->left->color == rb_tree_black) &&
                            (w->right == nullptr || w->right->color == rb_tree_black)) {
                            w->color = rb_tree_red;
                            x = x_parent;
                            x_parent = x_parent->parent;
                        } else {
                            if (w->right == 0 || w->right->color == rb_tree_black) {  // case3
                                if (w->left != nullptr) w->left->color = rb_tree_black;
                                w->color = rb_tree_red;
                                rb_tree_rotate_right(w, root());
                                w = x_parent->right;
                            }
                            // case4
                            w->color = x_parent->color;
                            x_parent->color = rb_tree_black;
                            if (w->right != nullptr) w->right->color = rb_tree_black;
                            rb_tree_rotate_left(x_parent, root());
                            break;
                        }
                    } else {                  // same as above, with _M_right <-> _M_left.
                        link_type w = x_parent->left;
                        if (w->color == rb_tree_red) {
                            w->color = rb_tree_black;
                            x_parent->color = rb_tree_red;
                            rb_tree_rotate_right(x_parent, root());
                            w = x_parent->left;
                        }
                        if ((w->right == 0 || w->right->color == rb_tree_black) &&
                            (w->left == 0 || w->left->color == rb_tree_black)) {
                            w->color = rb_tree_red;
                            x = x_parent;
                            x_parent = x_parent->parent;
                        } else {
                            if (w->left == 0 || w->left->color == rb_tree_black) {
                                if (w->right != nullptr) w->right->color = rb_tree_black;
                                w->color = rb_tree_red;
                                rb_tree_rotate_left(w, root());
                                w = x_parent->left;
                            }
                            w->color = x_parent->color;
                            x_parent->color = rb_tree_black;
                            if (w->left != nullptr) w->left->color = rb_tree_black;
                            rb_tree_rotate_right(x_parent, root());
                            break;
                        }
                    }
                if (x != nullptr) x->color = rb_tree_black;
            }
            return y;
        }

        std::pair<iterator, iterator> equal_range(const value_type& value) {
            return std::pair<iterator, iterator>(lower_bound(value), upper_bound(value));
        }
        iterator lower_bound(const value_type& value) {
            link_type y = header;
            link_type x = root();
            while (x != nullptr) {
                if (!key_compare(key(x), value)){  // key(x) <= value
                    y = x;
                    x = left(x);
                } else  // >
                    x = right(x);
            }
            return iterator(y);
        }

        iterator upper_bound(const value_type& value) {
            link_type y = header;
            link_type x = root();
            while (x != nullptr) {
                if (key_compare(value, key(x))) {  // value > key(x)
                    y = x;
                    x = left(x);
                } else  // key(x) >= value
                    // y = x;
                    x = right(x);
            }
            return iterator(y);
        }

        void erase_without_rebalance(link_type x) {
            while (x != nullptr) {
                erase_without_rebalance(right(x));
                link_type y = left(x);
                destroy_node(x);
                x = y;
            }
        }
    };


}

















#endif //KTINYSTL_RB_TREE_H
