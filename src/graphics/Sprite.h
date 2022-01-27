//
// Created by Jonathan on 21.01.2022.
//

#ifndef BLITLIB_SPRITE_H
#define BLITLIB_SPRITE_H

#include <memory>
#include "Draw.h"

class Sprite {
public:
    Sprite(Core::Draw::Rectangle &sourceRectangle,
           std::shared_ptr<Image> image,
           std::pair<u32, u32> position = {0,0}) :
            _srcRect(sourceRectangle), _image(std::move(image)) ,_position(std::move(position)) {}

    u32 &x() {return _position.first;}
    u32 &y() {return _position.second;}

    const u32 &x() const {return _position.first;}
    const u32 &y() const {return _position.second;}

    const u32& width() const {return _srcRect.size.x();}
    const u32& height() const {return _srcRect.size.y();}

private:
    Core::Draw::Rectangle &_srcRect;
    const std::shared_ptr<Image> _image;
    std::pair<u32, u32> _position;

};


#endif //BLITLIB_SPRITE_H
