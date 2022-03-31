#ifndef KTINYSTL_ALLOCATOR_H
#define KTINYSTL_ALLOCATOR_H

#include <cstddef>
#include <new>

namespace stl
{
    template<class T>
    class allocator {
    public:
        typedef T			value_type;
        typedef T*			pointer;
        typedef T&			reference;
        typedef size_t		size_type;
        typedef ptrdiff_t	difference_type;


        static pointer allocate();
        static pointer allocate(size_type n);

        static void deallocate(pointer ptr);
        static void deallocate(pointer ptr, size_type n);

        static void construct(pointer ptr);
        static void construct(pointer ptr, reference ref);

        static void destroy(pointer ptr);
        static void destroy(pointer first, pointer last);


    public:  // 使用MSCV C++必须要有下面的构造函数和析构函数，GNU的则不需要
        constexpr allocator() noexcept = default;

        constexpr allocator(const allocator&) noexcept = default;

        template <class Other>
        constexpr explicit allocator(const allocator<Other>&) noexcept {}

        ~allocator() = default;

    };

    template<class T>
    inline T* allocator<T>::allocate() {
        return static_cast<T*>(::operator new(sizeof(T)));
    }

    template<class T>
    inline T* allocator<T>::allocate(const size_type n) {
        if (n == 0)
            return nullptr;
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    template<class T>
    inline void allocator<T>::deallocate(T* ptr) {
        if (ptr == nullptr)
            return;
        ::operator delete(ptr);
    }

    template<class T>
    inline void allocator<T>::deallocate(T* ptr, const size_t) {
        if (ptr == nullptr)
            return;
        ::operator delete(ptr);
    }

    template<class T>
    inline void allocator<T>::construct(T* ptr) {
        ::new(ptr) T();
    }

    template<class T>
    inline void allocator<T>::construct(T* ptr, T& ref) {
        ::new(ptr) T(ref);
    }

    template<class T>
    inline void allocator<T>::destroy(T* ptr) {
        ptr->~T();
    }

    template<class T>
    inline void allocator<T>::destroy(T* first, T* last) {
        for (; first != last; ++first)
            destroy(&*first);
    }
}  // end of namespace stl



#endif //KTINYSTL_ALLOCATOR_H
