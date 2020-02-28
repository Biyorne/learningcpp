#ifndef SIMPLE_EFFECTS_UTIL_HPP_INCLUDED
#define SIMPLE_EFFECTS_UTIL_HPP_INCLUDED

#include "random.hpp"

#include <ostream>

#include <SFML/Graphics.hpp>

//

namespace sf
{
    using Vector2s = Vector2<std::size_t>;

    template <typename T>
    inline sf::Vector2<T> operator*(const sf::Vector2<T> & left, const sf::Vector2<T> & right)
    {
        static_assert(std::is_arithmetic_v<T>);
        return { (left.x * right.x), (left.y * right.y) };
    }

    template <typename T>
    inline sf::Vector2<T> operator/(const sf::Vector2<T> & left, const sf::Vector2<T> & right)
    {
        static_assert(std::is_arithmetic_v<T>);

        if ((!(right.x > T(0)) && !(right.x < T(0))) || (!(right.y > T(0)) && !(right.y < T(0))))
        {
            return { T(0), T(0) };
        }

        return { (left.x / right.x), (left.y / right.y) };
    }

    template <typename T>
    inline std::ostream & operator<<(std::ostream & os, const sf::Vector2<T> & vec)
    {
        static_assert(std::is_arithmetic_v<T>);

        os << '(' << vec.x << ',' << vec.y << ')';
        return os;
    }

} // namespace sf

namespace util
{

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
    inline sf::Vector2<T> position(const sf::Rect<T> & rect)
    {
        static_assert(std::is_arithmetic_v<T>);
        return { rect.left, rect.top };
    }

    template <typename T>
    inline sf::Vector2<T> size(const sf::Rect<T> & rect)
    {
        static_assert(std::is_arithmetic_v<T>);
        return { rect.width, rect.height };
    }

    template <typename T>
    inline sf::Vector2<T> center(const sf::Rect<T> & rect)
    {
        static_assert(std::is_arithmetic_v<T>);
        return { (rect.left + (rect.width / T(2))), (rect.top + (rect.height / T(2))) };
    }

    template <typename T>
    inline void setOrigin2Center(T & thing)
    {
        thing.setOrigin(
            (thing.getLocalBounds().width * 0.5f), (thing.getLocalBounds().height * 0.5f));
    }

    //

    template <typename T>
    inline T rightTriangleHyp(const sf::Vector2<T> & vec)
    {
        static_assert(std::is_arithmetic_v<T>);
        return std::sqrt((vec.x * vec.x) + (vec.y * vec.y));
    }

    //

    // Subtracts two positions
    //(Returns two numbers for the difference in both axes)
    // The order of the subtraction (to - from) ensures: from + result == to.
    //"Turns two positions into a right triangle"
    template <typename T>
    inline sf::Vector2<T> differenceFromTo(const sf::Vector2<T> & from, const sf::Vector2<T> & to)
    {
        static_assert(std::is_arithmetic_v<T>);
        return (to - from);
    }

    //...or, this is exactly the same as:
    //"Turns two positions into a vector pointing from->to and with magnitude==distance."
    template <typename T>
    inline sf::Vector2<T> vectorMake(const sf::Vector2<T> & from, const sf::Vector2<T> & to)
    {
        static_assert(std::is_arithmetic_v<T>);
        return differenceFromTo(from, to);
    }

    //

    // Calc distance between two positions
    //(Returns one number for the difference of both axes combined)
    // In the Cartesian space, from and to define a right triangle, then the answer is obvious
    template <typename T>
    inline T distanceFromTo(const sf::Vector2<T> & from, const sf::Vector2<T> & to)
    {
        static_assert(std::is_arithmetic_v<T>);
        return rightTriangleHyp(differenceFromTo(from, to));
    }

    //...or, this is exactly the same as:
    // In Cartesian space, both positions can be thought of as the corners of a right triangle
    template <typename T>
    inline T vectorMagnitudeFromTo(const sf::Vector2<T> & from, const sf::Vector2<T> & to)
    {
        static_assert(std::is_arithmetic_v<T>);
        return rightTriangleHyp(vectorMake(from, to));
    }

    //...or, this is exactly the same as:
    template <typename T>
    inline T vectorMagnitude(const sf::Vector2<T> & vec)
    {
        static_assert(std::is_arithmetic_v<T>);
        return rightTriangleHyp(vec);
    }

    //

    // Sets the magnitude of a vector without changing direction
    template <typename T>
    inline sf::Vector2<T> vectorMagnitudeOnlySet(const sf::Vector2<T> & vec, const T newMagnitude)
    {
        static_assert(std::is_arithmetic_v<T>);

        const T currentMag(vectorMagnitude(vec));

        if (currentMag > T(0))
        {
            return (vec * (newMagnitude / currentMag));
        }
        else
        {
            return {};
        }
    }

    // Sets the magnitude of a vector to 1 without changing direction (AKA Unit Vector)
    template <typename T>
    inline sf::Vector2<T> vectorNormalize(const sf::Vector2<T> & vec)
    {
        static_assert(std::is_arithmetic_v<T>);
        return vectorMagnitudeOnlySet(vec, T(1));
    }

    // Get a vector that only has the direction
    template <typename T>
    inline sf::Vector2<T>
        vectorNormalizeFromTo(const sf::Vector2<T> & from, const sf::Vector2<T> & to)
    {
        static_assert(std::is_arithmetic_v<T>);
        return vectorNormalize(vectorMake(from, to));
    }

    // Gets the direction of a vector
    template <typename T>
    inline sf::Vector2<T> vectorDirection(const sf::Vector2<T> & vec)
    {
        static_assert(std::is_arithmetic_v<T>);
        return vectorNormalize(vec);
    }

    // Get a vector that only has the direction
    template <typename T>
    inline sf::Vector2<T>
        vectorDirectionFromTo(const sf::Vector2<T> & from, const sf::Vector2<T> & to)
    {
        static_assert(std::is_arithmetic_v<T>);
        return vectorDirection(vectorMake(from, to));
    }

    // Sets the direction of a vector without changing the magnitude
    template <typename T>
    inline sf::Vector2<T> vectorDirectionOnlySetFromTo(
        const sf::Vector2<T> & vec, const sf::Vector2<T> & from, const sf::Vector2<T> & to)
    {
        static_assert(std::is_arithmetic_v<T>);
        return (vectorNormalizeFromTo(from, to) * vectorMagnitude(vec));
    }

    template <typename T>
    inline sf::Vector2<T>
        vectorMakeWithMag(const sf::Vector2<T> & from, const sf::Vector2<T> & to, const T mag)
    {
        static_assert(std::is_arithmetic_v<T>);
        return (vectorNormalizeFromTo(from, to) * mag);
    }

    // Scale

    inline float windowDiagonalLength(const sf::RenderTarget & window)
    {
        return rightTriangleHyp(sf::Vector2f(window.getSize()));
    }

    template <typename T = float>
    inline T windowSizeAvg(const sf::RenderTarget & window)
    {
        static_assert(std::is_arithmetic_v<T>);
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
        static_assert(std::is_arithmetic_v<T>);
        return { random.zeroTo(ranges.x), random.zeroTo(ranges.y) };
    }

    template <typename T>
    inline sf::Vector2<T> sfRandom(const Random & random, const sf::Rect<T> & bounds)
    {
        static_assert(std::is_arithmetic_v<T>);
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
