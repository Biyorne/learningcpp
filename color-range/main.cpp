// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// main.cpp
//
#include "color-names.hpp"
#include "nameof.hpp"
#include "random.hpp"

#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <memory>
#include <ostream>
#include <random>
#include <sstream>
#include <string>
#include <tuple>

#include <SFML/Graphics.hpp>

//

constexpr std::size_t operator"" _st(unsigned long long number)
{
    return static_cast<std::size_t>(number);
}

//

namespace sf
{

    inline bool operator<(const Color & left, const Color & right)
    {
        return (left.toInteger() < right.toInteger());
    }

    inline std::ostream & operator<<(std::ostream & os, const sf::Color & color)
    {
        std::ostringstream ss;

        ss << '(' << int(color.r) << ',' << int(color.g) << ',' << int(color.b);

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

//

inline void scaleInPlace(sf::FloatRect & rect, const sf::Vector2f & scale) noexcept
{
    const auto widthChange((rect.width * scale.x) - rect.width);
    rect.width += widthChange;
    rect.left -= (widthChange * 0.5f);

    const float heightChange((rect.height * scale.y) - rect.height);
    rect.height += heightChange;
    rect.top -= (heightChange * 0.5f);
}

inline sf::FloatRect
    scaleInPlaceCopy(const sf::FloatRect & before, const sf::Vector2f & scale) noexcept
{
    sf::FloatRect after(before);
    scaleInPlace(after, scale);
    return after;
}

inline void scaleInPlace(sf::FloatRect & rect, const float scale) noexcept
{
    scaleInPlace(rect, { scale, scale });
}

inline sf::FloatRect scaleInPlaceCopy(const sf::FloatRect & before, const float scale) noexcept
{
    sf::FloatRect after(before);
    scaleInPlace(after, scale);
    return after;
}

//

namespace color
{

    using Diff_t = std::int32_t;

    // color value diffs

    template <typename T = Diff_t>
    [[nodiscard]] constexpr T diff(const sf::Uint8 left, const sf::Uint8 right) noexcept
    {
        return (static_cast<T>(left) - static_cast<T>(right));
    }

    template <typename T = Diff_t>
    [[nodiscard]] constexpr T diffAbs(const sf::Uint8 left, const sf::Uint8 right) noexcept
    {
        return math::abs(diff<T>(left, right));
    }

    [[nodiscard]] inline constexpr float
        diffRatio(const sf::Uint8 left, const sf::Uint8 right) noexcept
    {
        return (diffAbs<float>(left, right) / 255.0f);
    }

    // whole color diffs

    [[nodiscard]] inline Diff_t
        diffMagnitudeCount(const sf::Color & left, const sf::Color & right) noexcept
    {
        return (
            diffAbs(left.r, right.r) + diffAbs(left.g, right.g) + diffAbs(left.b, right.b)
            + diffAbs(left.a, right.a));
    }

    [[nodiscard]] inline Diff_t
        diffMagnitudeCountOpaque(const sf::Color & left, const sf::Color & right) noexcept
    {
        return (diffAbs(left.r, right.r) + diffAbs(left.g, right.g) + diffAbs(left.b, right.b));
    }

    [[nodiscard]] inline float
        diffMagnitude(const sf::Color & left, const sf::Color & right) noexcept
    {
        return (static_cast<float>(diffMagnitudeCount(left, right)) / static_cast<float>(255 * 4));
    }

    [[nodiscard]] inline float
        diffMagnitudeOpaque(const sf::Color & left, const sf::Color & right) noexcept
    {
        return (
            static_cast<float>(diffMagnitudeCountOpaque(left, right))
            / static_cast<float>(255 * 3));
    }

    [[nodiscard]] inline float diffEuclid(const sf::Color & left, const sf::Color & right) noexcept
    {
        const Diff_t diffRed { diffAbs(left.r, right.r) };
        const Diff_t diffGrn { diffAbs(left.g, right.g) };
        const Diff_t diffBlu { diffAbs(left.b, right.b) };
        const Diff_t diffAlp { diffAbs(left.a, right.a) };

        const double diff { std::sqrt(
            (diffRed * diffRed) + (diffGrn * diffGrn) + (diffBlu * diffBlu)
            + (diffAlp * diffAlp)) };

        const double ratioCorrection { std::sqrt(255 * 255 * 4) };

        return static_cast<float>(diff / ratioCorrection);
    }

    [[nodiscard]] inline float diffEuclidOpaque(sf::Color left, sf::Color right) noexcept
    {
        const Diff_t diffRed { diffAbs(left.r, right.r) };
        const Diff_t diffGrn { diffAbs(left.g, right.g) };
        const Diff_t diffBlu { diffAbs(left.b, right.b) };

        const double diff { std::sqrt(
            (diffRed * diffRed) + (diffGrn * diffGrn) + (diffBlu * diffBlu)) };

        const double ratioCorrection { std::sqrt(255 * 255 * 3) };

        return static_cast<float>(diff / ratioCorrection);
    }

    // whole color diffs (that attempt to look correct to the human eye)

    // return
    // from https://www.compuphase.com/cmetric.htm
    [[nodiscard]] inline float
        diffWeightedEuclidOpaque(const sf::Color & left, const sf::Color & right) noexcept
    {
        const Diff_t avgRed { (static_cast<Diff_t>(left.r) + static_cast<Diff_t>(right.r)) / 2 };

        const Diff_t diffRed { diff(left.r, right.r) };
        const Diff_t diffGrn { diff(left.g, right.g) };
        const Diff_t diffBlu { diff(left.b, right.b) };

        const Diff_t bitsRed { ((512 + avgRed) * diffRed * diffRed) >> 8 };
        const Diff_t bitsGreen { 4 * diffGrn * diffGrn };
        const Diff_t bitsBlue { ((767 - avgRed) * diffBlu * diffBlu) >> 8 };

        const double diff { std::sqrt(static_cast<double>(bitsRed + bitsGreen + bitsBlue)) };
        const double ratioCorrection { std::sqrt(584970.0) };

        return static_cast<float>(diff / ratioCorrection);
    }

    // blends

    [[nodiscard]] inline constexpr sf::Uint8
        blend(const float ratio, const sf::Uint8 from, const sf::Uint8 to) noexcept
    {
        return static_cast<sf::Uint8>(
            math::mapRatioTo(ratio, static_cast<float>(from), static_cast<float>(to)));
    }

    [[nodiscard]] inline sf::Color
        blend(const float ratio, const sf::Color & from, const sf::Color & to) noexcept
    {
        return sf::Color(
            blend(ratio, from.r, to.r),
            blend(ratio, from.g, to.g),
            blend(ratio, from.b, to.b),
            blend(ratio, from.a, to.a));
    }

    template <typename Container_t>
    [[nodiscard]] inline sf::Color blend(const float ratio, const Container_t & src)
    {
        const std::size_t srcSize { src.size() };

        if (0 == srcSize)
        {
            return sf::Color::Transparent;
        }

        const auto srcBegin { std::begin(src) };

        if ((1 == srcSize) || !(ratio > 0.0f))
        {
            return *srcBegin;
        }

        if (!(ratio < 1.0f))
        {
            return *(srcBegin + static_cast<std::ptrdiff_t>(srcSize - 1));
        }

        const float srcSizeMinusOneF { static_cast<float>(srcSize - 1) };
        const auto fromIndex { static_cast<std::ptrdiff_t>(ratio * srcSizeMinusOneF) };

        const float colorSpread { 1.0f / srcSizeMinusOneF };
        const float colorRatioMin { static_cast<float>(fromIndex) * colorSpread };
        const float colorRatio { (ratio - colorRatioMin) / colorSpread };

        return blend(colorRatio, *(srcBegin + fromIndex), *(srcBegin + fromIndex + 1_st));
    }

    // HSL (Hue Saturation Brightness)

    namespace hsl
    {

        struct Hsla
        {
            float hue { 0.0f }; // ratio of degrees [0,360]
            float sat { 0.0f };
            float lgt { 0.0f };
            sf::Uint8 alp { 255 }; // same as sf::Color.a (transparent=0, opaque=255)
        };

        inline std::ostream & operator<<(std::ostream & os, const Hsla & hsl)
        {
            std::ostringstream ss;

            ss << '[' << hsl.hue << ',' << hsl.sat << ',' << hsl.lgt;

            if (hsl.alp < 255)
            {
                ss << ',' << int(hsl.alp);
            }

            ss << ']';

            os << ss.str();

            return os;
        }

        inline constexpr float hueRatioCorrect(const float hueRatio) noexcept
        {
            float result { hueRatio };

            if (result < 0.0f)
            {
                result += 1.0f;
            }

            if (result > 1.0f)
            {
                result -= 1.0f;
            }

            return result;
        }

        inline constexpr sf::Uint8
            hueRatioToRgb(const float var1, const float var2, const float hueParam) noexcept
        {
            const float hue { hueRatioCorrect(hueParam) };

            float result { 0.0f };

            if ((6.0f * hue) < 1.0f)
            {
                result = (var1 + (var2 - var1) * 6.0f * hue);
            }
            else if ((2.0f * hue) < 1.0f)
            {
                result = var2;
            }
            else if ((3.0f * hue) < 2.0f)
            {
                result = (var1 + (var2 - var1) * ((2.0f / 3.0f) - hue) * 6.0f);
            }
            else
            {
                result = var1;
            }

            result *= 255.0f;

            // fix float math errors
            if ((result < 255.0f) && ((result - std::floor(result)) > 0.99f))
            {
                result = std::ceil(result);
            }

            return static_cast<sf::Uint8>(result);
        }

        inline Hsla colorToHsla(const sf::Color & color)
        {
            Hsla hsl;
            hsl.alp = color.a;

            const float redRatio { static_cast<float>(color.r) / 255.0f };
            const float grnRatio { static_cast<float>(color.g) / 255.0f };
            const float bluRatio { static_cast<float>(color.b) / 255.0f };

            const float min { math::min(redRatio, grnRatio, bluRatio) };
            const float max { math::max(redRatio, grnRatio, bluRatio) };
            const float diff { max - min };

            hsl.lgt = ((max + min) * 0.5f);

            if (math::isRealZeroOrLess(diff))
            {
                return hsl;
            }

            if (hsl.lgt < 0.5f)
            {
                hsl.sat = (diff / (max + min));
            }
            else
            {
                hsl.sat = (diff / (2.0f - max - min));
            }

            const float redDiff { (((max - redRatio) / 6.0f) + (diff / 2.0f)) / diff };
            const float grnDiff { (((max - grnRatio) / 6.0f) + (diff / 2.0f)) / diff };
            const float bluDiff { (((max - bluRatio) / 6.0f) + (diff / 2.0f)) / diff };

            if (math::isRealClose(redRatio, max))
            {
                hsl.hue = (bluDiff - grnDiff);
            }
            else if (math::isRealClose(grnRatio, max))
            {
                hsl.hue = (((1.0f / 3.0f) + redDiff) - bluDiff);
            }
            else if (math::isRealClose(bluRatio, max))
            {
                hsl.hue = (((2.0f / 3.0f) + grnDiff) - redDiff);
            }

            hsl.hue = hueRatioCorrect(hsl.hue);

            return hsl;
        }

        inline sf::Color hslToColor(const Hsla & hsl)
        {
            sf::Color color;

            if (math::isRealZero(hsl.sat))
            {
                color.r = static_cast<sf::Uint8>(hsl.lgt * 255.0f);
                color.g = static_cast<sf::Uint8>(hsl.lgt * 255.0f);
                color.b = static_cast<sf::Uint8>(hsl.lgt * 255.0f);
            }
            else
            {
                const float var2 { (hsl.lgt < 0.5f) ? (hsl.lgt * (1 + hsl.sat))
                                                    : ((hsl.lgt + hsl.sat) - (hsl.sat * hsl.lgt)) };

                const float var1 { (2 * hsl.lgt) - var2 };

                color.r = hueRatioToRgb(var1, var2, (hsl.hue + (1.0f / 3.0f)));
                color.g = hueRatioToRgb(var1, var2, hsl.hue);
                color.b = hueRatioToRgb(var1, var2, (hsl.hue - (1.0f / 3.0f)));
            }

            color.a = hsl.alp;

            return color;
        }

    } // namespace hsl

    // helper utils

    struct ColorAtRatio
    {
        sf::Color color = sf::Color::Transparent;
        float ratio = 0.0f;
    };

    template <typename Container_t>
    void normalize(Container_t & colorsAt)
    {
        // fix order mistakes
        std::stable_sort(
            std::begin(colorsAt),
            std::end(colorsAt),
            [](const ColorAtRatio & left, const ColorAtRatio & right) {
                return (left.ratio < right.ratio);
            });

        // remove colors at the same position
        colorsAt.erase(
            std::unique(
                std::begin(colorsAt),
                std::end(colorsAt),
                [&](const ColorAtRatio & left, const ColorAtRatio & right) {
                    return math::isRealClose(left.ratio, right.ratio);
                }),
            std::end(colorsAt));

        if (colorsAt.empty())
        {
            return;
        }

        // fix if first ratio is not 0.0f by offsetting
        const float firstRatio { colorsAt.front().ratio };
        if ((firstRatio < 0.0f) || (firstRatio > 0.0f))
        {
            for (ColorAtRatio & colorAt : colorsAt)
            {
                colorAt.ratio -= firstRatio;
            }

            colorsAt.front().ratio = 0.0f;
        }

        if (colorsAt.size() == 1)
        {
            return;
        }

        // fix if last ratio is not 1.0f by scaling
        const float lastRatio { colorsAt.back().ratio };
        if ((lastRatio < 1.0f) || (lastRatio > 1.0f))
        {
            for (ColorAtRatio & colorAt : colorsAt)
            {
                colorAt.ratio *= (1.0f / lastRatio);
            }

            colorsAt.back().ratio = 1.0f;
        }
    }

    template <typename Container_t>
    Container_t normalizeCopy(const Container_t & colorsAt)
    {
        Container_t cleaned { colorsAt };
        colorsAtRatioNormalize(cleaned);
        return cleaned;
    }

    template <typename Container_t>
    sf::Color ratioFromClamped(const float ratio, const Container_t & container)
    {
        const std::size_t size { container.size() };

        if (0 == size)
        {
            return sf::Color::Transparent;
        }

        if (!(ratio > 0.0f))
        {
            return *std::begin(container);
        }

        const std::size_t index { static_cast<std::size_t>(ratio * static_cast<float>(size)) };

        if (index >= size)
        {
            return *(std::begin(container) + std::ptrdiff_t(size - 1));
        }

        return *(std::begin(container) + std::ptrdiff_t(index));
    }

    template <typename Container_t>
    sf::Color ratioFromRotation(const float ratio, const Container_t & container)
    {
        return ratioFromClamped((ratio - std::floor(ratio)), container);
    }

    inline float brightnessRatio(const sf::Color & color)
    {
        const float red { static_cast<float>(color.r) };
        const float green { static_cast<float>(color.g) };
        const float blue { static_cast<float>(color.b) };

        const float redFactor { 0.299f * red * red };
        const float greenFactor { 0.587f * green * green };
        const float blueFactor { 0.114f * blue * blue };

        return std::sqrt(redFactor + greenFactor + blueFactor) / 255.0f;
    }

    inline float luminosityRatio(const sf::Color & color)
    {
        const float redRatio { static_cast<float>(color.r) / 255.0f };
        const float greenRatio { static_cast<float>(color.g) / 255.0f };
        const float blueRatio { static_cast<float>(color.b) / 255.0f };

        const float redFactor { 0.2126f * std::pow(redRatio, 2.2f) };
        const float greenFactor { 0.7152f * std::pow(greenRatio, 2.2f) };
        const float blueFactor { 0.0722f * std::pow(blueRatio, 2.2f) };

        return (redFactor + greenFactor + blueFactor);
    }

    inline float brightnessHslRatio(const sf::Color & color) { return hsl::colorToHsla(color).lgt; }

    inline sf::Color random(const Random & random, const bool willRandomizeAlpha = false)
    {
        return sf::Color(
            random.ofType<sf::Uint8>(),
            random.ofType<sf::Uint8>(),
            random.ofType<sf::Uint8>(),
            ((willRandomizeAlpha) ? random.ofType<sf::Uint8>() : 255));
    }

    inline sf::Color randomVibrant(const Random & random, const bool willRandomizeAlpha = false)
    {
        std::array<sf::Uint8, 3> values = { random.fromTo<sf::Uint8>(0, 255),
                                            random.fromTo<sf::Uint8>(0, 255),
                                            random.fromTo<sf::Uint8>(0, 255) };

        if (diff(values[0], values[1]) < 191)
        {
            const auto avg { (values[0] + values[1]) / 2 };

            if (avg < 127)
            {
                values[2] = random.fromTo<sf::Uint8>(235, 255);
            }
            else
            {
                values[2] = random.fromTo<sf::Uint8>(0, 20);
            }
        }

        random.shuffle(values);

        sf::Uint8 alpha { 255 };
        if (willRandomizeAlpha)
        {
            alpha = random.ofType<sf::Uint8>();
        }

        return sf::Color(values[0], values[1], values[2], alpha);
    }

    // blend fills (where colors are equally/linear spaced)

    template <typename OutColorIter_t>
    void blendFill(
        const OutColorIter_t dstBegin,
        const std::size_t count,
        const sf::Color & from,
        const sf::Color & to)
    {
        const float divisor { (count <= 2) ? 1.0f : static_cast<float>(count - 1) };

        for (std::size_t i(0); i < count; ++i)
        {
            *(dstBegin + std::ptrdiff_t(i)) = blend((static_cast<float>(i) / divisor), from, to);
        }
    }

    template <typename Container_t>
    void blendFill(Container_t & dst, const sf::Color & from, const sf::Color & to)
    {
        blendFill(std::begin(dst), dst.size(), from, to);
    }

    template <typename DstContainer_t, typename SrcContainer_t>
    void blendFill(DstContainer_t & dst, const SrcContainer_t & src)
    {
        if (src.empty() || dst.empty())
        {
            return;
        }

        const std::size_t srcSize { src.size() };
        const auto srcBegin { std::begin(src) };

        const float divisor { (srcSize <= 2) ? 1.0f : static_cast<float>(srcSize - 1) };

        std::vector<ColorAtRatio> colorsAt(srcSize);

        for (std::size_t i(0); i < src.size(); ++i)
        {
            colorsAt[i].color = *(srcBegin + std::ptrdiff_t(i));
            colorsAt[i].ratio = (static_cast<float>(i) / divisor);
        }

        blendFillNonLinear(dst, colorsAt);
    }

    // blend fills (where colors are NOT equally/linear spaced)

    template <typename DstContainer_t, typename SrcContainer_t>
    void blendFillNonLinear(DstContainer_t & dst, const SrcContainer_t & srcOrig)
    {
        if (dst.empty())
        {
            return;
        }

        const std::size_t dstSize { dst.size() };
        const auto dstBegin { std::begin(dst) };

        std::vector<ColorAtRatio> src { srcOrig };
        normalize(src);

        const auto srcBegin { std::begin(src) };
        const std::size_t srcSize { src.size() };

        if (src.empty())
        {
            return;
        }

        if ((1 == srcSize) || (1 == dstSize))
        {
            std::fill_n(dstBegin, dstSize, srcBegin->color);
            return;
        }

        if (2 == srcSize)
        {
            blendFill(dst, srcBegin->color, (srcBegin + 1)->color);
            return;
        }

        const std::size_t idealColorsPerBlend { 512 };
        const std::size_t idealSize { srcSize * idealColorsPerBlend };

        if (dstSize >= idealSize)
        {
            // this is the normal case where dst was big enough

            std::size_t srcIndex(0);
            std::size_t dstIndex(0);
            while ((srcIndex < (srcSize - 1)) && (dstIndex < dstSize))
            {
                const ColorAtRatio & from { *(srcBegin + std::ptrdiff_t(srcIndex)) };
                const ColorAtRatio & to { *(srcBegin + std::ptrdiff_t(srcIndex + 1)) };

                const std::size_t blendCount { static_cast<std::size_t>(
                    (to.ratio - from.ratio) * static_cast<float>(dstSize)) };

                blendFill((dstBegin + std::ptrdiff_t(dstIndex)), blendCount, from.color, to.color);

                ++srcIndex;
                dstIndex += blendCount;
            }

            *(dstBegin + std::ptrdiff_t(dstSize - 1))
                = (srcBegin + std::ptrdiff_t(srcSize - 1))->color;
        }
        else
        {
            // first: recurse and render to temp container "ideal" if dst was too quarter
            DstContainer_t ideal(idealSize);
            blendFillNonLinear(ideal, src);

            const auto idealBegin { std::begin(ideal) };

            // second: scale down the ideal to whatever size dst actually is
            for (std::size_t dstIndex(0); dstIndex < dstSize; ++dstIndex)
            {
                const float ratio { static_cast<float>(dstIndex)
                                    / static_cast<float>(dstSize - 1) };

                const std::size_t idealIndex { static_cast<std::size_t>(
                    ratio * static_cast<float>(ideal.size() - 1)) };

                *(dstBegin + std::ptrdiff_t(dstIndex)) = *(idealBegin + std::ptrdiff_t(idealIndex));
            }

            *(dstBegin + std::ptrdiff_t(dstSize - 1))
                = *(idealBegin + std::ptrdiff_t(ideal.size() - 1));
        }
    }

    //

    class BlendCache
    {
        explicit BlendCache(const std::size_t size)
            : m_colors(size)
        {}

    public:
        BlendCache(const BlendCache &) = default;
        BlendCache(BlendCache &&) = default;
        BlendCache & operator=(const BlendCache &) = default;
        BlendCache & operator=(BlendCache &&) = default;

        //

        bool isEmpty() const noexcept { return m_colors.empty(); }
        std::size_t size() const noexcept { return m_colors.size(); }

        sf::Color atRatioClamped(const float ratio) const
        {
            return ratioFromClamped(ratio, m_colors);
        }

        sf::Color atRatioRotation(const float ratio) const
        {
            return ratioFromRotation(ratio, m_colors);
        }

        sf::Color first() const
        {
            return ((isEmpty()) ? sf::Color::Transparent : m_colors.front());
        }

        sf::Color last() const { return ((isEmpty()) ? sf::Color::Transparent : m_colors.back()); }

        const std::vector<sf::Color> & vector() const noexcept { return m_colors; }

        //

        template <typename Container_t>
        static BlendCache makeFromColors(const std::size_t size, const Container_t & container)
        {
            BlendCache cache(size);
            blendFill(cache.m_colors, container);
            return cache;
        }

        template <typename Container_t>
        static BlendCache
            makeFromColorsNonLinear(const std::size_t size, const Container_t & container)
        {
            BlendCache cache(size);
            blendFillNonLinear(cache.m_colors, container);
            return cache;
        }

        template <typename Container_t>
        static BlendCache makeFromBlendCaches(const std::size_t size, const Container_t & container)
        {
            std::size_t total { 0 };
            for (const BlendCache & cache : container)
            {
                total += cache.size();
            }

            std::vector<sf::Color> colors;
            colors.reserve(total);

            for (const BlendCache & cache : container)
            {
                std::copy(
                    std::begin(cache.vector()),
                    std::end(cache.vector()),
                    std::back_inserter(cache.m_colors));
            }

            return makeFromColors(size, colors);
        }

    private:
        std::vector<sf::Color> m_colors;
    };

} // namespace color
using namespace color;

// color statistics

namespace stats
{

    namespace Row
    {

        enum Enum
        {
            VideoCard = 0,
            Blend2,
            BlendN,
            BlendFill2,
            BlendNonLinear,
            Count
        };

        inline std::string toString(const Enum row)
        {
            // clang-format off
        switch (row)
        {
            case VideoCard: { return "VideoCard"; }
            case Blend2: { return "Blend2"; }
            case BlendN: { return "BlendN"; }
            case BlendFill2: { return "BlendFill2"; }
            case BlendNonLinear: { return "BlendNonLinear"; }
            case Count: { return "row_enum_error__count"; }
            default: { return ("row_enum_error__out_of_bounds=" + std::to_string(int(row))); }
        }
            // clang-format on
        }

    } // namespace Row

    struct ColorRow
    {
        explicit ColorRow(const Row::Enum rowEnum = Row::Count)
            : row(rowEnum)
            , name(Row::toString(rowEnum))
        {}

        Row::Enum row;
        std::string name;
        float top = 0.0f;
        float bottom = 0.0f;
        unsigned middle = 0;
    };

    //

    struct IntColor
    {
        IntColor() = default;

        explicit IntColor(const sf::Color & color)
            : r(static_cast<int>(color.r))
            , g(static_cast<int>(color.g))
            , b(static_cast<int>(color.b))
        {}

        int r { 0 };
        int g { 0 };
        int b { 0 };
    };

    template <typename T>
    struct Stats
    {
        std::string toString(const std::streamsize numberWidth = 5) const
        {
            std::ostringstream ss;
            ss.imbue(std::locale("")); // this is only to put commas in the big numbers

            ss << "x" << std::setw(numberWidth + (numberWidth / 2)) << std::left << count;
            ss << " [" << std::setw(numberWidth) << std::right << min;
            ss << ", " << std::setw(numberWidth) << std::right << avg;
            ss << ", " << std::setw(numberWidth) << std::right << max;
            ss << "] (" << std::setw(numberWidth) << std::left << sdv << ")";

            return ss.str();
        }

        std::size_t count { 0 };
        T min { T(0) };
        T max { T(0) };
        T sum { T(0) };
        double avg { 0.0 };
        double sdv { 0.0 };
    };

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
        // stats.avg_diff = stats.avg;

        if (stats.count < 2)
        {
            return stats;
        }

        double deviationSum { 0.0 };
        for (const T number : container)
        {
            const double diff { static_cast<double>(number) - stats.avg };
            deviationSum += (diff * diff);
        }

        stats.sdv = std::sqrt(deviationSum / static_cast<double>(stats.count));
        // stats.sdv_diff = stats.sdv;

        return stats;
    }

    struct ColorValues
    {
        ColorValues() { values.reserve(1 << 20); }

        void reset() { values.clear(); }
        void calcStats() { stats = makeStats(values); }

        Stats<int> stats;
        std::vector<int> values;
    };

    struct ColorMakerStats
    {
        ColorMakerStats(const std::string & titleParam)
            : title(titleParam)
        {
            bright_quick.reserve(1 << 20);
            bright_full.reserve(1 << 20);
        }

        template <typename T>
        void run(T makeColorLambda)
        {
            for (std::size_t r(0); r < generation_repeat_count; ++r)
            {
                reds.reset();
                greens.reset();
                blues.reset();

                // bright_full.clear();

                for (std::size_t g(0); g < colors_per_generation; ++g)
                {
                    const sf::Color color { makeColorLambda() };
                    reds.values.push_back(color.r);
                    greens.values.push_back(color.g);
                    blues.values.push_back(color.b);

                    // bright_quick.push_back(brightnessQuick(color));
                    // bright_full.push_back(static_cast<double>(color::brightness(color)));
                }

                reds.calcStats();
                greens.calcStats();
                blues.calcStats();

                const double avg_diff_sum { (reds.stats.avg + greens.stats.avg + blues.stats.avg)
                                            / 3.0 };

                avg_diff_sums.push_back(avg_diff_sum);

                const double sdv_diff_sum { (reds.stats.sdv + greens.stats.sdv + blues.stats.sdv)
                                            / 3.0 };

                sdv_diff_sums.push_back(sdv_diff_sum);

                // std::cout << title << ": brightness=" << (100.0 * (makeStats(bright_full).avg /
                // 255.0))
                //          << "%" << std::endl;
            }

            const auto avgDiffSumStats { makeStats(avg_diff_sums) };
            const auto sdvDiffSumStats { makeStats(sdv_diff_sums) };

            std::cout << title << ":" << std::endl;
            std::cout << "  Average:  " << avgDiffSumStats.toString() << std::endl;
            std::cout << "   StdDev:  " << sdvDiffSumStats.toString() << std::endl;
        }

        std::string title;

        std::vector<double> bright_quick;
        std::vector<double> bright_full;

        ColorValues reds;
        ColorValues greens;
        ColorValues blues;

        std::vector<double> avg_diff_sums;
        std::vector<double> sdv_diff_sums;

        static inline constexpr std::size_t colors_per_generation { 1 << 14 };
        static inline constexpr std::size_t generation_repeat_count { 100 };
    };

} // namespace stats
using namespace stats;

//

namespace ColorTest
{

