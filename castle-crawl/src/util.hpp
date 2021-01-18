#ifndef CASTLECRAWL_UTIL_HPP_INCLUDED
#define CASTLECRAWL_UTIL_HPP_INCLUDED
//
// util.hpp
//
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <stdexcept>

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

namespace util
{
    inline const std::string colorToString(const sf::Color & C)
    {
        std::string str;
        str.reserve(16);

        str += '(';

        if (sf::Color::Black == C)
        {
            str += "Black";
        }
        else if (sf::Color::White == C)
        {
            str += "White";
        }
        else if (sf::Color::Red == C)
        {
            str += "Red";
        }
        else if (sf::Color::Green == C)
        {
            str += "Green";
        }
        else if (sf::Color::Blue == C)
        {
            str += "Blue";
        }
        else if (sf::Color::Yellow == C)
        {
            str += "Yellow";
        }
        else if (sf::Color::Magenta == C)
        {
            str += "Magenta";
        }
        else if (sf::Color::Cyan == C)
        {
            str += "Cyan";
        }
        else
        {
            str += std::to_string(static_cast<unsigned>(C.r));
            str += ',';
            str += std::to_string(static_cast<unsigned>(C.g));
            str += ',';
            str += std::to_string(static_cast<unsigned>(C.b));

            if (C.a != 255)
            {
                str += ',';
                str += std::to_string(static_cast<unsigned>(C.a));
            }
        }

        str += ')';

        return str;
    }
} // namespace util

//

namespace sf
{
    using Vector2s = sf::Vector2<std::size_t>;

    template <typename T>
    [[nodiscard]] bool operator<(const sf::Vector2<T> & left, const sf::Vector2<T> & right)
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
    [[nodiscard]] sf::Vector2<T>
        operator*(const sf::Vector2<T> & left, const sf::Vector2<T> & right)
    {
        return { (left.x * right.x), (left.y * right.y) };
    }

    template <typename T>
    [[nodiscard]] sf::Vector2<T>
        operator/(const sf::Vector2<T> & numerator, const sf::Vector2<T> & denominator)
    {
        return { (numerator.x / denominator.x), (numerator.y / denominator.y) };
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

    inline std::ostream & operator<<(std::ostream & os, const sf::Color & C)
    {
        os << util::colorToString(C);
        return os;
    }

    inline std::ostream & operator<<(std::ostream & os, const sf::Vertex & vert)
    {
        os << "(pos=" << vert.position << ", col=" << vert.color << ", tc=" << vert.texCoords
           << ")";

        return os;
    }
} // namespace sf

//

namespace util
{
    constexpr const std::size_t verts_per_quad{ 4 };

    // bit hacking

    // Counting High Bits
    //  Peter Wegner's Method, which was also discovered independently by Derrick Lehmer in 1964.
    //  This method goes through as many iterations as there are set bits.
    template <typename T>
    std::size_t countHighBits(T number)
    {
        static_assert(std::is_unsigned_v<T>);
        static_assert(!std::is_same_v<std::remove_cv<T>, bool>);

        std::size_t count{ 0 };
        for (; number; count++)
        {
            number &= (number - 1);
        }

        return count;
    }

    // std lib

    // requires random access but a quick way to erase back without invalidating other iters
    template <typename Container_t>
    void swapAndPop(Container_t & container, const typename Container_t::iterator & toErase)
    {
        if (container.empty())
        {
            return;
        }

        if (container.size() > 1)
        {
            std::iter_swap(toErase, (std::end(container) - 1));
        }

        container.pop_back();
    }

    // math

    constexpr float pi{ 3.1415926535897932f };

    [[nodiscard]] constexpr float degreesToRadians(const float degrees) noexcept
    {
        return (degrees * (pi / 180.0f));
    }

    [[nodiscard]] constexpr float radiansToDegrees(const float radians) noexcept
    {
        return (radians * (180.0f / pi));
    }

