#ifndef KTINYSTL_HASHTABLE_H
#define KTINYSTL_HASHTABLE_H

#include "allocator.h"
#include "iterator.h"
#include "vector.h"
#include "algorithm.h"

#include <functional>


namespace stl
{
    // hashtable 的节点
    template<class Value>
    struct hashtable_node {
        hashtable_node* next;
        Value val;
    };

    template <class Key, class Value, class HashFunc, class ExtractKey, class EqualKey>
    class hashtable;

    template <class Key, class Value, class HashFunc, class ExtractKey, class EqualKey>
    struct hashtable_iterator;

    template <class Key, class Value, class HashFunc, class ExtractKey, class EqualKey>
    struct hashtable_const_iterator;

    // hashtable 的迭代器
    // ExtractKey: KeyOfValue
    template <class Key, class Value, class HashFunc, class ExtractKey, class EqualKey>
    struct hashtable_iterator {
        typedef stl::hashtable<Key, Value, HashFunc, ExtractKey, EqualKey> hashtable;
        typedef stl::hashtable_iterator<Key, Value, HashFunc, ExtractKey, EqualKey> iterator;
        typedef stl::hashtable_const_iterator<Key, Value, HashFunc, ExtractKey, EqualKey> const_iterator;
        typedef stl::hashtable_iterator<Key, Value, HashFunc, ExtractKey, EqualKey> self;
        typedef hashtable_node<Value> node;

        typedef forward_iterator_tag iterator_category;
        typedef Value value_type;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
        typedef Value& reference;
        typedef Value* pointer;

        node* cur;  // 当前指向的节点
        hashtable* ht;  // 当前的桶（bucket）的位置

        hashtable_iterator() {}
        hashtable_iterator(node* n, hashtable* tab): cur(n), ht(tab) {}

        reference operator*() const {
            return cur->val;
        }
        pointer operator->() const {
            return &(operator*());
        }

        self& operator++() {  // ++i
            node* old = cur;
            cur = cur->next;
            if (!cur) {  // 如果不存在，则去下一个bucket找
                size_type n = ht->bkt_num(old->val);
                while (!cur && ++n < ht->_bucket.size())
                    cur = ht->_bucket[n];
            }
            return *this;
        }
        self operator++(int) {
            self temp = *this;
            ++*this;
            return temp;
        }

        // TODO // 搞不明白为什么直接传引用（iterator&）不行， 只有传常量引用（const iterator&）或者传值（iterator）才不会编译报错
        bool operator==(const iterator& it) {return cur == it.cur; }
        bool operator!=(const self& it) {return cur != it.cur; }
    };


    template <class Key, class Value, class HashFunc, class ExtractKey, class EqualKey>
    struct hashtable_const_iterator {
        typedef stl::hashtable<Key, Value, HashFunc, ExtractKey, EqualKey> hashtable;
        typedef stl::hashtable_iterator<Key, Value, HashFunc, ExtractKey, EqualKey> iterator;
        typedef stl::hashtable_const_iterator<Key, Value, HashFunc, ExtractKey, EqualKey> const_iterator;
        typedef stl::hashtable_iterator<Key, Value, HashFunc, ExtractKey, EqualKey> self;
        typedef hashtable_node<Value> node;

        typedef forward_iterator_tag iterator_category;
        typedef Value value_type;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
        typedef const Value& reference;
        typedef const Value* pointer;

        const node* cur;
        const hashtable* ht;

        hashtable_const_iterator() {}
        hashtable_const_iterator(const node* n, const hashtable* tab): cur(n), ht(tab) {}
        hashtable_const_iterator(const iterator& it): cur(it.cur), ht(it.ht) {}

        reference operator*() const {return cur->val; }
        pointer operator->() const {return &(operator*()); }

        self& operator++() {
            node* old = cur;
            cur = cur->next;
            if (!cur) {  // 如果不存在，则去下一个bucket找
                size_type n = ht->bkt_num(old->val);
                while (!cur && ++n < ht->_bucket.size())
                    cur = ht->_bucket[n];
            }
            return *this;
        }