    enum Enum : unsigned
    {
        Blend = 0,
        Brightness,
        Diff,
        Hsl
    };

}

template <typename Container_t>
std::size_t containerIndexWrap(const std::size_t index, const Container_t & container)
{
    if (index > container.size())
    {
        return (container.size() - 1);
    }
    else if (index == container.size())
    {
        return 0;
    }
    else
    {
        return index;
    }
}

//

struct Resources
{
    Resources();

    bool isWindowOpen() const { return window.isOpen(); }
    unsigned windowWidth() const { return window.getSize().x; }
    sf::Vector2f windowSize() const { return sf::Vector2f { window.getSize() }; }
    const std::vector<sf::Color> & gradient() const { return gradients.at(horiz_index); }
    const ColorRow & colorRow(const Row::Enum row) const { return color_rows.at(row); }

    void setupWindow();
    void redraw();
    void handleEvents();
    void handleEvent(const sf::Event & event);
    void draw();
    void setupColorTest_Blend();
    void setupColorTest_Brightness();
    void setupColorTest_Diff();
    void setupColorTest_HSL();
    void reset();

    Random random;

    float rotation { 0.0f };
    float rotation_speed { 0.0f };
    const float rotation_inc { 1.0f / 1000.0f };
    ColorTest::Enum which_test { ColorTest::Blend };

private:
    void printColorRowDifferences();
    std::size_t horizIndexWrap(const std::size_t index) const;
    std::size_t vertIndexWrap(const std::size_t index) const;

