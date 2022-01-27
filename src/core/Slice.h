//
// Created by Jonathan on 21.01.2022.
//

#ifndef BLITLIB_SLICE_H
#define BLITLIB_SLICE_H


#include <cstddef>
namespace Core {
    template<typename T> class Slice {
        T* _ptr;
        std::size_t _begin;
        std::size_t _end;
    public:
        Slice(T* ptr, std::size_t size) : _ptr(ptr), _begin(0), _end(size) {};
        Slice(T* ptr, std::size_t begin, std::size_t end) : _ptr(ptr), _begin(begin), _end(end) {};

        constexpr T* get() {return _ptr + _begin;}
        constexpr const T* get() const {return _ptr + _begin;}

        constexpr std::size_t size() const {return _end - _begin;}
        consteval std::size_t bytes() {return (_end - _begin) * sizeof(T);}
    };

}


#endif //BLITLIB_SLICE_H
