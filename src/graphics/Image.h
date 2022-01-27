//
// Created by Jonathan on 20.01.2022.
//

#ifndef BLITLIB_IMAGE_H
#define BLITLIB_IMAGE_H

#include <optional>
#include "../core/Buffer2D.h"
#include "../core/types.hpp"

class Image {
public:
    explicit Image(Buffer2D<u32>& data) : _data(data) {}

    Buffer2D<u32> _data;

    const u32* data() const {return _data.data();}
    u32* data() {return _data.data();};
    const Core::Vec2<std::size_t, std::size_t>& getDimensions() const {return _data.dimensions;}
    const Buffer2D<u32> &getColorMask() const {return _colorMask.value();}

    std::optional<Buffer2D<u32>> _colorMask;
    u32* mmColorMask = nullptr;

    void constructMask();
    ~Image() {free(mmColorMask);}
private:

};


#endif //BLITLIB_IMAGE_H
