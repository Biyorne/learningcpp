#ifndef METHHEADS_UTILS_HPP_INCLUDED
#define METHHEADS_UTILS_HPP_INCLUDED

#include <cmath>
#include <limits>
#include <string>

#include <SFML/Graphics.hpp>

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

    inline bool startsWith(const std::string & searchIn, const std::string & searchFor)
    {
        if (searchFor.empty())
        {
            return false;
        }

        return (searchIn.find(searchFor) == 0);
    }

    // I actually have to write this because cmath does not use templates...
    template <typename T>
    [[nodiscard]] T constexpr simpleAbs(const T number) noexcept
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

    template <typename SfmlThing_t>
    void placeInBounds(SfmlThing_t & thing, const sf::FloatRect & bounds)
    {
        const auto localBounds(thing.getLocalBounds());

        if ((localBounds.width < 1.0f) || (localBounds.height < 1.0f))
        {
            return;
        }

        // scale to fit inside bounds
        float scale(1.0f);
        if (localBounds.width > localBounds.height)
        {
            scale = (bounds.width / localBounds.width);
        }
        else
        {
            scale = (bounds.height / localBounds.height);
        }

        thing.setScale(scale, scale);
        thing.setOrigin(localBounds.left, localBounds.top);

        // position to center of cell (bounds)
        const sf::Vector2f finalTextSize(
            thing.getGlobalBounds().width, thing.getGlobalBounds().height);

        const sf::Vector2f boundsPos(bounds.left, bounds.top);
        const sf::Vector2f boundsSize(bounds.width, bounds.height);
        const sf::Vector2f boundsPosCenter(boundsPos + (boundsSize * 0.5f));
        thing.setPosition(boundsPosCenter - (finalTextSize * 0.5f));
    }

    inline void scaleRectInPlace(sf::FloatRect & rect, const sf::Vector2f & scale)
    {
        const float widthChange((rect.width * scale.x) - rect.width);
        rect.width += widthChange;
        rect.left -= (widthChange * 0.5f);

        const float heightChange((rect.height * scale.y) - rect.height);
        rect.height += heightChange;
        rect.top -= (heightChange * 0.5f);
    }

    inline void scaleRectInPlace(sf::FloatRect & rect, const float scale)
    {
        scaleRectInPlace(rect, { scale, scale });
    }

    template <typename SfmlThing_t>
    void scaleInPlace(SfmlThing_t & thing, const sf::Vector2f & scale)
    {
        const sf::Vector2f sizeOrig(thing.getSize());
        thing.setScale(scale);
        const sf::Vector2f sizeChange(thing.getSize() - sizeOrig);

        const sf::Vector2f moveThatReCenters(sizeChange * 0.5f * -1.0f);
        thing.move(moveThatReCenters);
    }

    template <typename SfmlThing_t>
    void scaleInPlace(SfmlThing_t & thing, const float scale)
    {
        scaleInPlace(thing, { scale, scale });
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

#endif // METHHEADS_UTILS_HPP_INCLUDED
