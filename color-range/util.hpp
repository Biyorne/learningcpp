#ifndef COLOR_GRADS_UTIL_HPP_INCLUDED
#define COLOR_GRADS_UTIL_HPP_INCLUDED
//
// util.hpp
//
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#ifdef WIN32
using StdManip_t = std::streamsize;
#else
using StdManip_t = int;
#endif

//

constexpr std::size_t operator"" _st(unsigned long long number)
{
    return static_cast<std::size_t>(number);
}

//

namespace sf
{
    using Vector2s = Vector2<std::size_t>;

    inline bool operator<(const Color & left, const Color & right)
    {
        return (
            std::tie(left.r, left.g, left.b, left.a)
            < std::tie(right.r, right.g, right.b, right.a));
    }

    inline std::ostream & operator<<(std::ostream & os, const sf::Color & color)
    {
        std::ostringstream ss;

        ss << '(' << std::setw(3) << std::right << int(color.r) << ',' << std::setw(3) << std::right
           << int(color.g) << ',' << std::setw(3) << std::right << int(color.b);

        if (color.a < 255)
        {
            ss << ',' << int(color.a);
        }

        ss << ')';

        os << ss.str();

        return os;
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

namespace util
{

    // These functions are written here by hand because:
    //  * <cmath>'s abs() does not use templates
    //  * some of the std functions are not constexpr when they could be
    //  * some of the std functions are not noexcept when they could be
    //  * comparing reals is only needs to be simple/fast/less-accurate for games

    template <typename T>
    [[nodiscard]] T constexpr abs(const T number) noexcept
    {
        static_assert(std::is_arithmetic_v<T>);

        if constexpr (std::is_unsigned_v<T>)
        {
            return number;
        }
        else
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
    }

    //

    template <typename T>
    [[nodiscard]] constexpr T max(const T left, const T right) noexcept
    {
        static_assert(std::is_arithmetic_v<T>);

        if (left < right)
        {
            return right;
        }
        else
        {
            return left;
        }
    }

    template <typename T, typename... Ts>
    [[nodiscard]] constexpr T max(const T first, const Ts... allOthers) noexcept
    {
        return max(first, max(allOthers...));
    }

    template <typename T>
    [[nodiscard]] constexpr T min(const T left, const T right) noexcept
    {
        static_assert(std::is_arithmetic_v<T>);

        if (left < right)
        {
            return left;
        }
        else
        {
            return right;
        }
    }

    template <typename T, typename... Ts>
    [[nodiscard]] constexpr T min(const T first, const Ts... allOthers) noexcept
    {
        return min(first, min(allOthers...));
    }

    //

    // this lib is for simple/innaccurate/game/etc apps, so a simple multiple of epsilon works
    template <typename T>
    constexpr T float_compare_epsilon = (std::numeric_limits<T>::epsilon() * T(10));

    //

    template <typename T>
    [[nodiscard]] constexpr bool isRealClose(const T left, const T right) noexcept
    {
        static_assert(std::is_arithmetic_v<T>);

        if constexpr (std::is_integral_v<T>)
        {
            return (left == right);
        }
        else
        {
            const T diffAbs { abs(right - left) };

            if (diffAbs < T(1))
            {
                return !(diffAbs > float_compare_epsilon<T>);
            }
            else
            {
                return !(diffAbs > (max(abs(left), abs(right), T(1)) * float_compare_epsilon<T>));
            }
        }
    }

    template <typename T>
    [[nodiscard]] constexpr bool isRealCloseOrLess(const T number, const T comparedTo) noexcept
    {
        return ((number < comparedTo) || isRealClose(number, comparedTo));
    }

    template <typename T>
    [[nodiscard]] constexpr bool isRealCloseOrGreater(const T number, const T comparedTo) noexcept
    {
        return ((number > comparedTo) || isRealClose(number, comparedTo));
    }

    //

    template <typename T>
    [[nodiscard]] constexpr bool isRealZero(const T number)
    {
        return isRealClose(number, T(0));
    }

    template <typename T>
    [[nodiscard]] constexpr bool isRealZeroOrLess(const T number) noexcept
    {
        return ((number < T(0)) || isRealClose(number, T(0)));
    }

    template <typename T>
    [[nodiscard]] constexpr bool isRealZeroOrGreater(const T number) noexcept
    {
        return ((number > T(0)) || isRealClose(number, T(0)));
    }

    //

    template <typename T>
    [[nodiscard]] constexpr bool isRealOne(const T number)
    {
        return isRealClose(number, T(1));
    }

    template <typename T>
    [[nodiscard]] constexpr bool isRealOneOrLess(const T number) noexcept
    {
        return ((number < T(1)) || isRealClose(number, T(1)));
    }

    template <typename T>
    [[nodiscard]] constexpr bool isRealOneOrGreater(const T number) noexcept
    {
        return ((number > T(1)) || isRealClose(number, T(1)));
    }

    template <typename T, typename U = T>
    [[nodiscard]] constexpr U
        map(const T number, const T inMin, const T inMax, const U outMin, const U outMax) noexcept
    {
        if (isRealClose(inMin, inMax))
        {
            return outMax;
        }

        return (outMin + static_cast<U>(((number - inMin) * (outMax - outMin)) / (inMax - inMin)));
    }

    // assumes ratio is [0,1]
    template <typename Ratio_t, typename Number_t>
    [[nodiscard]] constexpr Number_t
        mapRatioTo(const Ratio_t ratio, const Number_t outMin, const Number_t outMax) noexcept
    {
        static_assert(std::is_arithmetic_v<Number_t>);
        static_assert(std::is_floating_point_v<Ratio_t>);

        return (
            outMin
            + static_cast<Number_t>(
                ratio * (static_cast<Ratio_t>(outMax) - static_cast<Ratio_t>(outMin))));
    }

    template <typename Number_t, typename Ratio_t = float>
    [[nodiscard]] constexpr Ratio_t
        mapToRatio(const Number_t number, const Number_t inMin, const Number_t inMax) noexcept
    {
        static_assert(std::is_floating_point_v<Ratio_t>);

        if (isRealClose(inMin, inMax))
        {
            return Ratio_t(1);
        }

        return static_cast<Ratio_t>((number - inMin) / (inMax - inMin));
    }

    //
    // sf utils
    //

    // sf::Text needs this local offset correction after changing the: string, scale, or
    // characterSize
    template <typename T>
    void localOffsetCorrection(T & text)
    {
        if constexpr (std::is_same_v<std::remove_cv_t<T>, sf::Text>)
        {
            const auto localBounds { text.getLocalBounds() };
            text.setOrigin(localBounds.left, localBounds.top);
        }
    }

    // sfml used to have a sf::Window::capture() function for this, but for some reason he
    // deprecated it and now we have to write it ourselves...
    inline sf::Image windowToImage(const sf::Window & window)
    {
        sf::Image image;

        const sf::Vector2u windowSize { window.getSize() };

        sf::Texture texture;
        if (texture.create(windowSize.x, windowSize.y))
        {
            texture.update(window);
            image = texture.copyToImage();
        }

        return image;
    }

    inline void appendQuad(
        std::vector<sf::Vertex> & verts,
        const sf::Vector2f & pos,
        const sf::Vector2f & size,
        const sf::Color & clr = sf::Color::Transparent)
    {
        verts.push_back({ pos, clr });
        verts.push_back({ { (pos.x + size.x), pos.y }, clr });
        verts.push_back({ (pos + size), clr });
        verts.push_back({ { pos.x, (pos.y + size.y) }, clr });
    }

    inline void appendQuadRect(
        std::vector<sf::Vertex> & verts,
        const sf::FloatRect & rect,
        const sf::Color & clr = sf::Color::Transparent)
    {
        appendQuad(
            verts,
            (sf::Vector2f(rect.left, rect.top) - sf::Vector2f(1.0f, 1.0f)),
            (sf::Vector2f(rect.width, rect.height) + sf::Vector2f(2.0f, 2.0f)),
            clr);
    }

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

    template <typename T>
    void scale(T & thing, const sf::Vector2f & size)
    {
        // skip if source size is zero (or close) to avoid dividing by zero below
        const sf::FloatRect localBounds { thing.getLocalBounds() };
        if ((localBounds.width < 1.0f) || (localBounds.height < 1.0f))
        {
            return;
        }

        {
            const float scaleHoriz { size.x / localBounds.width };
            thing.setScale(scaleHoriz, scaleHoriz);
        }

        if (thing.getGlobalBounds().height > size.y)
        {
            const float scaleVert { size.y / localBounds.height };
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

    //
    // Square Roots Perfect and Near Perfect
    //

    template <typename T>
    inline bool isPerfectSquare(const T number)
    {
        static_assert(std::is_integral_v<T>);

        if (number < 4)
        {
            return false;
        }

        const T squareRootFloor { static_cast<T>(std::sqrt(number)) };
        return ((squareRootFloor * squareRootFloor) == number);
    }

    template <typename T>
    inline bool isAlmostPerfectSquare(const T number)
    {
        static_assert(std::is_integral_v<T>);

        if (number < 4)
        {
            return false;
        }

        const T squareRootFloor { static_cast<T>(std::sqrt(number)) };
        return ((squareRootFloor * (squareRootFloor + 1)) == number);
    }

    //

    enum class Squares
    {
        Perfect,
        Uniform,
        Irregular
    };

    //

    template <typename T>
    inline bool isSquareEnough(const T number, const Squares allowed)
    {
        if (isPerfectSquare(number))
        {
            return true;
        }

        return ((allowed == Squares::Uniform) && isAlmostPerfectSquare(number));
    }

    // may return the number given
    template <typename T>
    inline T findPrevSquare(const T count, const Squares allowed)
    {
        T result { count };

        if (result < 4)
        {
            return 0;
        }

        while ((result >= 4) && !isSquareEnough(result, allowed))
        {
            --result;
        }

        return result;
    }

    // may return the number given
    template <typename T>
    inline T findNextSquare(const T count, const Squares allowed)
    {
        const T max { count * 100 };

        T result { count };
        while ((result < max) && !isSquareEnough(result, allowed))
        {
            ++result;
        }

        if (result >= max)
        {
            return 0;
        }

        return result;
    }

    //

    // struct SquaresFinder
    //{
    //    SquaresFinder(const int count, const Squares allowed)
    //        : prev(findPrevSquare(count, allowed))
    //        , dist_to_prev(count - prev)
    //        , next(findNextSquare(count, allowed))
    //        , dist_to_next(next - count)
    //        // clang-format off
    //        , is_valid(
    //            (count > 0)         &&
    //            (prev <= count)     &&
    //            (next >= count)     &&
    //            (prev <= next)      &&
    //            (dist_to_prev >= 0) &&
    //            (dist_to_next >= 0))
    //    // clang-format on
    //    {
    //        if (is_valid)
    //        {
    //            std::cout << "\n  count          = " << count;
    //            std::cout << "\n  squares.prev   = " << prev << ", dist=" << dist_to_prev;
    //            std::cout << "\n  squares.next   = " << next << ", dist=" << dist_to_next;
    //            std::cout << "\n  squares.valid  = " << std::boolalpha << is_valid;
    //        }
    //    }
    //
    //    // prev of zero is not always an error
    //    int prev;
    //    int dist_to_prev;
    //
    //    int next;
    //    int dist_to_next;
    //
    //    bool is_valid;
    //};

} // namespace util

#endif // COLOR_GRADS_UTIL_HPP_INCLUDED
