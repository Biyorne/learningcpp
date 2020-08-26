#ifndef COLOR_RANGE_HPP_INCLUDED
#define COLOR_RANGE_HPP_INCLUDED
//
// color-range.hpp
//
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace color_range
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
        return std::abs(diff<T>(left, right));
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
            util::mapRatioTo(ratio, static_cast<float>(from), static_cast<float>(to)));
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

    //
    // HSL
    //
    //  HSL and HSV are alternative representations of the RGB color model designed in the
    //  1970s. They are better than RGB, but still make a trade-off of perceptual relevance
    //  for computation speed.  They work well enough when humans need to pick a single
    //  distinct color in isolation, but they are a poor choice when humans need to measure
    //  of how similar/different multiple colors appear to be.
    //
    //  From 1970-2000 there were few displays that could accurately display colors.  So
    //  humans were forced to use HSL/HSV numbers for colors.  They were better than RGB,
    //  but still fell far short of how humans see color.  Now that displays capable of
    //  showing accurate colors, humans can finally use their eyes to pick
    //  related/similar/distinct colors on screen.  So now the limitations of HSL and HSV
    //  are obvious, and significant enough to require better, more perceptual models.
    //  Luckily, computers are also now fast enough use such models that are far more
    //  computationally expensive, such as CIE-Lab/Luv.  But, uh, the HSL and Hsla are
    //  still here for some reason.
    //
    struct Hsla
    {
        Hsla() = default;

        Hsla(
            const float hue,
            const float saturation,
            const float lightness,
            const sf::Uint8 alpha = 255)
            : h(hue)
            , s(saturation)
            , l(lightness)
            , a(alpha)
        { }

        explicit Hsla(const sf::Color & color)
        {
            a = color.a;

            const float redRatio { static_cast<float>(color.r) / 255.0f };
            const float grnRatio { static_cast<float>(color.g) / 255.0f };
            const float bluRatio { static_cast<float>(color.b) / 255.0f };

            const float min { std::min(std::min(redRatio, grnRatio), bluRatio) };
            const float max { std::max(std::max(redRatio, grnRatio), bluRatio) };
            const float diff { max - min };

            l = ((max + min) * 0.5f);

            if (util::isRealCloseOrLess(diff, 0.0f))
            {
                return;
            }

            if (l < 0.5f)
            {
                s = (diff / (max + min));
            }
            else
            {
                s = (diff / (2.0f - max - min));
            }

            const float redDiff { (((max - redRatio) / 6.0f) + (diff / 2.0f)) / diff };
            const float grnDiff { (((max - grnRatio) / 6.0f) + (diff / 2.0f)) / diff };
            const float bluDiff { (((max - bluRatio) / 6.0f) + (diff / 2.0f)) / diff };

            if (util::isRealClose(redRatio, max))
            {
                h = (bluDiff - grnDiff);
            }
            else if (util::isRealClose(grnRatio, max))
            {
                h = (((1.0f / 3.0f) + redDiff) - bluDiff);
            }
            else if (util::isRealClose(bluRatio, max))
            {
                h = (((2.0f / 3.0f) + grnDiff) - redDiff);
            }

            h = hueRatioCorrect(h);
        }

        sf::Color sfColor() const
        {
            sf::Color color;

            if (util::isRealClose(s, 0.0f))
            {
                color.r = static_cast<sf::Uint8>(l * 255.0f);
                color.g = static_cast<sf::Uint8>(l * 255.0f);
                color.b = static_cast<sf::Uint8>(l * 255.0f);
            }
            else
            {
                const float var2 { (l < 0.5f) ? (l * (1 + s)) : ((l + s) - (s * l)) };
                const float var1 { (2 * l) - var2 };

                color.r = hueRatioToRgb(var1, var2, (h + (1.0f / 3.0f)));
                color.g = hueRatioToRgb(var1, var2, h);
                color.b = hueRatioToRgb(var1, var2, (h - (1.0f / 3.0f)));
            }

            color.a = a;
            return color;
        }

        std::string toString() const
        {
            std::ostringstream ss;

            ss << '[' << h << ',' << s << ',' << l;

            if (a < 255)
            {
                ss << ',' << int(a);
            }

            ss << ']';
            return ss.str();
        }

        float h { 0.0f }; // ratio of degrees [0,360]
        float s { 0.0f };
        float l { 0.0f };
        sf::Uint8 a { 255 }; // same as sf::Color.a (transparent=0, opaque=255)

    private:
        static constexpr float hueRatioCorrect(const float hueRatio) noexcept
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

        static constexpr sf::Uint8
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
    }; // namespace Hsla

    inline std::ostream & operator<<(std::ostream & os, const Hsla & hsl)
    {
        os << hsl.toString();
        return os;
    }

    //
    // helper stuff
    //

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
                    return util::isRealClose(left.ratio, right.ratio);
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

    inline float brightnessAverage(const sf::Color & color)
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

    inline float brightnessHslRatio(const sf::Color & color) { return Hsla(color).l; }

    //
    // Random Colors
    //

    inline sf::Color random(const util::Random & random, const bool willRandomizeAlpha = false)
    {
        return sf::Color(
            random.ofType<sf::Uint8>(),
            random.ofType<sf::Uint8>(),
            random.ofType<sf::Uint8>(),
            ((willRandomizeAlpha) ? random.ofType<sf::Uint8>() : 255));
    }

    inline sf::Color
        randomVibrant(const util::Random & random, const bool willRandomizeAlpha = false)
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

    //
    // blend fills (where colors are equally/linear spaced)
    //

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

    //
    // blend fills (where colors are NOT equally/linear spaced)
    //

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

    // just a container to prevent re-calculating
    class BlendCache
    {
        explicit BlendCache(const std::size_t size)
            : m_colors(size)
        { }

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

} // namespace color_range

