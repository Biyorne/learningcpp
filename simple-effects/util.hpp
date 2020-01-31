#ifndef SIMPLE_EFFECTS_UTIL_HPP_INCLUDED
#define SIMPLE_EFFECTS_UTIL_HPP_INCLUDED

#include <SFML/Graphics.hpp>

namespace util
{
    template <typename T>
    inline float right(const T & thing)
    {
        if constexpr (std::is_same_v<std::remove_cv_t<T>, sf::FloatRect>)
        {
            return (thing.left + thing.width);
        }
        else
        {
            return right(thing.getGlobalBounds());
        }
    }

    template <typename T>
    inline float bottom(const T & thing)
    {
        if constexpr (std::is_same_v<std::remove_cv_t<T>, sf::FloatRect>)
        {
            return (thing.top + thing.height);
        }
        else
        {
            return bottom(thing.getGlobalBounds());
        }
    }

    template <typename T>
    inline void setOrigin2Center(T & thing)
    {
        thing.setOrigin(
            (thing.getLocalBounds().width * 0.5f), (thing.getLocalBounds().height * 0.5f));
    }

    // Right-triangle

    inline float rightTriHypotenuse(const sf::Vector2f & vec)
    {
        return std::sqrt((vec.x * vec.x) + (vec.y * vec.y));
    }

    // Position

    inline sf::Vector2f posDiffV(const sf::Vector2f & from, const sf::Vector2f & to)
    {
        return (to - from);
    }

    inline float posDistance(const sf::Vector2f & from, const sf::Vector2f & to)
    {
        return rightTriHypotenuse(posDiffV(from, to));
    }

    // Cartesian Vectors

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