    constexpr float tiny{ 0.0001f };

    [[nodiscard]] inline bool isAbsTiny(const float value) noexcept
    {
        return (std::abs(value) < tiny);
    }

    //

    template <typename T>
    [[nodiscard]] sf::Rect<T> floor(const sf::Rect<T> & rect)
    {
        return { std::floor(rect.left),
                 std::floor(rect.top),
                 std::floor(rect.width),
                 std::floor(rect.height) };
    }

    template <typename T>
    [[nodiscard]] sf::Vector2<T> floor(const sf::Vector2<T> & vec)
    {
        return { std::floor(vec.x), std::floor(vec.y) };
    }

    // position, size, and center

    template <typename T>
    [[nodiscard]] sf::Vector2<T> position(const sf::Rect<T> & rect)
    {
        return { rect.left, rect.top };
    }

    template <typename T>
    [[nodiscard]] sf::Vector2f position(const T & thing)
    {
        return position(thing.getGlobalBounds());
    }

    template <typename T>
    [[nodiscard]] sf::Vector2f positionLocal(const T & thing)
    {
        return position(thing.getLocalBounds());
    }

    template <typename T>
    [[nodiscard]] T right(const sf::Rect<T> & rect)
    {
        return (rect.left + rect.width);
    }

    template <typename T>
    [[nodiscard]] float right(const T & thing)
    {
        return right(thing.getGlobalBounds());
    }

    template <typename T>
    [[nodiscard]] T bottom(const sf::Rect<T> & rect)
    {
        return (rect.top + rect.height);
    }

    template <typename T>
    [[nodiscard]] float bottom(const T & thing)
    {
        return bottom(thing.getGlobalBounds());
    }

    template <typename T>
    [[nodiscard]] sf::Vector2<T> size(const sf::Rect<T> & rect)
    {
        return { rect.width, rect.height };
    }

    template <typename T>
    [[nodiscard]] sf::Vector2f size(const T & thing)
    {
        return size(thing.getGlobalBounds());
    }

    template <typename T>
    [[nodiscard]] sf::Vector2f sizeLocal(const T & thing)
    {
        return size(thing.getLocalBounds());
    }

    template <typename T>
    [[nodiscard]] sf::Vector2<T> center(const sf::Rect<T> & rect)
    {
        return (position(rect) + (size(rect) / T(2)));
    }

    template <typename T>
    [[nodiscard]] sf::Vector2f center(const T & thing)
    {
        return center(thing.getGlobalBounds());
    }

    template <typename T>
    [[nodiscard]] sf::Vector2f centerLocal(const T & thing)
    {
        return center(thing.getLocalBounds());
    }

    template <typename T>
    void setOriginToCenter(T & thing)
    {
        thing.setOrigin(centerLocal(thing));
    }

    // sf::Text needs correction after changing the: string, scale, or characterSize
    template <typename T>
    void setOriginToPosition(T & thing)
    {
        thing.setOrigin(positionLocal(thing));
    }

    template <typename Output_t, typename Input_t>
    Output_t makeMultOf(const Input_t startingNumber, const Output_t mult)
    {
        static_assert(std::is_integral_v<Output_t>);

        Output_t result{ static_cast<Output_t>(startingNumber) };

        while ((result % mult) != 0)
        {
            ++result;
        }

        return result;
    };

    template <typename Output_t, typename Input_t>
    sf::Vector2<Output_t>
        makeVector2MultOf(const sf::Vector2<Input_t> & before, const sf::Vector2<Output_t> & mults)
    {
        static_assert(std::is_integral_v<Output_t>);
        return { makeMultOf(before.x, mults.x), makeMultOf(before.y, mults.y) };
    };

    // vetor and euclidian math

    [[nodiscard]] inline float
        dotProduct(const sf::Vector2f & left, const sf::Vector2f & right) noexcept
    {
        return static_cast<float>((left.x * right.x) + (left.y * right.y));
    }

