#ifndef KTINYSTL_TEMP_H
#define KTINYSTL_TEMP_H

iterator insert(iterator __position, const _Tp& __x) {
    _Node* __tmp = _M_create_node(__x);
    __tmp->_M_next = __position._M_node;
    __tmp->_M_prev = __position._M_node->_M_prev;
    ((_Node*) (__position._M_node->_M_prev))->_M_next = __tmp;
    __position._M_node->_M_prev = __tmp;
    return __tmp;
}


template<class _Integer>
void _M_insert_dispatch(iterator __pos, _Integer __n, _Integer __x,
                        __true_type) {
    insert(__pos, (size_type) __n, (_Tp) __x);
}

template <class _InputIterator>
void _M_insert_dispatch(iterator __pos,
                        _InputIterator __first, _InputIterator __last,
                        __false_type);

template <class _InputIterator>
void insert(iterator __pos, _InputIterator __first, _InputIterator __last) {
    typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
    _M_insert_dispatch(__pos, __first, __last, _Integral());
}

template <class _Tp, class _Alloc> template <class _InputIter>
void
list<_Tp, _Alloc>::_M_insert_dispatch(iterator __position,
                                      _InputIter __first, _InputIter __last,
                                      __false_type)
{
    for ( ; __first != __last; ++__first)
        insert(__position, *__first);
}

#endif //KTINYSTL_TEMP_H