#endif // COLOR_RANGE_HPP_INCLUDED

//
// ITU-R=International Telecommunication Union RadioCommunication Sector
//
/*
// ITU-R.601  (also CCIR.601 / Rec.601 / BT.601)  (year 1982)
//  The good old standard for encoding interlaced analog video signals as digital video.
//  Includes 525-line encoding at 60 Hz, and 625-line encoding at 50 Hz signals.
//      RGB Luma coefficients (Y’): R=0.299R, G=0.5870G, B=0.1140B
//
// ITU-R.709  (also Rec.709 / BT.709)  (year 1990)
//  Created for widescreen (16:9) high-definition television.
//  NOTE:  Rec.709 and sRGB share the same primary chromaticities and white-point
//  chromaticity, but sRGB is explicitly for display's with gamma 2.2.
//      RGB Luma coefficients (Y’):  R=0.2126, G=0.7152, B=0.0722
//
// ITU-R.2020 (also Rec.2020 / BT.2020)  (year 2012)
//  Created for UHDTV with standard dynamic range (SDR) and wide color gamut (WCG).
//      RGB Luma coefficients (Y’): R=0.2627, G=0.678, B=0.0593
//
// ITU-R.2100 (also Rec.2100 / BT.2100)  (year 2016)
//  Created for high dynamic range (HDR) formats for both HDTV 1080p and 4K/8K UHDTV
//  resolutions that uses different transfer functions for HDR, but...
//      RGB Luma coefficients (Y’):  USES SAME COLOR PRIMARIES AS REC.2020.
*/