    [[nodiscard]] inline sf::Vector2f
        difference(const sf::Vector2f & from, const sf::Vector2f & to) noexcept
    {
        return (to - from);
    }

    [[nodiscard]] inline float magnitude(const sf::Vector2f & vec) noexcept
    {
        return std::sqrtf((vec.x * vec.x) + (vec.y * vec.y));
    }

    [[nodiscard]] inline float distance(const sf::Vector2f & from, const sf::Vector2f & to) noexcept
    {
        return magnitude(to - from);
    }

    [[nodiscard]] inline sf::Vector2f normalize(
        const sf::Vector2f & vec, const sf::Vector2f & returnOnError = { 0.0f, 0.0f }) noexcept
    {
        const float mag{ magnitude(vec) };

        if (mag < tiny)
        {
            return returnOnError;
        }

        return (vec / mag);
    }

    [[nodiscard]] inline sf::Vector2f diffNormal(
        const sf::Vector2f & from,
        const sf::Vector2f & to,
        const sf::Vector2f & returnOnError = { 0.0f, 0.0f }) noexcept
    {
        return normalize(difference(from, to), returnOnError);
    }

    // degrees, assumes 0/360 degrees aims right, and positive degress turns clockwise
    [[nodiscard]] inline float angleFromVector(const sf::Vector2f & velocity)
    {
        const sf::Vector2f posDiffNormal{ normalize(velocity) };
        const float angleRadians{ std::acosf(posDiffNormal.x) };
        const float angleDegrees{ radiansToDegrees(angleRadians) };

        // vertical or Y values that are positive move down, so have to flip
        if (velocity.y < 0.0f)
        {
            return -angleDegrees;
        }
        else
        {
            return angleDegrees;
        }
    }

    // assumes 0 and 360 degrees aims right, and positive degress turns clockwise
    [[nodiscard]] inline float angleFromTo(const sf::Vector2f & from, const sf::Vector2f & to)
    {
        return angleFromVector(difference(from, to));
    }

    template <typename T, typename U = T>
    [[nodiscard]] float angleFromTo(const T & from, const U & to)
    {
        sf::Vector2f fromPos{ 0.0f, 0.0f };
        if constexpr (std::is_same_v<std::remove_cv_t<T>, sf::Vector2f>)
        {
            fromPos = from;
        }
        else
        {
            fromPos = center(from);
        }

        sf::Vector2f toPos{ 0.0f, 0.0f };
        if constexpr (std::is_same_v<std::remove_cv_t<U>, sf::Vector2f>)
        {
            toPos = to;
        }
        else
        {
            toPos = center(to);
        }

        return angleFromTo(fromPos, toPos);
    }

    template <typename T>
    void aimAtPosition(T & thing, const sf::Vector2f & pos)
    {
        thing.setRotation(angleFromTo(center(thing), pos));
    }

    template <typename T>
    void aimWithVector(T & thing, const sf::Vector2f & velocity)
    {
        thing.setRotation(angleFromVector(velocity));
    }

    // scales, offsets, and local bounds

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

    [[nodiscard]] inline sf::FloatRect
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

    [[nodiscard]] inline sf::FloatRect
        scaleRectInPlaceCopy(const sf::FloatRect & before, const float scale) noexcept
    {
        sf::FloatRect after(before);
        scaleRectInPlace(after, scale);
        return after;
    }

    // re-sizing (scaling), centering, and all while maintaining origins

    // does NOT change the shape
    template <typename T>
    void scale(T & thing, const sf::Vector2f & size, const bool willSkewToFitExactly = false)
    {
        // skip if source size is zero (or close) to avoid dividing by zero below
        const sf::FloatRect localBounds{ thing.getLocalBounds() };
        if ((localBounds.width < 1.0f) || (localBounds.height < 1.0f))
        {
            return;
        }

        const sf::Vector2f scaleExact{ (size.x / localBounds.width),
                                       (size.y / localBounds.height) };

        if (willSkewToFitExactly)
        {
            thing.setScale(scaleExact);
        }
        else
        {
            thing.setScale(scaleExact.x, scaleExact.x);

            if (thing.getGlobalBounds().height > size.y)
            {
                thing.setScale(scaleExact.y, scaleExact.y);
            }
        }

        // setOriginToPosition(thing);
    }