    bool is_fullscreen { false };

    std::size_t blend_color_count { 0 };

    sf::RenderWindow window;
    sf::RenderTexture side_texture;

    std::vector<sf::Vertex> quad_verts;
    std::vector<sf::Vertex> line_verts;

    std::size_t horiz_index { 0 };
    std::size_t vert_index { 0 };

    std::vector<std::vector<sf::Color>> gradients;

    const sf::Color orange { 255, 165, 0 };
    const sf::Color redBrown { 200, 82, 45 };
    const sf::Color redRedBrown { 255, 82, 45 };

    std::vector<ColorRow> color_rows;

    ColorMakerStats color_stats_normal;
    ColorMakerStats color_stats_vibrant;
    ColorMakerStats color_stats_alt;
};

//

void testHsl();
void testColorDiff();

//

int main()
{
    Resources res;

    while (res.isWindowOpen())
    {
        res.handleEvents();

        // if (ColorTest::Blend == res.which_test)
        //{
        //    res.rotation += res.rotation_speed;
        //    res.setupColorTest_Blend();
        //}

        res.draw();
    }

    return EXIT_SUCCESS;
}

// Resources functions

using namespace color;

Resources::Resources()
    : color_stats_normal("Normal")
    , color_stats_vibrant("Vibrant")
    , color_stats_alt("Alternate")
{
    gradients.push_back({ sf::Color::Black, sf::Color::White });
    gradients.push_back({ sf::Color::Yellow, sf::Color::Blue });
    gradients.push_back({ sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Red });
    gradients.push_back({ redBrown, orange, sf::Color::Yellow, sf::Color::White, redBrown });

    gradients.push_back({ sf::Color::White,
                          sf::Color::Red,
                          sf::Color::Green,
                          sf::Color::Blue,
                          redBrown,
                          orange,
                          sf::Color::Yellow,
                          sf::Color::White,
                          sf::Color::Yellow,
                          sf::Color::Blue,
                          sf::Color::White });

    for (int i(0); i < Row::Count; ++i)
    {
        color_rows.push_back(ColorRow(static_cast<Row::Enum>(i)));
    }

    setupWindow();
    reset();
    redraw();
}

void Resources::reset()
{
    blend_color_count = window.getSize().x;
    horiz_index = 0;
    vert_index = 0;
    rotation = 0.0f;
    rotation_speed = 0.0f;
    quad_verts.clear();
    line_verts.clear();
}

std::size_t Resources::horizIndexWrap(const std::size_t index) const
{
    // clang-format off
    switch (which_test)
    {
        case ColorTest::Blend:
        {
            return containerIndexWrap(index, gradients);
        }

        case ColorTest::Diff:
        case ColorTest::Hsl:
        {
            return containerIndexWrap(index, color::pallete::websafe::Array);
        }

        case ColorTest::Brightness:
        default:
        {
            return index;
        }
    }
    // clang-format on
}

std::size_t Resources::vertIndexWrap(const std::size_t index) const
{
    // clang-format off
    switch (which_test)
    {
        case ColorTest::Blend:
        case ColorTest::Diff:
        case ColorTest::Hsl:
        case ColorTest::Brightness:
        default:
        {
            if (index == std::numeric_limits<std::size_t>::max())
            {
                return 0;
            }
            else
            {
                return index;
            }
        }
    }
    // clang-format on
}

void Resources::setupWindow()
{
    if (window.isOpen())
    {
        window.close();
    }

    const sf::VideoMode videoMode {
        (is_fullscreen) ? sf::VideoMode::getDesktopMode()
                        : sf::VideoMode(2560, 2000, sf::VideoMode::getDesktopMode().bitsPerPixel)
    };

    window.create(
        videoMode, "Color Ranges", ((is_fullscreen) ? sf::Style::Fullscreen : sf::Style::Default));

    window.setFramerateLimit(30);

    side_texture.create(window.getSize().x, window.getSize().y);

    std::cout << "Window setup as " << windowSize().x << "x" << windowSize().y << " "
              << ((is_fullscreen) ? "Fullscreen" : "Floating") << std::endl;
}

void Resources::redraw()
{
    quad_verts.clear();
    line_verts.clear();

    switch (which_test)
    {
        case ColorTest::Blend: {
            setupColorTest_Blend();
            // printColorRowDifferences();
            break;
        }
        case ColorTest::Brightness: {
            setupColorTest_Brightness();
            break;
        }
        case ColorTest::Diff: {
            setupColorTest_Diff();
            break;
        }
        case ColorTest::Hsl: {
            setupColorTest_HSL();
            break;
        }
        default: {
            break;
        }
    }

    draw();
}

void Resources::handleEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        handleEvent(event);
    }
}

