#ifndef SIMPLE_EFFECTS_UTIL_HPP_INCLUDED
#define SIMPLE_EFFECTS_UTIL_HPP_INCLUDED

#include <SFML/Graphics.hpp>

namespace util
{
    inline float right(const sf::FloatRect & rect) { return (rect.left + rect.width); }

    inline float bottom(const sf::FloatRect & rect) { return (rect.top + rect.height); }

    inline void setOrigin2Center(sf::Sprite & sprite)
    {
        sprite.setOrigin(
            (sprite.getLocalBounds().width * 0.5f), (sprite.getLocalBounds().height * 0.5f));
    }

    // Right-triangle

    inline float rightTriHypotenuse(const sf::Vector2f & vec)
    {
        return std::sqrt((vec.x * vec.x) + (vec.y * vec.y));
    }

    // Position

    inline sf::Vector2f posDifference(const sf::Vector2f & from, const sf::Vector2f & to)
    {
        return (to - from);
    }

    inline float posDistance(const sf::Vector2f & from, const sf::Vector2f & to)
    {
        return rightTriHypotenuse(posDifference(from, to));
    }

    // Vectors

    inline float vecMagnitude(const sf::Vector2f & vec) { return rightTriHypotenuse(vec); }

    inline sf::Vector2f
        vecNormal(const sf::Vector2f & vec, const sf::Vector2f & onError = { 0.0f, 0.0f })
    {
        const float mag(vecMagnitude(vec));
        if (mag > 0.0f)
        {
            return (vec / mag);
        }
        else
        {
            return onError;
        }
    }

} // namespace util

#endif // SIMPLE_EFFECTS_UTIL_HPP_INCLUDED
