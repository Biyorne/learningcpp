#ifndef METH_HEAD_UTIL_HPP_INCLUDED
#define METH_HEAD_UTIL_HPP_INCLUDED

#include <SFML/Graphics.hpp>

namespace methhead
{
    inline void setSpriteRegion(sf::Sprite & sprite, const sf::FloatRect & region)
    {
        sprite.setPosition(region.left, region.top);

        float widthRatio(1.0f);
        if (sprite.getLocalBounds().width > 1.0f)
        {
            widthRatio = (region.width / sprite.getLocalBounds().width);
        }

        float heightRatio(1.0f);
        if (sprite.getLocalBounds().height > 1.0f)
        {
            heightRatio = (region.height / sprite.getLocalBounds().height);
        }

        sprite.setScale(widthRatio, heightRatio);
    }
} // namespace methhead

#endif // METH_HEAD_UTIL_HPP_INCLUDED
