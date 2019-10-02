#ifndef METHHEADS_UTIL_HPP_INCLUDED
#define METHHEADS_UTIL_HPP_INCLUDED

#include <cmath>
#include <limits>

#include <SFML/Graphics/Rect.hpp> // also includes Vector2.hpp

namespace sf
{

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

} // namespace sf

//

namespace methhead
{

    enum class Mode
    {
        Normal,
        SpeedTest
    };

    template <typename T>
    [[nodiscard]] constexpr T constexprAbs(const T number) noexcept
    {
        static_assert(std::is_arithmetic_v<T> && !std::is_same_v<std::remove_cv_t<T>, bool>);

        if constexpr (std::is_signed_v<T>)
        {
            if (number < T(0))
            {
                return -number;
            }
            else
            {
                return number;
            }
        }
        else
        {
            return number;
        }
    }

    template <typename T>
    [[nodiscard]] constexpr bool
        isRealClose(const T left, const T right, const T within = std::numeric_limits<T>::epsilon())
    {
        const T absDiff(constexprAbs(right - left));

        if (right < 1.0)
        {
            return (absDiff < within);
        }
        else
        {
            return (absDiff < (right * within));
        }
    }

    template <typename T>
    void placeInBounds(T & sfThing, const sf::FloatRect & region)
    {
        const auto localBounds(sfThing.getLocalBounds());

        if ((localBounds.width < 1.0f) || (localBounds.height < 1.0f))
        {
            return;
        }

        // scale to fit inside region
        float scale(1.0f);
        if (localBounds.width > localBounds.height)
        {
            scale = (region.width / localBounds.width);
        }
        else
        {
            scale = (region.height / localBounds.height);
        }
        sfThing.setScale(scale, scale);
        sfThing.setOrigin(localBounds.left, localBounds.top);

        // position to center of cell (region)
        const sf::Vector2f finalTextSize(
            sfThing.getGlobalBounds().width, sfThing.getGlobalBounds().height);

        const sf::Vector2f regionPos(region.left, region.top);
        const sf::Vector2f regionSize(region.width, region.height);
        const sf::Vector2f regionPosCenter(regionPos + (regionSize * 0.5f));
        sfThing.setPosition(regionPosCenter - (finalTextSize * 0.5f));
    }

    inline void resizeInPlace(sf::FloatRect & rect, const float amount)
    {
        rect.left -= (amount * 0.5f);
        rect.top -= (amount * 0.5f);
        rect.width += amount;
        rect.height += amount;
    }

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

#endif // METHHEADS_UTIL_HPP_INCLUDED
