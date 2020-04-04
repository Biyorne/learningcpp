#ifndef SIMPLE_EFFECTS_UTIL_HPP_INCLUDED
#define SIMPLE_EFFECTS_UTIL_HPP_INCLUDED

#include "random.hpp"

#include <ostream>

#include <SFML/Graphics.hpp>

//

constexpr std::size_t operator"" _st(unsigned long long number)
{
    return static_cast<std::size_t>(number);
}

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

    inline void setOriginToLocalPos(sf::Text & text)
    {
        text.setOrigin(text.getLocalBounds().left, text.getLocalBounds().top);
    }

    // With textures and sprites:
    //   WE DECIDED were the origin should be set.  (center spin sprite)
    //
    // With text:
    //  SFML DECIDED where the origin should be set, and it is: localBounds (pos)
    //  HOW to fix it:  SET THE ORIGIN to where SFML figured out it should be:
    //                  m_text.setOrigin( m_text.getLocalBounds().left, ...top)
    //  WHEN you will have to fix it:  changes to: string/character_size/font/scale
    //  Good Advice:  Pick ONE function that sets/changes the sf::Text,
    //                So you only have to write one line of code to fix this crap.

    // Call me whenever you change string/character_size/font/scale
    inline void fixSfTextPos(sf::Text & text) { setOriginToLocalPos(text); }

    //

    template <typename T>
    [[nodiscard]] inline T rightTriangleHyp(const sf::Vector2<T> & vec)
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
    [[nodiscard]] inline sf::Vector2<T>
        differenceFromTo(const sf::Vector2<T> & from, const sf::Vector2<T> & to)
    {
        static_assert(std::is_arithmetic_v<T>);
        return (to - from);
    }

    //...or, this is exactly the same as:
    //"Turns two positions into a vector pointing from->to and with magnitude==distance."
    template <typename T>
    [[nodiscard]] inline sf::Vector2<T>
        vectorMake(const sf::Vector2<T> & from, const sf::Vector2<T> & to)
    {
        static_assert(std::is_arithmetic_v<T>);
        return differenceFromTo(from, to);
    }

    //

    // Calc distance between two positions
    //(Returns one number for the difference of both axes combined)
    // In the Cartesian space, from and to define a right triangle, then the answer is obvious
    template <typename T>
    [[nodiscard]] inline T distanceFromTo(const sf::Vector2<T> & from, const sf::Vector2<T> & to)
    {
        static_assert(std::is_arithmetic_v<T>);
        return rightTriangleHyp(differenceFromTo(from, to));
    }

    //...or, this is exactly the same as:
    // In Cartesian space, both positions can be thought of as the corners of a right triangle
    template <typename T>
    [[nodiscard]] inline T
        vectorMagnitudeFromTo(const sf::Vector2<T> & from, const sf::Vector2<T> & to)
    {
        static_assert(std::is_arithmetic_v<T>);
        return rightTriangleHyp(vectorMake(from, to));
    }

    //...or, this is exactly the same as:
    template <typename T>
    [[nodiscard]] inline T vectorMagnitude(const sf::Vector2<T> & vec)
    {
        static_assert(std::is_arithmetic_v<T>);
        return rightTriangleHyp(vec);
    }

    //

    // Sets the magnitude of a vector without changing direction
    template <typename T>
    [[nodiscard]] inline sf::Vector2<T>
        vectorMagnitudeOnlySet(const sf::Vector2<T> & vec, const T newMagnitude)
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
    [[nodiscard]] inline sf::Vector2<T> vectorNormalize(const sf::Vector2<T> & vec)
    {
        static_assert(std::is_arithmetic_v<T>);
        return vectorMagnitudeOnlySet(vec, T(1));
    }

    // Get a vector that only has the direction
    template <typename T>
    [[nodiscard]] inline sf::Vector2<T>
        vectorNormalizeFromTo(const sf::Vector2<T> & from, const sf::Vector2<T> & to)
    {
        static_assert(std::is_arithmetic_v<T>);
        return vectorNormalize(vectorMake(from, to));
    }

    // Gets the direction of a vector
    template <typename T>
    [[nodiscard]] inline sf::Vector2<T> vectorDirection(const sf::Vector2<T> & vec)
    {
        static_assert(std::is_arithmetic_v<T>);
        return vectorNormalize(vec);
    }

    // Get a vector that only has the direction
    template <typename T>
    [[nodiscard]] inline sf::Vector2<T>
        vectorDirectionFromTo(const sf::Vector2<T> & from, const sf::Vector2<T> & to)
    {
        static_assert(std::is_arithmetic_v<T>);
        return vectorDirection(vectorMake(from, to));
    }

    // Sets the direction of a vector without changing the magnitude
    template <typename T>
    [[nodiscard]] inline sf::Vector2<T> vectorDirectionOnlySetFromTo(
        const sf::Vector2<T> & vec, const sf::Vector2<T> & from, const sf::Vector2<T> & to)
    {
        static_assert(std::is_arithmetic_v<T>);
        return (vectorNormalizeFromTo(from, to) * vectorMagnitude(vec));
    }

    template <typename T>
    [[nodiscard]] inline sf::Vector2<T>
        vectorMakeWithMag(const sf::Vector2<T> & from, const sf::Vector2<T> & to, const T mag)
    {
        static_assert(std::is_arithmetic_v<T>);
        return (vectorNormalizeFromTo(from, to) * mag);
    }

    // Limits the magnitude of a vector if if it exceeds a certain value
    [[nodiscard]] inline sf::Vector2f
        vectorMagnitudeLimit(const sf::Vector2f & vec, const float limit)
    {
        if (vectorMagnitude(vec) > limit)
        {
            return vectorMagnitudeOnlySet(vec, limit);
        }
        else
        {
            return vec;
        }
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

    template <typename T>
    inline void scaleTo(T & sfThing, const sf::Vector2f & bounds)
    {
        if ((bounds.x < 0.0f) || (bounds.y < 0.0f))
        {
            return;
        }

        const auto localBounds{ sfThing.getLocalBounds() };
        if ((localBounds.width < 1.0f) || (localBounds.height < 1.0f))
        {
            return;
        }

        // Scale by width
        const float widthRatio(bounds.x / localBounds.width);
        sfThing.setScale(widthRatio, widthRatio);

        // Check to see if it fits, if not...
        if (sfThing.getGlobalBounds().height > bounds.y)
        {
            // Scale by height
            const float heightRatio(bounds.y / localBounds.height);
            sfThing.setScale(heightRatio, heightRatio);
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

    // Image Tiler

    inline sf::Sprite setupSfmlTileTrick(sf::Texture & texture, const sf::RenderTarget & target)
    {
        texture.setRepeated(true);
        return sf::Sprite(texture, sf::IntRect( { 0, 0 }, sf::Vector2i(target.getSize()) ));
    }

    // This supports that sfml tile trick -just set the sf::Texture to repeat and target size.
    // If so, then there is only ONE target.draw() call.
    inline void
        tileTarget(sf::Sprite sprite, sf::RenderTarget & target, sf::RenderStates states = {})
    {
        if (!(sprite.getGlobalBounds().width > 0.0f) || !(sprite.getGlobalBounds().height > 0.0f))
        {
            return;
        }

        const sf::Vector2f size(target.getSize());
        if (!(size.x > 0.0f) || !(size.y > 0.0f))
        {
            return;
        }

        sprite.setPosition(0.0f, 0.0f);

        if (sprite.getTexture()->isRepeated())
        {
            if ((sprite.getTextureRect().width >= size.x) &&
                ((sprite.getTextureRect().height >= size.y)))
            {
                target.draw(sprite, states);
                return;
            }
        }

        while (sprite.getGlobalBounds().top < size.y)
        {
            target.draw(sprite, states);

            if (sprite.getGlobalBounds().left < size.x)
            {
                sprite.move(sprite.getGlobalBounds().width, 0.0f);
            }
            else
            {
                sprite.move(
                    (sprite.getGlobalBounds().left * -1.0f), sprite.getGlobalBounds().height);
            }
        }
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
