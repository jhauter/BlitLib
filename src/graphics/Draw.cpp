//
// Created by Jonathan on 21.01.2022.
//
#include "Draw.h"

Core::Draw::Rectangle::Rectangle(Core::Vec2<u32, u32> position, Core::Vec2<u32, u32> size) : position(
        position),
                                                                                             size(size) {}

void Core::Draw::ClearColor(Buffer2D<u32> &dest,
                            const __m256i &pixel) {

    auto *ptr = dest.data();
    for (int i = 0; i < dest.size(); i += 8) {
        _mm256_store_si256(reinterpret_cast<__m256i *>(ptr + i), pixel);
    }
}

void
Core::Draw::DrawRectangle(Buffer2D<u32> &dest,
                          const Core::Vec2<u32, u32> &position,
                          const Core::Vec2<u32, u32> &size,
                          const __m256i &color) {
    auto *ptr = dest[position.y()] + position.x();

    for (u32 y = 0; y < size.y() + 1; ++y) {
        for (u32 x = 0; x < size.x(); x += 8) {
            _mm256_store_si256(reinterpret_cast<__m256i *>(ptr + x), color);
        }
        ptr = dest[position.y() + y] + position.x();
    }
}

void Core::Draw::toARGB(Buffer2D<u32> &buf) {
    u32 *ptr = buf.data();
    __m128i mask = _mm_set_epi8(15, 12, 13, 14,
                                11, 8, 9, 10,
                                7, 4, 5, 6,
                                3, 0, 1, 2);

    for (int i = 0; i < buf.size(); i += 4) {
        _mm_storeu_si128(reinterpret_cast<__m128i *>(ptr + i),
                         _mm_shuffle_epi8(_mm_loadu_si128(reinterpret_cast<const __m128i *>(ptr + i)),
                                          mask));
    }
}

void
Core::Draw::CopyRectangle(Buffer2D<u32> &dest,
                          const Vec2<u32, u32> &destPosition,
                          const Buffer2D<u32> &src,
                          const Core::Draw::Rectangle &srcRect) {
    auto *destPtr = dest[destPosition.y()] + destPosition.x();
    auto *srcPtr = src[srcRect.position.y()] + srcRect.position.x();

    for (u32 y = 0; y < srcRect.size.y(); ++y) {
        for (u32 x = 0; x < srcRect.size.x(); x += 8) {
            _mm256_storeu_si256(reinterpret_cast<__m256i *>(destPtr + x),
                                _mm256_loadu_si256(reinterpret_cast<const __m256i *>(srcPtr + x)));
        }
        destPtr = dest[destPosition.y() + y] + destPosition.x();
        srcPtr = src[srcRect.position.y() + y] + srcRect.position.x();
    }
}

void Core::Draw::CopyRectangleAlphaMaskBuffer(Buffer2D<u32> &dest,
                                              const Core::Vec2<u32, u32> &destPosition,
                                              const Image &src,
                                              const Core::Draw::Rectangle &srcRect) {

    u32 *destPtr = dest[destPosition.y()] + destPosition.x();
    const u32 *srcPtr = src._data[srcRect.position.y()] + srcRect.position.x();
    const u32 *mPtr = src.getColorMask()[srcRect.position.y()] + srcRect.position.x();

    for (u32 y = 0; y < srcRect.size.y() + 1; ++y) {
        for (u32 x = 0; x < srcRect.size.x(); x += 8) {
            _mm256_storeu_si256(reinterpret_cast<__m256i *>(destPtr + x),
                                _mm256_blendv_epi8(
                                        _mm256_loadu_si256(reinterpret_cast<const __m256i *>(destPtr + x)),
                                        _mm256_loadu_si256(reinterpret_cast<const __m256i *>(srcPtr + x)),
                                        _mm256_loadu_si256(reinterpret_cast<const __m256i *>(mPtr + x))));
        }
        destPtr = dest[destPosition.y() + y] + destPosition.x();
        srcPtr = src._data[srcRect.position.y() + y] + srcRect.position.x();
        mPtr = src.getColorMask()[srcRect.position.y() + y] + srcRect.position.x();
    }
}

void Core::Draw::flipBuffer(Buffer2D<u32> dest,
                            const Buffer2D<u32> &src) {
    auto *srcPtr = src[src.dimensions.y() - 1];
    auto *dstPtr = dest.data();

    for (int y = 0; y <= dest.dimensions.y(); y++) {
        for (int x = 0; x < src.dimensions.x(); x += 8) {
            _mm256_storeu_si256(reinterpret_cast<__m256i *>(dstPtr + x),
                                _mm256_loadu_si256(reinterpret_cast<const __m256i *>(srcPtr + x)));
        }

        dstPtr = dest[y];
        srcPtr = src[src.dimensions.y() - y] - src.dimensions.x();
    }
}
