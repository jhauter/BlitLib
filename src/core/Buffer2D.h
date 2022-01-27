//
// Created by Jonathan on 19.01.2022.
//

#ifndef BLITLIB_BUFFER2D_H
#define BLITLIB_BUFFER2D_H


#include <type_traits>
#include "Slice.h"
#include "types.hpp"
#include "Vec2.h"

template<typename T> class Buffer2D {
    static_assert(std::is_arithmetic<T>::value, "T must me numeric");

public:
    Buffer2D(T* ptr, Core::Vec2<size_t, size_t> dim) : dimensions(dim), buffer({ptr, dim.x() * dim.y()}) {};
    Core::Slice<u32> buffer;
    Core::Vec2<size_t , size_t> dimensions;

    constexpr T* operator[](std::size_t idx)       { return buffer.get() + (idx * dimensions.x()); }
    constexpr const T* operator[](std::size_t idx) const {  return buffer.get() + (idx * dimensions.x()); }

    T* data() {return buffer.get();}
    const T* data() const {return buffer.get();}

    size_t size(){return buffer.size();}
    size_t size() const{return buffer.size();}
};


#endif //BLITLIB_BUFFER2D_H
