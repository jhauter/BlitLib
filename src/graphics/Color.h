//
// Created by Jonathan on 19.01.2022.
//

#ifndef BLITLIB_COLOR_H
#define BLITLIB_COLOR_H


#include "../core/types.hpp"
constexpr u32 CreateColor(const u32& a, const u32& r, const u32& g, const u32& b) {
    return (a << 24 | r << 16 | g << 8 | b);
}

inline __m256i CreatePixel(u32 color) {
    return _mm256_set1_epi32(color);
}

#endif //BLITLIB_COLOR_H
