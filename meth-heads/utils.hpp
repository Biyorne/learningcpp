#ifndef METHHEADS_UTILS_HPP_INCLUDED
#define METHHEADS_UTILS_HPP_INCLUDED

#include <cmath>
#include <cstddef>
#include <iostream>
#include <limits>
#include <string>

#include <SFML/Graphics.hpp>

//

constexpr std::size_t operator"" _st(unsigned long long number)
{
    return static_cast<std::size_t>(number);
}

//
// adding some things into the sf namespace...being naughty here...
namespace sf
{
    using Vector2s = sf::Vector2<std::size_t>;

    template <typename T>
    inline bool operator<(const sf::Vector2<T> & left, const sf::Vector2<T> & right)
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

namespace methhead
{
    using BoardPos_t = sf::Vector2i;

    enum class Mode
    {
        Normal,
        SpeedTest
    };

    //

    // string utils

    inline bool startsWith(const std::string & searchIn, const std::string & searchFor)
    {
        if (searchFor.empty())
        {
            return false;
        }

        return (searchIn.find(searchFor) == 0);
    }

    // math utils

    // I actually have to write this because cmath does not use templates...
    template <typename T>
    [[nodiscard]] T constexpr simpleAbs(const T number) noexcept
    {
        if constexpr (std::is_signed_v<T>)
        {
            if (number < T(0))
            {
                return -number;
            }
        }

        return number;
    }

    template <typename T>
    [[nodiscard]] constexpr bool isRealClose(
        const T left, const T right, const T closeEnough = std::numeric_limits<T>::epsilon())
    {
        const T absDiff(simpleAbs(right - left));

        if (right < T(1))
        {
            return !(absDiff > closeEnough);
        }
        else
        {
            return !(absDiff > (right * closeEnough));
        }
    }

    // sfml utils

    // sf::Text needs correction after changing the: string, scale, or characterSize
    template <typename T>
    void localOffsetCorrection(T & text)
    {
        if constexpr (std::is_same_v<std::remove_cv_t<T>, sf::Text>)
        {
            const auto localBounds{ text.getLocalBounds() };
            text.setOrigin(localBounds.left, localBounds.top);
        }
    }

    // sfml utils to re-size (scale) any sf::FloatRect without moving it
    inline void scaleRectInPlace(sf::FloatRect & rect, const sf::Vector2f & scale) noexcept
    {
        const auto widthChange((rect.width * scale.x) - rect.width);
        rect.width += widthChange;
        rect.left -= (widthChange * 0.5f);

        const float heightChange((rect.height * scale.y) - rect.height);
        rect.height += heightChange;
        rect.top -= (heightChange * 0.5f);
    }

    inline sf::FloatRect
        scaleRectInPlaceCopy(const sf::FloatRect & before, const sf::Vector2f & scale) noexcept
    {
        sf::FloatRect after(before);
        scaleRectInPlace(after, scale);
        return after;
    }

    inline void scaleRectInPlace(sf::FloatRect & rect, const float scale) noexcept
    {
        scaleRectInPlace(rect, { scale, scale });
    }

    inline sf::FloatRect
        scaleRectInPlaceCopy(const sf::FloatRect & before, const float scale) noexcept
    {
        sf::FloatRect after(before);
        scaleRectInPlace(after, scale);
        return after;
    }

    // sfml util to re-size (scale) various sfml types

    template <typename T>
    void scale(T & thing, const sf::Vector2f & size)
    {
        // skip if source size is zero (or close) to avoid dividing by zero below
        const sf::FloatRect localBounds{ thing.getLocalBounds() };
        if ((localBounds.width < 1.0f) || (localBounds.height < 1.0f))
        {
            return;
        }

        {
            const float scaleHoriz{ size.x / localBounds.width };
            thing.setScale(scaleHoriz, scaleHoriz);
        }

        if (thing.getGlobalBounds().height > size.y)
        {
            const float scaleVert{ size.y / localBounds.height };
            thing.setScale(scaleVert, scaleVert);
        }

        localOffsetCorrection(thing);
    }

    template <typename T>
    void scale(T & thing, const sf::FloatRect & rect)
    {
        scale(thing, { rect.width, rect.height });
    }

    template <typename T>
    void center(T & thing, const sf::FloatRect & rect)
    {
        localOffsetCorrection(thing);
        const sf::Vector2f thingSize(thing.getGlobalBounds().width, thing.getGlobalBounds().height);
        const sf::Vector2f rectPos(rect.left, rect.top);
        const sf::Vector2f rectSize(rect.width, rect.height);
        const sf::Vector2f rectPosCenter(rectPos + (rectSize * 0.5f));
        thing.setPosition(rectPosCenter - (thingSize * 0.5f));
    }

    template <typename T>
    sf::Vector2<T> center(const sf::Rect<T> & rect)
    {
        return { (rect.left + (rect.width / T(2))), (rect.top + (rect.height / T(2))) };
    }

    template <typename T>
    void fit(T & thing, const sf::FloatRect & rect)
    {
        scale(thing, rect);
        center(thing, rect);
    }

    // std algorithm utils

    template <
        typename InputIterator,
        typename OutputIterator,
        typename UnaryFunction,
        typename PredicateFunction>
    OutputIterator transform_if(
        InputIterator sourceIter,
        InputIterator sourceIterLast,
        OutputIterator destIter,
        PredicateFunction willConvertAndStore,
        UnaryFunction convertFunction)
    {
        while (sourceIter != sourceIterLast)
        {
            if (willConvertAndStore(*sourceIter))
            {
                *destIter = convertFunction(*sourceIter);
                ++destIter;
            }

            ++sourceIter;
        }

        return destIter;
    }
} // namespace methhead

#endif // METHHEADS_UTILS_HPP_INCLUDED