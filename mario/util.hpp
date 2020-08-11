#ifndef MARIO_UTIL_HPP_INCLUDED
#define MARIO_UTIL_HPP_INCLUDED
//
// util.hpp
//
#include <cmath>
#include <cstdlib>

#include <SFML/Graphics.hpp>

//

constexpr std::size_t operator"" _st(unsigned long long number)
{
    return static_cast<std::size_t>(number);
}

constexpr std::ptrdiff_t operator"" _pd(unsigned long long number)
{
    return static_cast<std::ptrdiff_t>(number);
}

//

namespace sf
{
    using Vector2s = sf::Vector2<std::size_t>;

    template <typename T>
    bool operator<(const sf::Vector2<T> & left, const sf::Vector2<T> & right)
    {
        if (left.x != right.x)
        {
            return (left.x < right.x);
        }
        else
        {
            return (left.y < right.y);
        }
    }

    template <typename T>
    sf::Vector2<T> operator*(const sf::Vector2<T> & left, const sf::Vector2<T> & right)
    {
        return { (left.x * right.x), (left.y * right.y) };
    }

    template <typename T>
    sf::Vector2<T> operator/(const sf::Vector2<T> & numerator, const sf::Vector2<T> & denominator)
    {
        return { (numerator.x / denominator.x), (numerator.y * denominator.y) };
    }

    template <typename T>
    std::ostream & operator<<(std::ostream & os, const sf::Vector2<T> & vec)
    {
        os << '(' << vec.x << 'x' << vec.y << ')';
        return os;
    }

    template <typename T>
    std::ostream & operator<<(std::ostream & os, const sf::Rect<T> & rect)
    {
        os << '(' << rect.left << ',' << rect.top << '_' << rect.width << 'x' << rect.height << ')';
        return os;
    }
} // namespace sf

//

namespace mario
{
    namespace util
    {
        // constexpr float pi{ 3.1415926535897932f };
        //
        // constexpr float degreesToRadians(const float degrees) noexcept
        //{
        //    return (degrees * (pi / 180.0f));
        //}
        //
        // constexpr float radiansToDegrees(const float radians) noexcept
        //{
        //    return (radians * (180.0f / pi));
        //}

        template <typename T>
        const sf::Vector2<T> center(const sf::Vector2<T> & vec)
        {
            return (vec / T(2));
        }

        template <typename T>
        const sf::Vector2<T> center(const T width, const T height)
        {
            return center(sf::Vector2<T>(width, height));
        }

        template <typename T>
        const sf::Vector2<T> center(const sf::Rect<T> & rect)
        {
            return (sf::Vector2<T>(rect.left, rect.top) + center(rect.width, rect.height));
        }

        constexpr float tiny{ 0.0001f };

        inline bool isAbsTiny(const float value) noexcept { return (std::abs(value) < tiny); }

        inline float dotProduct(const sf::Vector2f & left, const sf::Vector2f & right) noexcept
        {
            return static_cast<float>((left.x * right.x) + (left.y * right.y));
        }

        inline sf::Vector2f difference(const sf::Vector2f & from, const sf::Vector2f & to) noexcept
        {
            return (to - from);
        }

        inline float magnitude(const sf::Vector2f & vec) noexcept
        {
            return std::sqrtf((vec.x * vec.x) + (vec.y * vec.y));
        }

        inline float distance(const sf::Vector2f & from, const sf::Vector2f & to) noexcept
        {
            return magnitude(to - from);
        }

        inline sf::Vector2f normalize(
            const sf::Vector2f & vec, const sf::Vector2f & returnOnError = { 0.0f, 0.0f }) noexcept
        {
            const float mag{ magnitude(vec) };

            if (mag < tiny)
            {
                return returnOnError;
            }

            return (vec / mag);
        }

        inline sf::Vector2f diffNormal(
            const sf::Vector2f & from,
            const sf::Vector2f & to,
            const sf::Vector2f & returnOnError = { 0.0f, 0.0f }) noexcept
        {
            return normalize(difference(from, to), returnOnError);
        }
    } // namespace util

    //
} // namespace mario

#endif // MARIO_UTIL_HPP_INCLUDED