    template <typename T>
    void scale(T & thing, const sf::FloatRect & rect, const bool willSkewToFitExactly = false)
    {
        scale(thing, { rect.width, rect.height }, willSkewToFitExactly);
    }

    template <typename T>
    void scale(T & thing, const float newScale, const bool willSkewToFitExactly = false)
    {
        scale(thing, { newScale, newScale }, willSkewToFitExactly);
    }

    template <typename T>
    void centerInside(T & thing, const sf::FloatRect & rect)
    {
        thing.setPosition((center(rect) - (size(thing) * 0.5f)) + thing.getOrigin());
    }

    template <typename T>
    void scaleAndCenterInside(
        T & thing, const sf::FloatRect & rect, const bool willSkewToFitExactly = false)
    {
        scale(thing, rect, willSkewToFitExactly);
        centerInside(thing, rect);
    }

    template <typename T>
    void skewAndCenterInside(
        T & thing, const sf::FloatRect & rect, const bool willSkewToFitExactly = false)
    {
        skew(thing, rect, willSkewToFitExactly);
        centerInside(thing, rect);
    }

    // quad making and appending

    template <typename Container_t>
    void setupQuadVerts(
        const sf::Vector2f & pos,
        const sf::Vector2f & size,
        const std::size_t index,
        Container_t & verts,
        const sf::Color & color = sf::Color::Transparent)
    {
        // clang-format off
        verts[index + 0].position = pos;
        verts[index + 1].position = sf::Vector2f((pos.x + size.x),  pos.y          );
        verts[index + 2].position = sf::Vector2f((pos.x + size.x), (pos.y + size.y));
        verts[index + 3].position = sf::Vector2f( pos.x          , (pos.y + size.y));
        // clang-format on

        if (color != sf::Color::Transparent)
        {
            verts[index + 0].color = color;
            verts[index + 1].color = color;
            verts[index + 2].color = color;
            verts[index + 3].color = color;
        }
    }

    template <typename Container_t>
    void setupQuadVerts(
        const sf::FloatRect & rect,
        const std::size_t index,
        Container_t & verts,
        const sf::Color & color = sf::Color::Transparent)
    {
        setupQuadVerts(position(rect), size(rect), index, verts, color);
    }

    template <typename Container_t>
    void appendQuadVerts(
        const sf::Vector2f & pos,
        const sf::Vector2f & size,
        Container_t & verts,
        const sf::Color & color = sf::Color::Transparent)
    {
        std::size_t origSize{ 0 };
        if constexpr (std::is_same_v<std::remove_cv_t<Container_t>, sf::VertexArray>)
        {
            origSize = verts.getVertexCount();
        }
        else
        {
            origSize = verts.size();
        }

        verts.resize(origSize + 4);

        setupQuadVerts(pos, size, origSize, verts, color);
    }

    template <typename Container_t>
    void appendQuadVerts(
        const sf::FloatRect & rect,
        Container_t & verts,
        const sf::Color & color = sf::Color::Transparent)
    {
        appendQuadVerts(position(rect), size(rect), verts, color);
    }

    // slow running but handy debugging shapes

    [[nodiscard]] inline sf::VertexArray
        makeRectangleVerts(const sf::FloatRect & rect, const sf::Color & color = sf::Color::White)
    {
        sf::VertexArray verts(sf::Quads, 4);
        setupQuadVerts(position(rect), size(rect), 0, verts, color);
        return verts;
    }