        self operator++(int) {
            self temp = *this;
            ++*this;
            return temp;
        }

        bool operator==(const_iterator& it) const {return cur == it.cur; }
        bool operator!=(const_iterator& it) const {return cur != it.cur; }
    };

    static const int num_primes = 28;
    static const unsigned long prime_list[num_primes] = {
            53ul,         97ul,         193ul,       389ul,       769ul,
            1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
            49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
            1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
            50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
            1610612741ul, 3221225473ul, 4294967291ul
    };

    inline unsigned long next_prime(unsigned long n) {
        const unsigned long* first = prime_list;
        const unsigned long* last = prime_list + num_primes;
        const unsigned long* pos = stl::lower_bound(first, last, n);

        return pos == last ? *(last - 1) : *pos;
    }

    template <class Key, class Value, class HashFunc, class ExtractKey, class EqualKey>
    class hashtable {
    public:
        typedef Key key_type;
        typedef Value value_type;
        typedef HashFunc hasher;
        typedef EqualKey key_equal;
        typedef hashtable_node<Value> node;

        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;

        typedef hashtable_iterator<Key, Value, HashFunc, ExtractKey, EqualKey> iterator;
        typedef hashtable_const_iterator<Key, Value, HashFunc, ExtractKey, EqualKey> const_iterator;
        friend struct hashtable_iterator<Key, Value, HashFunc, ExtractKey, EqualKey>;
        friend struct hashtable_const_iterator<Key, Value, HashFunc, ExtractKey, EqualKey>;
        typedef allocator<node> node_allocator;

    private:
        hasher _hash;
        key_equal _equals;
        ExtractKey _get_key;
        vector<node*> _bucket;
        size_type _num_element;

    public:
        hasher get_hash_func() const {return _hash; }
        key_type get_key_equal() const {return _equals; }

        hashtable(size_type n = 53,
                  const HashFunc& hf = std::hash<key_type>(),
                  const EqualKey& eql = std::equal_to<value_type>(),
                  const ExtractKey& ext = std::_Identity<value_type>())
        : _hash(hf), _equals(eql), _get_key(ext), _num_element(0){
            initialize_buckets(n);
        }

        explicit hashtable(const hashtable& _ht)
        : _hash(_ht._hash), _equals(_ht._equals), _get_key(_ht._get_key), _num_element(0) {
            copy_from(_ht);
        }

        hashtable& operator=(const hashtable& _ht) {
            if (&_ht != this) {
                clear();
                _hash = _ht._hash;
                _equals = _ht._equals;
                _get_key = _ht._get_key;
                copy_from(_ht);
            }
            return *this;
        }

        ~hashtable() {clear(); }

        size_type size() const {return _num_element; }
        size_type max_size() const {return size_type(-1); }
        bool empty() const {return size() == 0; }
        void swap(hashtable& _ht) {
            stl::swap(_hash, _ht._hash);
            stl::swap(_equals, _ht._equals);
            stl::swap(_get_key, _ht._get_key);
            _bucket.swap(_ht._bucket);
            stl::swap(_num_element, _ht._num_element);
        }

        iterator begin() {
            for (size_type i = 0; i < size(); ++i) {
                if (_bucket[i])
                    return iterator(_bucket[i], this);
            }
            return end();
        }
        const_iterator begin() const {
            for (size_type i = 0; i < size(); ++i) {
                if (_bucket[i])
                    return const_iterator(_bucket[i], this);
            }
            return end();
        }
        iterator end() {return iterator(0, this); }  // 最后一个元素总是0？
        const_iterator end() const {return const_iterator(0, this); }

        size_type bucket_count() const {return _bucket.size(); }
        size_type max_bucket_count() const {return prime_list[num_primes - 1]; }
        size_type elems_in_bucket(size_type n) const {
            size_type res = 0;
            for (node* cur = _bucket[n]; cur; cur = cur->next)
                ++res;
            return res;
        }

