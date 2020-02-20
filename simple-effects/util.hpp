#ifndef SIMPLE_EFFECTS_UTIL_HPP_INCLUDED
#define SIMPLE_EFFECTS_UTIL_HPP_INCLUDED

#include "random.hpp"

#include <ostream>

#include <SFML/Graphics.hpp>

namespace util
{

    inline std::ostream & operator<<(std::ostream & os, const sf::Vector2f & vec)
    {
        os << '(' << vec.x << ',' << vec.y << ')';
        return os;
    }

    //

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

    template <typename T>
    inline T rightTriHypotenuse(const sf::Vector2<T> & vec)
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

    inline sf::Vector2f vecScale(const sf::Vector2f & vec, const float magnitude)
    {
        return (vecNormal(vec) * magnitude);
    }

    inline sf::Vector2f vecSetDirection(
        const sf::Vector2f & vec, const sf::Vector2f & from, const sf::Vector2f & to)
    {
        return (util::vecNormal(to - from) * vecMagnitude(vec));
    }

    // Size/scale

    inline float windowDiagonalLength(const sf::RenderTarget & window)
    {
        return rightTriHypotenuse(sf::Vector2f(window.getSize()));
    }

    template <typename T = float>
    inline T windowSizeAvg(const sf::RenderTarget & window)
    {
        return static_cast<T>((window.getSize().x + window.getSize().y) / 2u);
    }

    inline void scaleTo(const float maxDimension, sf::Sprite & sprite)
    {
        if (maxDimension < 0.0f)
        {
            return;
        }

        if ((sprite.getLocalBounds().width < 1.0f) || (sprite.getLocalBounds().height < 1.0f))
        {
            return;
        }

        // Scale by width
        const float widthRatio(maxDimension / sprite.getLocalBounds().width);
        sprite.setScale(widthRatio, widthRatio);

        // Check to see if it fits, if not...
        if (sprite.getGlobalBounds().height > maxDimension)
        {
            // Scale by height
            const float heightRatio(maxDimension / sprite.getLocalBounds().height);
            sprite.setScale(heightRatio, heightRatio);
        }
    }

    // SFML Random
    template <typename T>
    inline sf::Vector2<T> sfRandom(const Random & random, const sf::Vector2<T> & ranges)
    {
        return { random.zeroTo(ranges.x), random.zeroTo(ranges.y) };
    }

    template <typename T>
    inline sf::Vector2<T> sfRandom(const Random & random, const sf::Rect<T> & bounds)
    {
        return { random.fromTo(bounds.left, right(bounds)),
                 random.fromTo(bounds.top, bottom(bounds)) };
    }

    // quick & dirty

    inline sf::RectangleShape makeRectangleShape(
        const sf::FloatRect & bounds,
        const sf::Color & color = sf::Color::White,
        const bool willFill = false)
    {
        sf::RectangleShape rs;

        if (willFill)
        {
            rs.setFillColor(color);
            rs.setOutlineColor(sf::Color::Transparent);
        }
        else
        {
            rs.setFillColor(sf::Color::Transparent);
            rs.setOutlineColor(color);
        }

        rs.setOutlineThickness(1.0f);
        rs.setPosition(bounds.left, bounds.top);
        rs.setSize(sf::Vector2f(bounds.width, bounds.height));

        return rs;
    }

} // namespace util

#endif // SIMPLE_EFFECTS_UTIL_HPP_INCLUDED