    inline void drawRectangleVerts(
        sf::RenderTarget & target,
        const sf::FloatRect & rect,
        const sf::Color & color = sf::Color::White)
    {
        target.draw(makeRectangleVerts(rect, color));
    }

    [[nodiscard]] inline sf::RectangleShape
        makeRectangleShape(const sf::FloatRect & rect, const sf::Color & color = sf::Color::White)
    {
        sf::RectangleShape rs;
        rs.setFillColor(color);
        rs.setPosition(position(rect));
        rs.setSize(size(rect));
        return rs;
    }

    inline void drawRectangleShape(
        sf::RenderTarget & target,
        const sf::FloatRect & rect,
        const sf::Color & color = sf::Color::White)
    {
        target.draw(makeRectangleShape(rect, color));
    }

    [[nodiscard]] inline sf::CircleShape makeCircleShape(
        const sf::Vector2f & position,
        const float radius,
        const sf::Color & color = sf::Color::White,
        const std::size_t pointCount = 32)
    {
        sf::CircleShape cs;
        cs.setFillColor(color);
        cs.setPointCount(pointCount);
        cs.setRadius(radius);
        setOriginToCenter(cs);
        cs.setPosition(position);
        return cs;
    }

    inline void drawCircleShape(
        sf::RenderTarget & target,
        const sf::Vector2f & position,
        const float radius,
        const sf::Color & color = sf::Color::White,
        const std::size_t pointCount = 32)
    {
        target.draw(makeCircleShape(position, radius, color, pointCount));
    }

    [[nodiscard]] inline sf::CircleShape makeCircleShape(
        const sf::FloatRect & rect,
        const sf::Color & color = sf::Color::White,
        const std::size_t pointCount = 32)
    {
        return makeCircleShape(
            center(rect), (std::min(rect.width, rect.height) * 0.5f), color, pointCount);
    }

    inline void drawCircle(
        sf::RenderTarget & target,
        const sf::FloatRect & rect,
        const sf::Color & color = sf::Color::White)
    {
        target.draw(makeCircleShape(rect, color));
    }

    inline sf::VertexArray makeLines(
        const std::vector<sf::Vector2f> & points, const sf::Color & color = sf::Color::White)
    {
        sf::VertexArray va(sf::Lines);

        for (const sf::Vector2f & point : points)
        {
            va.append(sf::Vertex(point, color));
        }

        return va;
    }

    inline void drawlines(
        sf::RenderTarget & target,
        const std::vector<sf::Vector2f> & points,
        const sf::Color & color = sf::Color::White)
    {
        target.draw(makeLines(points, color));
    }

    inline void drawlines(
        sf::RenderTarget & target,
        const std::initializer_list<sf::Vector2f> & points,
        const sf::Color & color = sf::Color::White)
    {
        target.draw(makeLines(points, color));
    }

    // more misc sfml

    inline sf::Color colorBlend(
        const float ratio,
        const sf::Color & fromColor,
        const sf::Color & toColor,
        const bool willIgnoreAlpha = false)
    {
        if (ratio < 0.0f)
        {
            return fromColor;
        }

        if (ratio > 1.0f)
        {
            return toColor;
        }

        auto calcColorValue = [ratio](const sf::Uint8 fromVal, const sf::Uint8 toVal) {
            const float diff{ static_cast<float>(toVal) - static_cast<float>(fromVal) };
            const float finalValue{ static_cast<float>(fromVal) + (diff * ratio) };
            return static_cast<sf::Uint8>(finalValue);
        };

        sf::Color color{ toColor };
        color.r = calcColorValue(fromColor.r, toColor.r);
        color.g = calcColorValue(fromColor.g, toColor.g);
        color.b = calcColorValue(fromColor.b, toColor.b);

        if (!willIgnoreAlpha)
        {
            color.a = calcColorValue(fromColor.a, toColor.a);
        }

        return color;
    }