        void clear() {
            for (size_type n = 0; n < _bucket.size(); ++n) {
                node* cur = _bucket[n];
                while(cur) {
                    node* temp = cur->next;
                    destroy_node(temp);
                    cur = temp;
                }
                _bucket[n] = 0;
            }
            _num_element = 0;
        }


        std::pair<iterator, bool> insert_unique(const value_type& value) {
            resize(_num_element + 1);
            return insert_unique_noresize(value);
        }

        template<class InputIterator>
        void insert_unique(InputIterator first, InputIterator last) {
            insert_unique(first, last, iterator_category(first));
        }

        template<class InputIterator>
        void insert_unique(InputIterator first, InputIterator last, input_iterator_tag) {
            for (; first != last; ++first)
                insert_unique(*first);
        }

        template<class InputIterator>
        void insert_unique(InputIterator first, InputIterator last, forward_iterator_tag) {
            size_type n = 0;
            distance(first, last, n);
            resize(_num_element + n);
            for (; n > 0; --n, ++first)
                insert_unique_noresize(*first);
        }

        iterator insert_equal(const value_type& value) {
            resize(_num_element + 1);
            return insert_equal_noresize(value);
        }

        template<class InputIterator>
        void insert_equal(InputIterator first, InputIterator last) {
            insert_unique(first, last, iterator_category(first));
        }

        template<class InputIterator>
        void insert_equal(InputIterator first, InputIterator last, input_iterator_tag) {
            for (; first != last; ++first)
                insert_unique(*first);
        }

        template<class InputIterator>
        void insert_equal(InputIterator first, InputIterator last, forward_iterator_tag) {
            size_type n = 0;
            distance(first, last, n);
            resize(_num_element + n);
            for (; n > 0; --n, ++first)
                insert_equal_noresize(*first);
        }

        reference find_or_insert(const value_type& value) {
            resize(_num_element + 1);

            size_type n = bkt_num(value);
            node* first = _bucket[n];  // 记录桶子的第一个元素
            for (node* cur = first; cur; cur = cur->next)
                if (_equals(_get_key(value), _get_key(cur->val)))
                    return cur->val;  // 存在相等元素就返回
            node* temp = create_node(value);
            temp->next = first;
            _bucket[n] = temp;  // 不存在就在当前桶子第一个位置插入元素，并返回
            ++_num_element;
            return temp->val;

        }

        iterator find(const key_type& key) {
            size_type n = bkt_num_key(key);
            node* cur;
            for (cur = _bucket[n]; cur && !_equals(_get_key(cur->val), key); cur = cur->next)
                ;
            return iterator(cur, this);
        }
        const_iterator find(const key_type& key) const {return const_iterator(find(key)); }  // 貌似这种写法不正确且没啥用

        size_type count(const key_type& key) {  // 统计当前key对应的元素数量
            size_type n = bkt_num_key(key);
            size_type res = 0;

            for (const node* cur = _bucket[n]; cur; cur = cur->next)
                if (_equals(_get_key(cur->val), key))
                    ++res;
            return res;
        }

        std::pair<iterator, iterator> equal_range(const size_type& key) {
            typedef std::pair<iterator, iterator> pii;
            const size_type n = bkt_num_key(key);
            for (node* first = _bucket[n]; first; first = first->next) {
                if (_equals(_get_key(first->val), key)) {  // 找到相同元素
                    for (node* cur = first->next; cur; cur = cur->next) {
                        if (!_equals(_get_key(cur->val), key))
                            return pii(iterator(first, this), iterator(cur, this));
                        for (size_type m = n + 1; m < _bucket.size(); ++m) {
                            if (_bucket[m])
                                return pii(iterator(first, this), iterator(_bucket[m], this));
                        }
                    }
                }
            }
            return pii(end(), end());
        }

        std::pair<const_iterator, const_iterator> equal_range(const size_type& key) const {
            typedef std::pair<iterator, iterator> pii;
            const size_type n = bkt_num_key(key);
            for (node* first = _bucket[n]; first; first = first->next) {
                if (_equals(_get_key(first->val), key)) {  // 找到相同元素
                    for (node* cur = first->next; cur; cur = cur->next) {
                        if (!_equals(_get_key(cur->val), key))
                            return pii(const_iterator(first, this), const_iterator(cur, this));
                        for (size_type m = n + 1; m < _bucket.size(); ++m) {
                            if (_bucket[m])
                                return pii(const_iterator(first, this), const_iterator(_bucket[m], this));
                        }
                    }
                }
            }
            return pii(end(), end());
        }