//
// XYZ (Tristimulus) Reference values of a perfect reflecting diffuser
//
/*
    namespace xyz
    {
        struct ReferenceValues
        {
            std::string_view observer;
            std::string_view description;

            // CIE 1931 2degrees
            float x2;
            float y2;
            float z2;

            // CIE 1964 10degrees
            float x10;
            float y10;
            float z10;
        };

        // clang-format off
        //
        //constexpr ReferenceValues A{    "A",   "Incandescent/Tungsten", 109.850f,
   100.000f,  35.585f,    111.144f, 100.000f,  35.200f };
        //constexpr ReferenceValues B{    "B",   "Old Direct Sunlight At
   Noon",          99.0927f, 100.000f,  85.313f,     99.178f, 100.000f,  84.3493f};
        //constexpr ReferenceValues C{    "C",   "Old
   Daylight",                         98.074f,  100.000f, 118.232f,     97.285f, 100.000f,
   116.145f };
        //constexpr ReferenceValues D50{  "D50", "ICC Profile
   PCS",                      96.422f,  100.000f,  82.521f,     96.720f, 100.000f,  81.427f };
        //constexpr ReferenceValues D55{  "D55", "Midmorning
   Daylight",                  95.682f,  100.000f,  92.149f,     95.799f, 100.000f,  90.926f };
        //constexpr ReferenceValues D65{  "D65",
   "Daylight/sRGB/AdobeRGB",               95.047f,  100.000f, 108.883f,     94.811f, 100.000f,
   107.304f };
        //constexpr ReferenceValues D75{  "D75", "North Sky
   Daylight",                   94.972f,  100.000f, 122.638f,     94.416f, 100.000f, 120.641f };
        //constexpr ReferenceValues E{    "E",   "Equal Energy", 100.000f,  100.000f, 100.000f,
   100.000f, 100.000f, 100.000f };
        //constexpr ReferenceValues F1{   "F1",  "Daylight
   Fluorescent",                 92.834f,  100.000f, 103.665f,     94.791f, 100.000f, 103.191f
   };
        //constexpr ReferenceValues F2{   "F2",  "Cool
   Fluorescent",                     99.187f,  100.000f,  67.395f,    103.280f,
   100.000f,  69.026f };
        //constexpr ReferenceValues F3{   "F3",  "White Fluorescent", 103.754f,
   100.000f,  49.861f,    108.968f, 100.000f,  51.965f };
        //constexpr ReferenceValues F4{   "F4",  "Warm White Fluorescent", 109.147f,
   100.000f,  38.813f,    114.961f, 100.000f,  40.963f };
        //constexpr ReferenceValues F5{   "F5",  "Daylight
   Fluorescent",                 90.872f,  100.000f,  98.723f,     93.369f, 100.000f,  98.636f
   };
        //constexpr ReferenceValues F6{   "F6",  "Lite White
   Fluorescent",               97.309f,  100.000f,  60.191f,    102.148f, 100.000f,  62.074f };
        //constexpr ReferenceValues F7{   "F7",  "Daylight Fluorescent/D65
   Simulator",   95.044f,  100.000f, 108.755f,     95.792f, 100.000f, 107.687f };
        //constexpr ReferenceValues F8{   "F8",  "Sylvania F40/D50
   Simulator",           96.413f,  100.000f,  82.333f,     97.115f, 100.000f,  81.135f };
        //constexpr ReferenceValues F9{   "F9",  "Cool White Fluorescent", 100.365f,
   100.000f,  67.868f,    102.116f, 100.000f,  67.826f };
        //constexpr ReferenceValues F10{  "F10", "Ultralume 50/Philips
   TL85",            96.174f,  100.000f,  81.712f,     99.001f, 100.000f,  83.134f };
        //constexpr ReferenceValues F11{  "F11", "Ultralume 40/Philips TL84", 100.966f,
   100.000f,  64.370f,    103.866f, 100.000f,  65.627f };
        //constexpr ReferenceValues F12{  "F12", "Ultralume 30/Philips TL83", 108.046f,
   100.000f,  39.228f,    111.428f, 100.000f,  40.353f };
        //
        //
        // Converting
        // RGB -> CIE.XYZ.Rec.709 with D65 white point:
        // 0.412453 & 0.357580 & 0.180423
        // 0.212671 & 0.715160 & 0.072169
        // 0.019334 & 0.119193 & 0.950227
        //...and back again
        // 3.240479 & -1.53715 & -0.498535
        // -0.969256 & 1.875991 & 0.041556
        // 0.055648 & -0.204043 & 1.057311
        //
        // clang-format on

        struct Xyza
        {
            Xyza() = default;

            explicit Xyza(const sf::Color & color)
                : x((0.412453f * color.r) + (0.35758f * color.g) + (0.180423f * color.b))
                , y((0.212671f * color.r) + (0.71516f * color.g) + (0.072169f * color.b))
                , z((0.019334f * color.r) + (0.119193f * color.g) + (0.950227f * color.b))
                , a(color.a)
            { }

            // sf::Color sfColor() const {}

            std::string toString() const
            {
                std::ostringstream ss;

                ss << '[' << x << ',' << y << ',' << z;

                if (a < 255)
                {
                    ss << ',' << int(a);
                }

                ss << ']';
                return ss.str();
            }

            float x { 0.0f };
            float y { 0.0f };
            float z { 0.0f };
            sf::Uint8 a { 255 }; // same as sf::Color.a (transparent=0, opaque=255)
        };
    } // namespace xyz
    */

//
// CIE
//
/*
    namespace cie
    {
        struct Laba
        {
            explicit Laba(const sf::Color & color)
                : l()
                , a()
                , b()
                , alpha(color.a)
            {
                // Reference-X, Y and Z refer to specific illuminants and observers.
                // Common reference values are available below in this same page.
                //
                // var_X = X / Reference - X
                // var_Y = Y / Reference - Y
                // var_Z = Z / Reference - Z
                //
                // if (var_X > 0.008856) var_X = var_X ^ (1 / 3)
                // else                    var_X = (7.787 * var_X) + (16 / 116)
                //
                // if (var_Y > 0.008856) var_Y = var_Y ^ (1 / 3)
                // else                    var_Y = (7.787 * var_Y) + (16 / 116)
                //
                // if (var_Z > 0.008856) var_Z = var_Z ^ (1 / 3)
                // else                    var_Z = (7.787 * var_Z) + (16 / 116)
                //
                // CIE - L * = (116 * var_Y) - 16
                // CIE - a * = 500 * (var_X - var_Y)
                // CIE - b * = 200 * (var_Y - var_Z)
            }

            float l { 0.0f };
            float a { 0.0f };
            float b { 0.0f };
            sf::Uint8 alpha { 255 }; // same as sf::Color.a (transparent=0, opaque=255)
        };
    } // namespace cie
    */