void Resources::handleEvent(const sf::Event & event)
{
    if (sf::Event::Closed == event.type)
    {
        window.close();
        return;
    }

    if (sf::Event::MouseButtonReleased == event.type)
    {
        sf::Image image(side_texture.getTexture().copyToImage());
        const sf::Vector2u mousePos { sf::Mouse::getPosition(window) };

        std::cout << "mouse at " << mousePos << "=" << image.getPixel(mousePos.x, mousePos.y)
                  << std::endl;

        return;
    }

    if (sf::Event::KeyPressed != event.type)
    {
        return;
    }

    if (sf::Keyboard::Escape == event.key.code)
    {
        window.close();
        return;
    }

    if (sf::Keyboard::F == event.key.code)
    {
        is_fullscreen = !is_fullscreen;
        setupWindow();
    }
    else if (sf::Keyboard::R == event.key.code)
    {
        reset();
    }
    else if (sf::Keyboard::Right == event.key.code)
    {
        if ((ColorTest::Blend == which_test) && event.key.shift)
        {
            rotation_speed -= rotation_inc;
        }
        else
        {
            horiz_index = horizIndexWrap(horiz_index + 1);
        }
    }
    else if (sf::Keyboard::Left == event.key.code)
    {
        if ((ColorTest::Blend == which_test) && event.key.shift)
        {
            rotation_speed += rotation_inc;
        }
        else
        {
            horiz_index = horizIndexWrap(horiz_index - 1);
        }
    }
    else if (sf::Keyboard::Up == event.key.code)
    {
        if (ColorTest::Blend == which_test)
        {
            const auto inc { static_cast<std::size_t>(
                static_cast<float>(blend_color_count) * 0.2f) };

            if (inc < 1)
            {
                blend_color_count += 1;
            }
            else
            {
                blend_color_count += inc;
            }
        }
        else
        {
            vert_index = vertIndexWrap(vert_index + 1);
        }
    }
    else if (sf::Keyboard::Down == event.key.code)
    {
        if ((ColorTest::Blend == which_test) && (blend_color_count > 0))
        {
            const auto inc { static_cast<std::size_t>(
                static_cast<float>(blend_color_count) * 0.2f) };

            if (inc < 1)
            {
                blend_color_count -= 1;
            }
            else
            {
                blend_color_count -= inc;
            }
        }
        else
        {
            vert_index = vertIndexWrap(vert_index - 1);
        }
    }
    else if (sf::Keyboard::Num1 == event.key.code)
    {
        reset();
        which_test = ColorTest::Blend;
        std::cout << "Switching to color test " << nameof::nameof_enum(which_test) << std::endl;
    }
    else if (sf::Keyboard::Num2 == event.key.code)
    {
        reset();
        which_test = ColorTest::Brightness;
        std::cout << "Switching to color test " << nameof::nameof_enum(which_test) << std::endl;
    }
    else if (sf::Keyboard::Num3 == event.key.code)
    {
        reset();
        which_test = ColorTest::Diff;
        std::cout << "Switching to color test " << nameof::nameof_enum(which_test) << std::endl;
    }
    else if (sf::Keyboard::Num4 == event.key.code)
    {
        reset();
        which_test = ColorTest::Hsl;
        std::cout << "Switching to color test " << nameof::nameof_enum(which_test) << std::endl;
    }

    redraw();
}