        size_type erase(key_type& key) {
            const size_type n = bkt_num_key(key);
            node* first = _bucket[n];
            size_type  erased = 0;

            if (first) {
                node* cur = first;
                node* next = cur->next;
                while (next) {
                    if (_equals(_get_key(next->val), key)) {
                        cur->next = next->next;
                        destroy_node(next);
                        next = cur->next;
                        ++erased;
                        --_num_element;
                    } else {
                        cur = next;
                        next = cur->next;
                    }
                }
                // 再比较第一个
                if(_equals(_get_key(first->val), key)) {
                    _bucket[n] = first->next;
                    destroy_node(first);
                    ++erased;
                    --_num_element;
                }
            }
            return erased;
        }

        iterator erase(iterator& it) {
            iterator res = it;
            if (node* const p = it.cur) {
                const size_type n = bkt_num(p->val);
                node* cur = _bucket[n];
                if (cur == p) {  // 比较当前桶子的第一个元素
                    _bucket[n] = cur->next;
                    destroy_node(cur);
                    --_num_element;
                    if (res == end()) return end();
                    else return ++res;
                    // return res == end() ? end() : ++res;
                } else {  // 比较当前桶子后面的元素
                    node* next = cur->next;
                    while (next) {
                        if (next == p) {
                            cur->next = next->next;
                            destroy_node(next);
                            --_num_element;
                            return res == end() ? end() : ++res;
                        } else {
                            cur = next;
                            next = cur->next;
                        }
                    }
                }
            }
        }

        void erase(iterator first, iterator last) {
            for (; first != last; ++first)
                first = erase(first);
        }



        size_type bkt_num_key(const key_type& key, size_type n) const {
            return _hash(key) % n;
        }

        size_type bkt_num(const value_type& value, const size_type n) const {
            return bkt_num_key(_get_key(value), n);
        }

        size_type bkt_num_key(const key_type& key) const {
            return bkt_num_key(key, _bucket.size());
        }

        size_type bkt_num(const value_type& value) const {
            return bkt_num_key(_get_key(value));
        }


    private:
        node* get_node() {return node_allocator::allocate(); }  // 申请一个节点空间
        void put_node(node* p) {node_allocator::deallocate(p); }  // 释放一个节点空间
        node* create_node(const value_type& val) {  // 申请并构造一个节点
            node* n = get_node();
            n->next = nullptr;
            try {
                construct(&n->val, val);
                return n;
            } catch (...) {
                put_node(n);
            }
            return n;
        }
        void destroy_node(node* n) {  // 析构并释放一个节点
            destroy(&n->val);
            put_node(n);
        }

        size_type next_size(size_type n) const {return next_prime(n); }

        void initialize_buckets(size_type n) {
            const size_type n_buckets = next_size(n);
            _bucket.reserve(n_buckets);
            _bucket.insert(_bucket.end(), n_buckets, nullptr);
            _num_element = 0;
        }

        void resize(size_type num_element_hint) {
            // 如果元素数量大于最大容量，就重建。这是一个经验
            if (num_element_hint > _bucket.size()) {
                const size_type n = next_size(num_element_hint);
                if (n > _bucket.size()) {  // 貌似是多余的判断
                    vector<node*> temp(n, nullptr);  // 新的bucket
                    try {
                        for (size_type i = 0; i < _bucket.size(); ++i) {  // 按照effective STL的经验应该使用for_each
                            node* first = _bucket[i];  // 指向当前bucket的起始节点
                            while (first) {
                                // 找出当前节点落在哪一个新bucket内
                                size_type new_bucket = bkt_num(first->val, n);

                                _bucket[i] = first->next;  // 起到一个迭代临时变量的作用
                                first->next = temp[new_bucket];
                                temp[new_bucket] = first;  // 当前节点一直作为新bucket的第一个节点例如：原始为|->1->2, 插入3变为|->3->1->2, 挺巧妙
                                first = _bucket[i];
                            }
                        }
                        _bucket.swap(temp);
                    } catch (...) {
                        for (size_type i = 0; i < temp.size(); ++i) {
                            while (temp[i]) {
                                node* next = temp[i]->next;
                                destroy_node(temp[i]);
                                temp[i] = next;
                            }
                        }
                    }
                }
            }
        }

