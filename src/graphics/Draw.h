//
// Created by Jonathan on 19.01.2022.
//

#ifndef BLITLIB_DRAW_H
#define BLITLIB_DRAW_H

#include <immintrin.h>

#include "../core/Buffer2D.h"
#include "../core/types.hpp"
#include "Image.h"

namespace Core::Draw {
    class Rectangle {
    public:
        Rectangle(Vec2<u32, u32> position, Vec2<u32, u32> size);

        Vec2<u32, u32> position;
        Vec2<u32, u32> size;
    };

    void ClearColor(Buffer2D<u32> &dest, const __m256i &pixel);

    void DrawRectangle(Buffer2D<u32> &dest,
                       const Vec2<u32, u32> &position,
                       const Vec2<u32, u32> &size,
                       const __m256i &color);

    void toARGB(Buffer2D<u32> &buf);
    void flipBuffer(Buffer2D<u32> dest, const Buffer2D<u32> &src);

    void CopyRectangle(Buffer2D<u32> &dest,
                       const Vec2<u32, u32> &destPosition,
                       const Buffer2D<u32> &src,
                       const Rectangle &srcRect);

    void CopyRectangleAlphaMaskBuffer(Buffer2D<u32> &dest,
                                      const Vec2<u32, u32> &destPosition,
                                      const Image &src,
                                      const Rectangle &srcRect);
}

#endif //BLITLIB_DRAW_H