void Resources::draw()
{
    side_texture.clear();

    if (!line_verts.empty())
    {
        side_texture.draw(&line_verts[0], line_verts.size(), sf::PrimitiveType::Lines);
    }

    if (!quad_verts.empty())
    {
        side_texture.draw(&quad_verts[0], quad_verts.size(), sf::PrimitiveType::Quads);
    }

    side_texture.display();

    window.clear();
    window.draw(sf::Sprite(side_texture.getTexture()));
    window.display();
}

void Resources::setupColorTest_Blend()
{
    const float lineHeight { (windowSize().y / static_cast<float>(color_rows.size())) };

    for (std::size_t i(0); i < color_rows.size(); ++i)
    {
        ColorRow & row { color_rows.at(i) };

        if (0 == i)
        {
            row.top = 0.0f;
        }
        else
        {
            ColorRow & prevRow { color_rows.at(i - 1) };
            row.top = (prevRow.bottom + 1.0f);
        }

        row.bottom = row.top + lineHeight;
        row.middle = static_cast<unsigned>(row.top + (lineHeight * 0.5f));
    }

    auto appendColorRowQuad = [this](
                                  const float left,
                                  const float width,
                                  const Row::Enum rowEnum,
                                  const sf::Color & colorFrom,
                                  const sf::Color & colorTo) {
        const ColorRow & row { colorRow(rowEnum) };
        quad_verts.push_back({ { left, row.top }, colorFrom });
        quad_verts.push_back({ { (left + width), row.top }, colorTo });
        quad_verts.push_back({ { (left + width), row.bottom }, colorTo });
        quad_verts.push_back({ { left, row.bottom }, colorFrom });
    };

    auto appendColorRowLine
        = [&](const float left, const Row::Enum rowEnum, const sf::Color & color) {
              appendColorRowQuad(left, 1.0f, rowEnum, color, color);
          };

    const float singleColorWidth { windowSize().x / static_cast<float>(gradient().size() - 1) };
    const std::size_t singleColorWidthST { static_cast<std::size_t>(singleColorWidth) };

    float horizStart { 0.0f };
    for (std::size_t colorFromIndex { 0 }; colorFromIndex < gradient().size(); ++colorFromIndex)
    {
        if ((colorFromIndex + 1) >= gradient().size())
        {
            break;
        }

        const auto & colorFrom { gradient().at(colorFromIndex) };
        const auto & colorTo { gradient().at(colorFromIndex + 1) };

        std::vector<sf::Color> blendFill2Colors(singleColorWidthST);
        blendFill(blendFill2Colors, colorFrom, colorTo);

        appendColorRowQuad(horizStart, singleColorWidth, Row::VideoCard, colorFrom, colorTo);

        for (float horiz(0.0f); math::isRealCloseOrLess(horiz, singleColorWidth); horiz += 0.999f)
        {
            const float left { horizStart + horiz };
            const float ratio { horiz / singleColorWidth };

            appendColorRowLine(left, Row::Blend2, blend(ratio, colorFrom, colorTo));
            appendColorRowLine(left, Row::BlendFill2, ratioFromClamped(ratio, blendFill2Colors));
        }

        horizStart += singleColorWidth;
    }

    std::vector<sf::Color> blendNonLinearColors(blend_color_count);

    //
    std::vector<ColorAtRatio> colorsAt;
    for (std::size_t i(0); i < gradient().size(); ++i)
    {
        const sf::Color & color { gradient().at(i) };
        const float ratio { static_cast<float>(i) / static_cast<float>(gradient().size() - 1) };
        colorsAt.push_back({ color, ratio });
    }

    blendFillNonLinear(blendNonLinearColors, colorsAt);

    for (std::size_t i(0); i < window.getSize().x; ++i)
    {
        const float left { static_cast<float>(i) };
        const float ratio { left / windowSize().x };

        appendColorRowLine(left, Row::BlendN, blend(ratio, gradient()));

        appendColorRowLine(
            left, Row::BlendNonLinear, ratioFromRotation((ratio), blendNonLinearColors));
    }
}