        std::pair<iterator, bool> insert_unique_noresize(const value_type& value) {
            const size_type n = bkt_num(value);
            node* first = _bucket[n];

            for (node* cur = first; cur; cur = cur->next) {
                if (_equals(_get_key(cur->val), _get_key(value)))  // 键值相同
                    return std::pair<iterator, bool>(iterator(cur, this), false);
            }
            node* temp = create_node(value);
            temp->next = first;
            _bucket[n] = temp;  // 令新节点称为当前桶子的第一个节点
            ++_num_element;
            return std::pair<iterator, bool>(iterator(temp, this), true);
        }

        iterator insert_equal_noresize(const value_type& value) {
            const size_type n = bkt_num(value);
            node* first = _bucket[n];

            for (node* cur = first; cur; cur = cur->next) {
                if (_equals(_get_key(cur->val), _get_key(value))) {  // 键值相同
                    node* temp = create_node(value);
                    temp->next = cur->next;  // 将新节点插入当前节点之后
                    cur->next = temp;
                    ++_num_element;
                    return iterator(temp, this);
                }
            }
            node* temp = create_node(value);
            temp->next = first;
            _bucket[n] = temp;  // 令新节点称为当前桶子的第一个节点
            ++_num_element;
            return iterator(temp, this);
        }

        void copy_from(const hashtable& _ht) {
            _bucket.clear();
            _bucket.reserve(_ht._bucket.size());
            _bucket.insert(_bucket.end(), _ht._bucket.size(), nullptr);
            try {
                for (size_type i = 0; i < _ht._bucket.size(); ++i) {
                    const node* cur = _ht._bucket[i];
                    if (cur) {
                        node* temp = create_node(cur->val);
                        _bucket[i] = temp;

                        for (node* next = cur->next; next; next = next->next) {
                            temp->next = create_node(next->val);
                            temp = temp->next;
                        }
                    }
                }
                _num_element = _ht._num_element;
            } catch (...) {
                clear();
            }
        }
    };



    template <class _Key> struct hash { };

    inline size_t __stl_hash_string(const char* __s)
    {
        unsigned long __h = 0;
        for ( ; *__s; ++__s)
            __h = 5*__h + *__s;

        return size_t(__h);
    }

    template<>
    struct hash<char*>
    {
        size_t operator()(const char* __s) const { return __stl_hash_string(__s); }
    };

    template<>
    struct hash<const char*>
    {
        size_t operator()(const char* __s) const { return __stl_hash_string(__s); }
    };

    template<>
    struct hash<char> {
        size_t operator()(char __x) const { return __x; }
    };

    template<>
    struct hash<unsigned char> {
        size_t operator()(unsigned char __x) const { return __x; }
    };

    template<>
    struct hash<signed char> {
        size_t operator()(unsigned char __x) const { return __x; }
    };

    template<>
    struct hash<short> {
        size_t operator()(short __x) const { return __x; }
    };

    template<>
    struct hash<unsigned short> {
        size_t operator()(unsigned short __x) const { return __x; }
    };

    template<>
    struct hash<int> {
        size_t operator()(int __x) const { return __x; }
    };

    template<>
    struct hash<unsigned int> {
        size_t operator()(unsigned int __x) const { return __x; }
    };

    template<>
    struct hash<long> {
        size_t operator()(long __x) const { return __x; }
    };

    template<>
    struct hash<unsigned long> {
        size_t operator()(unsigned long __x) const { return __x; }
    };


}















#endif //KTINYSTL_HASHTABLE_H
