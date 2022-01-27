//
// Created by Jonathan on 18.01.2022.
//

#ifndef BLITLIB_FRAMEBUFFER_H
#define BLITLIB_FRAMEBUFFER_H

#include <span>
#include "../core/types.hpp"

class Framebuffer {
public:
    std::pair<size_t, size_t> size;
    std::span<u32> data;
};


#endif //BLITLIB_FRAMEBUFFER_H