    inline sf::Color colorStepToward(
        const sf::Uint8 stepSize,
        const sf::Color & fromColor,
        const sf::Color & toColor,
        const bool willIgnoreAlpha = false)
    {
        if (0 == stepSize)
        {
            return fromColor;
        }

        if (255 == stepSize)
        {
            return toColor;
        }

        auto calcColorValue = [stepSize](const sf::Uint8 fromVal, const sf::Uint8 toVal) {
            if (fromVal == toVal)
            {
                return fromVal;
            }

            const int stepInt{ static_cast<int>(stepSize) };
            const int fromInt{ static_cast<int>(fromVal) };
            const int toInt{ static_cast<int>(toVal) };
            const int diff{ std::min(std::abs(toInt - fromInt), stepInt) };

            int finalValue{ fromInt };
            if (toVal > fromVal)
            {
                finalValue += diff;
            }
            else
            {
                finalValue -= diff;
            }

            return static_cast<sf::Uint8>(std::clamp(finalValue, 0, 255));
        };

        sf::Color color{ toColor };
        color.r = calcColorValue(fromColor.r, toColor.r);
        color.g = calcColorValue(fromColor.g, toColor.g);
        color.b = calcColorValue(fromColor.b, toColor.b);

        if (!willIgnoreAlpha)
        {
            color.a = calcColorValue(fromColor.a, toColor.a);
        }

        return color;
    }

    // statistics

    template <typename T>
    struct Stats
    {
        std::string toString(const std::streamsize numberWidth = 5) const
        {
            std::ostringstream ss;
            ss.imbue(std::locale("")); // this is only to put commas in the big numbers

            ss << "x" << count;
            ss << " [" << std::setw(numberWidth) << std::right << min;
            ss << ", " << std::setw(numberWidth) << std::right << static_cast<T>(avg);
            ss << ", " << std::setw(numberWidth) << std::right << max;
            ss << "] sd=" << std::setw(numberWidth) << std::left << sdv;

            return ss.str();
        }

        std::size_t count{ 0 };
        T min{ T(0) };
        T max{ T(0) };
        T sum{ T(0) };
        double avg{ 0.0 };
        double sdv{ 0.0 };
    };

    template <typename T>
    std::ostream & operator<<(std::ostream & os, const Stats<T> & stats)
    {
        os << stats.toString();
        return os;
    }

    template <typename Container_t>
    Stats<typename Container_t::value_type> makeStats(const Container_t & container)
    {
        using T = typename Container_t::value_type;

        Stats<T> stats;

        stats.count = container.size();

        stats.min = std::numeric_limits<T>::max();

        for (const T number : container)
        {
            stats.sum += number;

            if (number < stats.min)
            {
                stats.min = number;
            }

            if (number > stats.max)
            {
                stats.max = number;
            }
        }

        stats.avg = (static_cast<double>(stats.sum) / static_cast<double>(stats.count));

        if (stats.count < 2)
        {
            return stats;
        }

        double deviationSum{ 0.0 };
        for (const T number : container)
        {
            const double diff{ static_cast<double>(number) - stats.avg };
            deviationSum += (diff * diff);
        }

        stats.sdv = std::sqrt(deviationSum / static_cast<double>(stats.count));

        return stats;
    }

    // media directory finding

    struct MediaPath
    {
        explicit MediaPath(const std::filesystem::path & pathParam)
            : path(pathParam)
            , error_message(setErrorMessage(pathParam))
        {}

        bool isValid() const { return error_message.empty(); }

        static std::string setErrorMessage(const std::filesystem::path & path)
        {
            if (!std::filesystem::exists(path))
            {
                return "Directory was not found.";
            }
            else if (!std::filesystem::is_directory(path))
            {
                return "Path is not a directory.";
            }
            else
            {
                return "";
            }
        }

        std::filesystem::path path;
        std::string error_message;
    };
} // namespace util

#endif // CASTLECRAWL_UTIL_HPP_INCLUDED
