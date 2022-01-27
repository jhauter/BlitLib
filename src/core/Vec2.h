//
// Created by Jonathan on 21.01.2022.
//

#ifndef BLITLIB_VEC2_H
#define BLITLIB_VEC2_H

#include <type_traits>

namespace Core {
    template<typename T, typename T2> class Vec2 {
        static_assert(std::is_arithmetic<T>::value, "T must me numeric");
        static_assert(std::is_arithmetic<T2>::value, "T2 must me numeric");
    public:
        Vec2(T x, T2 y) : _x(x), _y(y) {};
        T& x() {return _x;};
        T2& y() {return _y;};

        const T& x() const {return _x;};
        const T2& y() const {return _y;};
    private:
        T _x;
        T2 _y;
    };

}


#endif //BLITLIB_VEC2_H