void Resources::printColorRowDifferences()
{
    // auto colDiffStr = [&](const sf::Image & image,
    //                      const Row::Enum testRowEnum,
    //                      const unsigned horiz) {
    //    const ColorRow & testRow { colorRow(testRowEnum) };
    //    const ColorRow & videoCardRow { colorRow(Row::VideoCard) };
    //
    //    const float diffRatio { diffRatioOpaque(
    //        image.getPixel(horiz, testRow.middle), image.getPixel(horiz, videoCardRow.middle))
    //        };
    //
    //    std::ostringstream ss;
    //
    //    if (math::isRealZero(diffRatio))
    //    {
    //        return std::string();
    //    }
    //
    //    ss << std::setprecision(2) << std::setw(3) << std::right << (100.0f * diffRatio) <<
    //    '%'; return ss.str();
    //};

    auto rowDiffStr = [&](const sf::Image & image, const Row::Enum testRowEnum) -> std::string {
        const ColorRow & testRow { colorRow(testRowEnum) };
        const ColorRow & videoCardRow { colorRow(Row::VideoCard) };

        float sum { 0.0f };

        for (unsigned horiz(0); horiz < image.getSize().x; ++horiz)
        {
            const auto testColor { image.getPixel(horiz, testRow.middle) };
            const auto videoCardColor { image.getPixel(horiz, videoCardRow.middle) };

            sum += diffWeightedEuclidOpaque(testColor, videoCardColor);
        }

        std::ostringstream ss;

        ss << "  " << std::setw(17) << std::left << testRow.name << std::setprecision(2)
           << std::setw(4) << std::right << ((sum * 100.0f) / static_cast<float>(image.getSize().x))
           << '%';

        // auto printColDiff = [&](const unsigned horiz) {
        //    const auto str { colDiffStr(image, testRowEnum, horiz) };
        //
        //    if (str.empty())
        //    {
        //        return;
        //    }
        //
        //    ss << "\t\tdiff at " << std::setw(4) << std::left << horiz << "=" << str <<
        //    std::endl;
        //};
        //
        // printColDiff(0);
        // printColDiff(1);
        // printColDiff((image.getSize().x / 2) - 1);
        // printColDiff(image.getSize().x / 2);
        // printColDiff((image.getSize().x / 2) + 1);
        // printColDiff(image.getSize().x - 2);
        // printColDiff(image.getSize().x - 1);
        // printColDiff(image.getSize().x);

        return ss.str();
    };

    static sf::Image image;
    image = side_texture.getTexture().copyToImage();

    std::cout << "Gradient #" << horiz_index << std::endl;
    for (std::size_t i(0); i < color_rows.size(); ++i)
    {
        std::cout << rowDiffStr(image, static_cast<Row::Enum>(i)) << std::endl;
    }

    std::cout << std::endl;
}

