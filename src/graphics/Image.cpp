//
// Created by Jonathan on 20.01.2022.
//

#include <immintrin.h>
#include "Image.h"

void Image::constructMask() {
    mmColorMask = static_cast<u32 *>(malloc(_data.size() * sizeof(u32)));
    _colorMask = Buffer2D<u32> {mmColorMask, getDimensions()};

    auto shiftMask = _mm256_set1_epi8(0xff);
    auto cmpMask = _mm256_set1_epi8(0);

    const auto *ptr = data();

    for (int i = 0; i < _data.size(); i += 8) {
        auto d = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ptr + i));

        _mm256_storeu_si256(reinterpret_cast<__m256i*>(mmColorMask + i),_mm256_cmpgt_epi32(
                _mm256_and_si256(
                        _mm256_srli_epi32(d, 24), shiftMask), cmpMask));
    }
}