void Resources::setupColorTest_Brightness()
{
    if (vert_index < 2)
    {
        vert_index = 2;
    }

    const float tileSizeRatio { 1.0f / static_cast<float>(vert_index) };

    std::vector<sf::Color> randomColors;

    auto randomColorAt = [&](const std::size_t index) { return randomColors.at(index); };

    auto appendQuad = [&](const sf::Vector2f & pos,
                          const sf::Vector2f & size,
                          std::vector<sf::Vertex> & verts,
                          const sf::Color & color = sf::Color::Transparent) {
        verts.push_back({ pos, color });
        verts.push_back({ { (pos.x + size.x), pos.y }, color });
        verts.push_back({ (pos + size), color });
        verts.push_back({ { pos.x, (pos.y + size.y) }, color });
    };

    auto appendQuadRect = [&](const sf::FloatRect & rect,
                              std::vector<sf::Vertex> & verts,
                              const sf::Color & color = sf::Color::Transparent) {
        appendQuad(
            (sf::Vector2f(rect.left, rect.top) - sf::Vector2f(1.0f, 1.0f)),
            (sf::Vector2f(rect.width, rect.height) + sf::Vector2f(2.0f, 2.0f)),
            verts,
            color);
    };

    auto fillWithTiles = [&](const float sizeRatio,
                             const sf::FloatRect & boundsF,
                             std::vector<sf::Vertex> & verts) {
        const sf::IntRect boundsI { boundsF };

        const sf::Vector2f tileSizeF { (boundsF.width * sizeRatio), (boundsF.height * sizeRatio) };
        const sf::Vector2i tileSizeI { tileSizeF };

        const int horizCount { boundsI.width / tileSizeI.x };
        const int vertCount { boundsI.height / tileSizeI.y };

        for (int vert(0); vert < vertCount; ++vert)
        {
            for (int horiz(0); horiz < horizCount; ++horiz)
            {
                const sf::Vector2i posIndexes(horiz, vert);

                const sf::Vector2i posI(
                    sf::Vector2i(boundsI.left, boundsI.top) + (posIndexes * tileSizeI));

                appendQuad(sf::Vector2f { posI }, sf::Vector2f { tileSizeI }, verts);
            }
        }
    };

    auto setQuadColors
        = [&](const sf::FloatRect & bounds, auto sortFunction, auto colorFetchFunction) {
              std::vector<sf::Vertex> verts;
              fillWithTiles(tileSizeRatio, bounds, verts);

              const std::size_t vertCount { verts.size() / 4 };
              while (randomColors.size() < vertCount)
              {
                  randomColors.push_back(color::random(random));
              }

              std::vector<sf::Color> colors;
              for (std::size_t i(0); i < vertCount; ++i)
              {
                  colors.push_back(colorFetchFunction(i));
              }

              std::sort(
                  std::begin(colors), std::end(colors), [&](const auto & left, const auto & right) {
                      return (sortFunction(left) < sortFunction(right));
                  });

              std::size_t colorIndex { 0 };
              std::size_t vertIndex { 0 };
              while ((vertIndex <= (verts.size() - 4)) && (colorIndex < colors.size()))
              {
                  const sf::Color color { colors.at(colorIndex) };

                  verts[vertIndex++].color = color;
                  verts[vertIndex++].color = color;
                  verts[vertIndex++].color = color;
                  verts[vertIndex++].color = color;

                  ++colorIndex;
              }

              std::copy(std::begin(verts), std::end(verts), std::back_inserter(quad_verts));
          };

    auto setupBrightnessTest = [&](const sf::FloatRect bounds, auto brightRatioFunction) {
        const sf::FloatRect colorBounds(
            bounds.left, bounds.top, bounds.width, (bounds.height * 0.5f));

        appendQuadRect(colorBounds, quad_verts, sf::Color::White);

        setQuadColors(colorBounds, brightRatioFunction, randomColorAt);

        auto colorToGrey = [&](const std::size_t index) {
            const sf::Color color { randomColorAt(index) };
            const float brightRatio { brightRatioFunction(color) };
            const sf::Uint8 greyValue { static_cast<sf::Uint8>(brightRatio * 255.0f) };
            return sf::Color(greyValue, greyValue, greyValue);
        };

        const sf::FloatRect greyBounds(
            bounds.left,
            (bounds.top + (bounds.height * 0.5f)),
            bounds.width,
            (bounds.height * 0.5f));

        appendQuadRect(greyBounds, quad_verts, sf::Color::White);

        setQuadColors(greyBounds, brightRatioFunction, colorToGrey);
    };

    const float borderRatio { 0.95f };
    const sf::Vector2f boundsSize { windowSize() * sf::Vector2f((1.0f / 3.0f), 1.0f) };

    const sf::FloatRect brightnessBounds { scaleInPlaceCopy(
        sf::FloatRect({ 0.0f, 0.0f }, boundsSize), borderRatio) };

    const sf::FloatRect luminosityBounds { scaleInPlaceCopy(
        sf::FloatRect({ boundsSize.x, 0.0f }, boundsSize), borderRatio) };

    const sf::FloatRect hslBounds { scaleInPlaceCopy(
        sf::FloatRect({ (boundsSize.x * 2.0f), 0.0f }, boundsSize), borderRatio) };

    setupBrightnessTest(brightnessBounds, color::brightnessRatio);
    setupBrightnessTest(luminosityBounds, color::luminosityRatio);
    setupBrightnessTest(hslBounds, color::brightnessHslRatio);
}

void Resources::setupColorTest_HSL()
{
    using namespace color::hsl;

    const sf::Color color { color::pallete::websafe::Array[horiz_index] };

    Hsla hsl { colorToHsla(color) };
    const sf::Color backgroundColorDark { sf::Color(36, 36, 36) };
    const sf::Color backgroundColorLight { backgroundColorDark + backgroundColorDark };

    //
    const sf::Vector2f windowCenter(windowSize() * 0.5f);

    const float cellBorderRatio { 0.01f };

    const std::size_t cellVertCount { std::clamp(vert_index, 4_st, 40_st) };
    const float cellVertCountF { static_cast<float>(cellVertCount) };

    const std::size_t cellHorizCountPerTest { std::clamp(((cellVertCount * 3) / 2), 4_st, 60_st) };
    const float cellHorizCountPerTestF { static_cast<float>(cellHorizCountPerTest) };

    const std::size_t cellHorizCount { (cellHorizCountPerTest * 2) - 1 };
    const float cellHorizCountF { static_cast<float>(cellHorizCount) };

    const sf::Vector2f origBoundsSize(
        (windowSize().x * (1.0f / cellHorizCountF)), (windowSize().y * 0.8f));

    const sf::Vector2f cellSize(origBoundsSize.x, (origBoundsSize.y / cellVertCountF));

    const sf::FloatRect origBounds((windowCenter - (origBoundsSize * 0.5f)), origBoundsSize);

    float posLeft { (windowSize().x - (cellHorizCountF * cellSize.x)) * 0.5f };

    //
    auto appendQuad = [&](const sf::Vector2f & pos,
                          const sf::Vector2f & size,
                          std::vector<sf::Vertex> & verts,
                          const sf::Color & clr = sf::Color::Transparent) {
        verts.push_back({ pos, clr });
        verts.push_back({ { (pos.x + size.x), pos.y }, clr });
        verts.push_back({ (pos + size), clr });
        verts.push_back({ { pos.x, (pos.y + size.y) }, clr });
    };

    auto appendQuadRect = [&](const sf::FloatRect & rect,
                              std::vector<sf::Vertex> & verts,
                              const sf::Color & clr = sf::Color::Transparent) {
        appendQuad(
            (sf::Vector2f(rect.left, rect.top) - sf::Vector2f(1.0f, 1.0f)),
            (sf::Vector2f(rect.width, rect.height) + sf::Vector2f(2.0f, 2.0f)),
            verts,
            clr);
    };

    auto appendLightnessColumn = [&](const sf::FloatRect & colBounds,
                                     Hsla hslColor,
                                     const sf::Color & backgroundColor) {
        appendQuadRect(
            scaleInPlaceCopy(
                colBounds,
                { (1.0f + cellBorderRatio),
                  (1.0f + ((cellBorderRatio * 2.0f * colBounds.width) / colBounds.height)) }),
            quad_verts,
            backgroundColor);

        //
        for (std::size_t light(0); light < cellVertCount; ++light)
        {
            const float lightRatio { static_cast<float>(light) / (cellVertCountF - 1.0f) };

            hslColor.lgt = lightRatio;

            const sf::FloatRect cellBounds(
                { colBounds.left, (colBounds.top + (static_cast<float>(light) * cellSize.y)) },
                cellSize);

            appendQuadRect(scaleInPlaceCopy(cellBounds, 0.925f), quad_verts, hslToColor(hslColor));

            // std::cout << light << "\t" << ratio << "\t" << cellBounds << "\t" << hslColor << "\t"
            //          << hslToColor(hslColor) << std::endl;
        }
    };

    //
    for (std::size_t sat(0); sat < (cellHorizCountPerTest - 1); ++sat)
    {
        const float satRatio { static_cast<float>(sat)
                               / static_cast<float>(cellHorizCountPerTest - 1) };

        const sf::FloatRect colBounds({ posLeft, origBounds.top }, origBoundsSize);

        hsl.sat = satRatio;

        appendLightnessColumn(colBounds, hsl, backgroundColorDark);

        posLeft += cellSize.x;
    }

    //
    const sf::FloatRect centerColBounds({ posLeft, origBounds.top }, origBoundsSize);
    hsl.sat = 1.0f;
    appendLightnessColumn(centerColBounds, hsl, backgroundColorLight);
    posLeft += cellSize.x;

    //
    for (std::size_t hue(1); hue < cellHorizCountPerTest; ++hue)
    {
        const sf::FloatRect colBounds({ posLeft, origBounds.top }, origBoundsSize);

        const sf::Color backgroundColor(36, 36, 36);

        hsl.hue += (1.0f / static_cast<float>(cellHorizCountPerTest - 1));

        appendLightnessColumn(colBounds, hsl, backgroundColorDark);

        posLeft += cellSize.x;
    }
}

void Resources::setupColorTest_Diff()
{
    auto appendQuad = [&](const sf::Vector2f & pos,
                          const sf::Vector2f & size,
                          std::vector<sf::Vertex> & verts,
                          const sf::Color & clr = sf::Color::Transparent) {
        verts.push_back({ pos, clr });
        verts.push_back({ { (pos.x + size.x), pos.y }, clr });
        verts.push_back({ (pos + size), clr });
        verts.push_back({ { pos.x, (pos.y + size.y) }, clr });
    };

    auto appendQuadRect = [&](const sf::FloatRect & rect,
                              std::vector<sf::Vertex> & verts,
                              const sf::Color & clr = sf::Color::Transparent) {
        appendQuad(
            (sf::Vector2f(rect.left, rect.top) - sf::Vector2f(1.0f, 1.0f)),
            (sf::Vector2f(rect.width, rect.height) + sf::Vector2f(2.0f, 2.0f)),
            verts,
            clr);
    };

    //
    const sf::FloatRect bounds({ 0.0f, 0.0f }, windowSize());

    const sf::Vector2f boundsCenter(
        (bounds.left + (bounds.width * 0.5f)), (bounds.top + (bounds.height * 0.5f)));

    const std::size_t cellVertCount { std::clamp(vert_index, 3_st, 20_st) };
    const float cellVertCountF { static_cast<float>(cellVertCount) };

    const float fullHeight { (bounds.height * 0.8f) };
    const float cellHeight { fullHeight / cellVertCountF };

    const float centerCellWidth { bounds.width * 0.15f };
    const float compCellWidth { centerCellWidth * 2.0f };

    const sf::Vector2f centerCellSize(centerCellWidth, cellHeight);
    const sf::Vector2f compCellSize(compCellWidth, cellHeight);

    const sf::FloatRect centerBounds(
        (boundsCenter - (sf::Vector2f(centerCellSize.x, fullHeight) * 0.5f)),
        { centerCellSize.x, fullHeight });

    // this is just to avoid black
    const sf::Color centerColor = [&]() {
        auto clr { color::pallete::websafe::Array[horiz_index] };

        if ((clr.r < 32) && (clr.g < 32) && (clr.b < 32))
        {
            clr.r += 200;
            clr.g += 200;
            clr.b += 200;
        }

        return clr;
    }();

    //
    std::vector<sf::Color> euclidColors;
    std::vector<sf::Color> weightedEuclidColors;
    {
        std::vector<sf::Color> origColors;

        while (origColors.size() < cellVertCount)
        {
            for (std::size_t i(0); i < 1000; ++i)
            {
                origColors.push_back(color::random(random));
            }

            origColors.erase(
                std::remove(std::begin(origColors), std::end(origColors), centerColor),
                std::end(origColors));

            std::sort(std::begin(origColors), std::end(origColors));

            origColors.erase(
                std::unique(std::begin(origColors), std::end(origColors)), std::end(origColors));
        }

        std::shuffle(
            std::begin(origColors), std::end(origColors), std::mt19937(std::random_device {}()));

        origColors.resize(std::clamp(vert_index, cellVertCount, 999_st));

        euclidColors = origColors;
        weightedEuclidColors = origColors;

        std::sort(
            std::begin(euclidColors),
            std::end(euclidColors),
            [&](const sf::Color & left, const sf::Color & right) {
                return (
                    color::diffEuclidOpaque(centerColor, left)
                    < color::diffEuclidOpaque(centerColor, right));
            });

        std::sort(
            std::begin(weightedEuclidColors),
            std::end(weightedEuclidColors),
            [&](const sf::Color & left, const sf::Color & right) {
                return (
                    color::diffWeightedEuclidOpaque(centerColor, left)
                    < color::diffWeightedEuclidOpaque(centerColor, right));
            });
    }

    //
    appendQuadRect(centerBounds, quad_verts, centerColor);

    for (std::size_t i(0); i < cellVertCount; ++i)
    {
        const sf::Color euclidColor { euclidColors.at(i) };
        const sf::Color wEuclidColor { weightedEuclidColors.at(i) };

        const float top { centerBounds.top + (static_cast<float>(i) * cellHeight) };

        const sf::FloatRect euclidianRect(
            { (centerBounds.left - compCellSize.x), top }, compCellSize);

        const sf::FloatRect weightedEuclidianRect(
            { (centerBounds.left + centerBounds.width), top }, compCellSize);

        appendQuadRect(scaleInPlaceCopy(euclidianRect, { 1.0f, 0.95f }), quad_verts, euclidColor);

        appendQuadRect(
            scaleInPlaceCopy(weightedEuclidianRect, { 1.0f, 0.95f }), quad_verts, wEuclidColor);
    }
}

// tests

void testHsl()
{
    auto testHsla = [&](const sf::Color & color) {
        const hsl::Hsla hsl { hsl::colorToHsla(color) };
        const sf::Color colorNew { hsl::hslToColor(hsl) };

        if (color == colorNew)
        {
            return;
        }

        std::cout << " hsl=" << std::setw(20) << std::left << hsl << ":\n\t" << color << "\n\t"
                  << colorNew << std::endl;
    };

    for (const sf::Color & color : color::pallete::rgbfull::Array)
    {
        testHsla(color);
    }

    for (const sf::Color & color : color::pallete::websafe::Array)
    {
        testHsla(color);
    }

    for (const sf::Color & color : color::pallete::xkcd::Array)
    {
        testHsla(color);
    }
}

void testColorDiff()
{
    /*
    const std::vector<sf::Color> colors { sf::Color::Black,      sf::Color::Blue,
                                          sf::Color::Cyan,       sf::Color::Green,
                                          sf::Color::Magenta,    sf::Color::Red,
                                          sf::Color::White,      sf::Color::Yellow,
                                          sf::Color::Transparent };

    std::vector<double> origs;
    std::vector<double> news;

    auto printIfTooHigh = [&](const float ratio) {
        if (ratio > 1.0f)
        {
            std::cout << " *** OUT OF BOUNDS *** " << std::endl;
        }
    };

    auto printDiffs = [&](const sf::Color & left, const sf::Color & right) {
        const auto diffOrigLR { color::diffHumanOpaque(left, right) };
        const auto diffOrigRL { color::diffHumanOpaque(right, left) };

        const auto diffNewLR { color::diffHuman(left, right) };
        const auto diffNewRL { color::diffHuman(right, left) };

        std::ostringstream ss;

        std::cout << left << " vs " << right << ":";

        ss << "\n\torig=" << diffOrigLR << ((diffOrigLR == diffOrigRL) ? "  (sym)" : " (ASYM)");
        ss << "\n\tnew =" << diffNewLR << ((diffNewLR == diffNewRL) ? "  (sym)" : "  (ASYM)");
        ss << "\n\tdiff=" << (diffOrigLR - diffNewLR);

        // if ((diffOrigLR != diffNewLR) || (diffOrigLR != diffOrigRL) || (diffNewLR !=
    diffNewRL))
        {
            std::cout << ss.str();
        }
        // else
        //{
        //    std::cout << "equal";
        //}

        std::cout << "\n\n";

        origs.push_back(diffOrigLR);
        news.push_back(diffNewLR);
    };

    for (const sf::Color & outer : colors)
    {
        for (const sf::Color & innerTemp : colors)
        {
            sf::Color inner { innerTemp };
            inner.a = 127;
            printDiffs(outer, inner);
        }
    }

    std::cout << "orig: " << makeStats(origs).toString() << std::endl;
    std::cout << "new : " << makeStats(news).toString() << std::endl << std::endl;
    */
}
