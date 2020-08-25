// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// main.cpp
//
#include "color-palletes.hpp"
#include "random.hpp"
#include "util.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <memory>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <tuple>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

//
inline void fitTitle(
    sf::Text & text,
    const sf::FloatRect & rect,
    const bool isAbove,
    const float heightRatio = 0.05f)
{
    // if no text then only set position and bail
    const std::string str { text.getString() };
    if (str.empty())
    {
        text.setPosition(rect.left, rect.top);
        text.move(sf::Vector2f(rect.width, rect.height) * 0.5f);
        return;
    }

    // count the number of lines so the height can grow accordingly
    std::size_t extraLineCount { 0 };
    for (const char ch : str)
    {
        if ('\n' == ch)
        {
            ++extraLineCount;
        }
    }

    // establish initial position and size
    sf::FloatRect titleRect(rect);
    titleRect.height *= heightRatio;

    if (extraLineCount > 0)
    {
        titleRect.height *= (static_cast<float>(extraLineCount) * 2.0f);
    }

    // position above or just inside the given rect
    if (isAbove)
    {
        titleRect.top -= titleRect.height;
        titleRect.top -= (rect.height * 0.005f);
    }
    else
    {
        titleRect.top += (rect.height * 0.005f);
    }

    util::fit(text, titleRect);
}

inline void fillWithQuads(
    std::vector<sf::Vertex> & verts, const sf::FloatRect & boundsF, const float sizeRatio)
{
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

            util::appendQuad(verts, sf::Vector2f(posI), sf::Vector2f(tileSizeI));
        }
    }
}

//
// color blending and gradients
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
        return util::abs(diff<T>(left, right));
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

            const float min { util::min(redRatio, grnRatio, bluRatio) };
            const float max { util::max(redRatio, grnRatio, bluRatio) };
            const float diff { max - min };

            l = ((max + min) * 0.5f);

            if (util::isRealZeroOrLess(diff))
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

            if (util::isRealZero(s))
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
    };

    inline std::ostream & operator<<(std::ostream & os, const Hsla & hsl)
    {
        os << hsl.toString();
        return os;
    }

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

    //
    // ITU-R=International Telecommunication Union RadioCommunication Sector
    //
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
} // namespace color

// yeah, yeah, I know...
using namespace color;

//
// color statistics
//
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
        { }

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
        { }

        int r { 0 };
        int g { 0 };
        int b { 0 };
    };

    template <typename T>
    struct Stats
    {
        std::string toString(const StdManip_t numberWidth = 5) const
        {
            if (0 == count)
            {
                return "";
            }

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

        if (0 == stats.count)
        {
            return stats;
        }

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

                // std::cout << title << ": brightness=" << (100.0 *
                // (makeStats(bright_full).avg / 255.0))
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
enum class ColorTest
{
    Blend,
    Brightness,
    Diff,
    Hsl,
    Greyscale
};

//
inline void indexWrap(std::size_t & index, const std::size_t maxValid)
{
    if (index > (maxValid + 1))
    {
        index = maxValid;
    }
    else if ((index == (maxValid + 1)) || (0 == maxValid))
    {
        index = 0;
    }
}
//
template <typename Container_t>
inline void indexWrap(std::size_t & index, const Container_t & container)
{
    indexWrap(index, (container.size() - 1));
}

template <typename T = std::chrono::microseconds>
long long getDuration(const std::chrono::high_resolution_clock::time_point & startTime)
{
    const auto stopTime { std::chrono::high_resolution_clock::now() };
    const auto duration { std::chrono::duration_cast<T>(stopTime - startTime) };
    return duration.count();
}

inline void printDuration(
    const std::string & name, const std::chrono::high_resolution_clock::time_point & startTime)
{
    std::cout << name << " took " << getDuration(startTime) / 1000 << "ms" << std::endl;
}

//
// Context / Resources
//

struct Resources
{
    Resources();

    bool isWindowOpen() const { return window.isOpen(); }
    unsigned windowWidth() const { return window.getSize().x; }
    unsigned windowHeight() const { return window.getSize().y; }
    sf::Vector2f windowSize() const { return sf::Vector2f { window.getSize() }; }
    sf::FloatRect windowRect() const { return { { 0.0f, 0.0f }, windowSize() }; }

    const std::vector<sf::Color> & gradient() const
    {
        return gradients.at(std::clamp(horiz_index, 0_st, (color_rows.size() - 1)));
    }

    const ColorRow & colorRow(const Row::Enum row) const { return color_rows.at(row); }

    void setupWindow();
    void redraw();

    void reset();

    void handleEvents();
    void handleEvent(const sf::Event & event);

    void draw();

    void setupColorTest_Blend();
    void setupColorTest_Brightness();
    void setupColorTest_Diff();
    void setupColorTest_HSL();

    util::Random random;

    ColorTest which_test { ColorTest::Greyscale };

private:
    void printColorRowDifferences();

    std::vector<sf::Color> makeRandomColors(
        const std::size_t count, const sf::Color & colorToAvoid = sf::Color::Transparent);

    inline void appendQuad(
        const sf::Vector2f & pos,
        const sf::Vector2f & size,
        const sf::Color & clr = sf::Color::Transparent)
    {
        util::appendQuad(quad_verts, pos, size, clr);
    }

    inline void
        appendQuadRect(const sf::FloatRect & rect, const sf::Color & clr = sf::Color::Transparent)
    {
        util::appendQuadRect(quad_verts, rect, clr);
    }

    void setupText(
        sf::Text & text,
        const sf::FloatRect & rect,
        const std::string & str = "",
        const sf::Color & clr = sf::Color::Transparent,
        const bool isAbove = false);

    inline void addTitle(
        const sf::FloatRect & rect,
        const std::string & str,
        const sf::Color & clr,
        const bool isAbove = false)
    {
        sf::Text & text { texts.emplace_back(titleText) };
        text.setString("");
        std::cout << "Add Title:  \"" << str << "\"\n";
        setupText(text, rect, str, clr, isAbove);
    }

    inline void appendWindowTitleText(const std::string & str)
    {
        if (titleText.getString().isEmpty())
        {
            std::cout << "Append Window Title:  \"" << str << "\"\n";
            const auto textRect { util::scaleRectInPlaceCopy(windowRect(), 0.25f) };
            setupText(titleText, textRect, (titleText.getString() + str));
        }
    }

    bool is_fullscreen { false };

    std::size_t blend_color_count { 0 };

    sf::RenderWindow window;

    std::vector<sf::Vertex> quad_verts;

    std::size_t horiz_index { 0 };
    std::size_t vert_index { 0 };

    std::size_t horiz_index_max { 0 };
    std::size_t vert_index_max { 0 };

    std::vector<std::vector<sf::Color>> gradients;

    const sf::Color orange { 255, 165, 0 };
    const sf::Color redBrown { 200, 82, 45 };
    const sf::Color redRedBrown { 255, 82, 45 };

    std::vector<ColorRow> color_rows;

    ColorMakerStats color_stats_normal;
    ColorMakerStats color_stats_vibrant;
    ColorMakerStats color_stats_alt;

    std::vector<std::unique_ptr<sf::Texture>> images;

    sf::Font font;
    sf::Text titleText;

    bool boolean { false };

    std::vector<sf::Text> texts;

    std::size_t count { 0 };
};

//

void testHsl();
void testColorDiff();
void makePalleteCode(const std::string & headerFilePath);

//

int main()
{
    // makePalleteCode("Z:\\src\\learningcpp\\color-range\\color-names.hpp");

    Resources res;

    while (res.isWindowOpen())
    {
        res.handleEvents();

        if (ColorTest::Blend == res.which_test)
        {
            res.setupColorTest_Blend();
        }

        res.draw();
    }

    return EXIT_SUCCESS;
}

// Resources functions

using namespace color;

//

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

    //{
    //    std::filesystem::recursive_directory_iterator dirIter(std::filesystem::current_path());
    //
    //    for (const std::filesystem::directory_entry & entry : dirIter)
    //    {
    //        if (!entry.is_regular_file())
    //        {
    //            continue;
    //        }
    //
    //        const std::string extension { entry.path().extension().string() };
    //
    //        if ((extension != ".jpg") && (extension != ".png"))
    //        {
    //            continue;
    //        }
    //
    //        auto texture(std::make_unique<sf::Texture>());
    //
    //        if (!texture->loadFromFile(entry.path().string()))
    //        {
    //            continue;
    //        }
    //
    //        texture->setSmooth(true);
    //
    //        std::cout << "Loaded Image: " << entry.path().string() << std::endl;
    //        images.push_back(std::move(texture));
    //    }
    //}

    if (!font.loadFromFile("transpass-light.ttf"))
    {
        std::cerr << "FAILED TO LOAD FONT: transpass-light.ttf" << std::endl;
    }

    titleText.setFont(font);
    titleText.setCharacterSize(30);
    titleText.setFillColor(sf::Color::Green);
    titleText.setLineSpacing(2.0f / 3.0f);

    setupWindow();
    reset();
    redraw();
}

void Resources::setupText(
    sf::Text & text,
    const sf::FloatRect & rect,
    const std::string & str,
    const sf::Color & clr,
    const bool isAbove)
{
    if (!str.empty())
    {
        text.setString(str);
    }

    if (clr != sf::Color::Transparent)
    {
        text.setFillColor(clr);
    }

    fitTitle(text, rect, isAbove);

    appendQuadRect(text.getGlobalBounds(), sf::Color(0, 0, 0, 127));
}

std::vector<sf::Color>
    Resources::makeRandomColors(const std::size_t theCount, const sf::Color & colorToAvoid)
{
    std::vector<sf::Color> colors;

    while (colors.size() < theCount)
    {
        for (std::size_t i(0); i < 1000; ++i)
        {
            colors.push_back(color::random(random));
        }

        colors.erase(
            std::remove(std::begin(colors), std::end(colors), colorToAvoid), std::end(colors));

        std::sort(std::begin(colors), std::end(colors));
        colors.erase(std::unique(std::begin(colors), std::end(colors)), std::end(colors));
    }

    std::shuffle(std::begin(colors), std::end(colors), std::mt19937(std::random_device {}()));

    return colors;
}

void Resources::reset()
{
    blend_color_count = window.getSize().x;
    quad_verts.clear();
    texts.clear();

    switch (which_test)
    {
        case ColorTest::Brightness: // 1
        {
            // horiz is not used in this case
            horiz_index = 0;
            horiz_index_max = 0;

            vert_index = 50;
            vert_index_max = 100;
            break;
        }

        case ColorTest::Diff: // 2
        {
            horiz_index = 0;
            horiz_index_max = 140;

            vert_index = 100;
            vert_index_max = 1000;
            break;
        }

        case ColorTest::Hsl: // 3
        {
            horiz_index = 0;
            horiz_index_max = 5;

            vert_index = 0;
            vert_index_max = 1;
            break;
        }

        case ColorTest::Greyscale: // 4
        {
            horiz_index = 0;
            horiz_index_max = 100;

            vert_index = 0;
            vert_index_max = 100;
            break;
        }

        case ColorTest::Blend: // 0
        default
            : //
        {
            horiz_index = 0;
            horiz_index_max = (gradients.size() - 1);

            vert_index = 0;
            vert_index_max = (windowWidth() * 2);
            break;
        }
    }
}

void Resources::setupWindow()
{
    if (window.isOpen())
    {
        window.close();
    }

    const sf::VideoMode videoMode {
        (is_fullscreen) ? sf::VideoMode::getDesktopMode()
                        : sf::VideoMode(1600, 1200, sf::VideoMode::getDesktopMode().bitsPerPixel)
    };

    window.create(
        videoMode, "Color Ranges", ((is_fullscreen) ? sf::Style::Fullscreen : sf::Style::Default));

    window.setFramerateLimit(60);

    std::cout << "Window setup as " << windowSize().x << "x" << windowSize().y << " "
              << ((is_fullscreen) ? "Fullscreen" : "Floating") << std::endl;
}

void Resources::redraw()
{
    indexWrap(horiz_index, horiz_index_max);
    indexWrap(vert_index, vert_index_max);

    // setupText(titleText, windowRect(), (std::string(nameof::nameof_enum(which_test)) + " Test"));

    quad_verts.clear();
    texts.clear();

    switch (which_test)
    {
        case ColorTest::Brightness: //
        {
            setupColorTest_Brightness();
            break;
        }

        case ColorTest::Diff: //
        {
            setupColorTest_Diff();
            break;
        }

        case ColorTest::Hsl: //
        {
            setupColorTest_HSL();
            break;
        }

        case ColorTest::Greyscale: //
        {
            break;
        }

        case ColorTest::Blend:
        default: {
            setupColorTest_Blend();
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

// Q/ESC - Quit
// Left/Right - Test Selection
// Up/Down
void Resources::handleEvent(const sf::Event & event)
{
    if (sf::Event::Closed == event.type)
    {
        window.close();
        return;
    }

    if (sf::Event::MouseButtonReleased == event.type)
    {
        const sf::Vector2u mousePos { sf::Mouse::getPosition(window) };
        const sf::Color color { util::windowToImage(window).getPixel(mousePos.x, mousePos.y) };
        std::cout << "Pixel at mouse position=" << mousePos << " has color=" << color << std::endl;
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
        ++horiz_index;
    }
    else if (sf::Keyboard::Left == event.key.code)
    {
        --horiz_index;
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
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        {
            ++count;
        }
        else
        {
            ++vert_index;
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
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        {
            if (count > 0)
            {
                --count;
            }
        }
        else
        {
            --vert_index;
        }
    }
    else if (sf::Keyboard::Num1 == event.key.code)
    {
        which_test = ColorTest::Blend;
        reset();
        std::cout << "Switching to color test:  Blend\n";
    }
    else if (sf::Keyboard::Num2 == event.key.code)
    {
        which_test = ColorTest::Brightness;
        reset();
        std::cout << "Switching to color test:  Brightness\n";
    }
    else if (sf::Keyboard::Num3 == event.key.code)
    {
        which_test = ColorTest::Diff;
        reset();
        std::cout << "Switching to color test:  Diff\n";
    }
    else if (sf::Keyboard::Num4 == event.key.code)
    {
        which_test = ColorTest::Hsl;
        reset();
        std::cout << "Switching to color test:  Hsl\n";
    }
    else if (sf::Keyboard::Num5 == event.key.code)
    {
        which_test = ColorTest::Greyscale;
        reset();
        std::cout << "Switching to color test:  Greyscale\n";
    }
    else if (sf::Keyboard::B == event.key.code)
    {
        boolean = !boolean;
        std::cout << "Switching to boolean = " << std::boolalpha << boolean << std::endl;
    }

    redraw();
}

void Resources::draw()
{
    window.clear();

    if (!quad_verts.empty())
    {
        window.draw(&quad_verts[0], quad_verts.size(), sf::PrimitiveType::Quads);
    }

    for (const sf::Text & text : texts)
    {
        window.draw(text);
    }

    window.draw(titleText);
    window.display();
}

// #0
void Resources::setupColorTest_Blend()
{
    std::ostringstream ss;

    ss << "\n"
       << std::to_string(gradient().size()) << " colors in gradient\n"
       << vert_index << " colors displayed";

    for (const sf::Color & color : gradient())
    {
        ss << "\n" << color;
    }

    appendWindowTitleText(ss.str());

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

        for (float horiz(0.0f); util::isRealCloseOrLess(horiz, singleColorWidth); horiz += 0.999f)
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

// #1
void Resources::setupColorTest_Brightness()
{
    const float tileSizeRatio { 1.0f / static_cast<float>(vert_index) };

    std::vector<sf::Color> randomColors;

    auto randomColorAt = [&](const std::size_t index) { return randomColors.at(index); };

    auto setQuadColors
        = [&](const sf::FloatRect & bounds, auto sortFunction, auto colorFetchFunction) {
              std::vector<sf::Vertex> verts;
              fillWithQuads(verts, bounds, tileSizeRatio);

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

        appendQuadRect(colorBounds, sf::Color::White);

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

        appendQuadRect(greyBounds, sf::Color::White);

        setQuadColors(greyBounds, brightRatioFunction, colorToGrey);
    };

    const float borderRatio { 0.95f };
    const sf::Vector2f boundsSize { windowSize() * sf::Vector2f((1.0f / 3.0f), 1.0f) };

    const sf::FloatRect brightnessBounds { util::scaleRectInPlaceCopy(
        sf::FloatRect({ 0.0f, 0.0f }, boundsSize), borderRatio) };

    const sf::FloatRect luminosityBounds { util::scaleRectInPlaceCopy(
        sf::FloatRect({ boundsSize.x, 0.0f }, boundsSize), borderRatio) };

    const sf::FloatRect hslBounds { util::scaleRectInPlaceCopy(
        sf::FloatRect({ (boundsSize.x * 2.0f), 0.0f }, boundsSize), borderRatio) };

    setupBrightnessTest(brightnessBounds, color::brightnessAverage);
    setupBrightnessTest(luminosityBounds, color::luminosityRatio);
    setupBrightnessTest(hslBounds, brightnessHslRatio);
}

// #2
void Resources::setupColorTest_Diff()
{
    const sf::FloatRect bounds({ 0.0f, 0.0f }, windowSize() * 0.5f);

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
        auto clr { color::pallete::websafe_139::Colors[std::clamp(
            horiz_index, 0_st, (pallete::websafe_139::Colors.size() - 1))] };

        if (sf::Color::Black == clr)
        {
            clr = sf::Color::Red;
        }

        return clr;
    }();

    //
    const std::size_t colorCount { vert_index_max };
    std::vector<sf::Color> origColors { makeRandomColors(colorCount, centerColor) };
    origColors.resize(std::clamp(vert_index, cellVertCount, colorCount));

    std::vector<sf::Color> euclidColors(origColors);

    std::sort(
        std::begin(euclidColors),
        std::end(euclidColors),
        [&](const sf::Color & left, const sf::Color & right) {
            return (
                color::diffEuclidOpaque(centerColor, left)
                < color::diffEuclidOpaque(centerColor, right));
        });

    std::vector<sf::Color> weightedEuclidColors(origColors);

    std::sort(
        std::begin(weightedEuclidColors),
        std::end(weightedEuclidColors),
        [&](const sf::Color & left, const sf::Color & right) {
            return (
                color::diffWeightedEuclidOpaque(centerColor, left)
                < color::diffWeightedEuclidOpaque(centerColor, right));
        });

    //
    appendQuadRect(centerBounds, centerColor);

    for (std::size_t i(0); i < cellVertCount; ++i)
    {
        const sf::Color euclidColor { euclidColors.at(i) };
        const sf::Color wEuclidColor { weightedEuclidColors.at(i) };

        const float top { centerBounds.top + (static_cast<float>(i) * cellHeight) };

        const sf::FloatRect euclidianRect(
            { (centerBounds.left - compCellSize.x), top }, compCellSize);

        const sf::FloatRect weightedEuclidianRect(
            { (centerBounds.left + centerBounds.width), top }, compCellSize);

        appendQuadRect(util::scaleRectInPlaceCopy(euclidianRect, { 1.0f, 0.95f }), euclidColor);
        appendQuadRect(
            util::scaleRectInPlaceCopy(weightedEuclidianRect, { 1.0f, 0.95f }), wEuclidColor);
    }
}

// #3
void Resources::setupColorTest_HSL()
{
    std::string title { ": " };

    const std::vector<sf::Color> sfColors = [&]() {
        const std::size_t palleteIndex { std::clamp(horiz_index, 0_st, 5_st) };

        switch (palleteIndex)
        {
            case 0: {
                title += "rgb_8";
                return color::pallete::rgb_8::Colors;
            }

            case 1: {
                title += "cga_16";
                return color::pallete::cga_16::Colors;
            }

            case 2: {
                title += "commodore_16";
                return color::pallete::commodore_16::Colors;
            }

            case 3: {
                title += "windows_16";
                return color::pallete::windows_16::Colors;
            }

            case 4: {
                title += "websafe_139";
                return color::pallete::websafe_139::Colors;
            }

            case 5:
            default: {
                title += "xkcd_949";
                return color::pallete::xkcd_949::Colors;
            }
        }
    }();

    const bool willUseHslDiff { (vert_index % 2) != 0 };

    if (willUseHslDiff)
    {
        title += " / diff_Hsl";
    }
    else
    {
        title += " / diff_sfColor";
    }

    const std::size_t cellSizeDivisor { 30 };
    const std::size_t horizCount { windowWidth() / cellSizeDivisor };
    const float horizCountF { static_cast<float>(horizCount) };

    const std::size_t vertCount { windowHeight() / cellSizeDivisor };
    const float vertCountF { static_cast<float>(vertCount) };

    const sf::Vector2f cellSize((windowSize().x / horizCountF), (windowSize().y / vertCountF));

    sf::FloatRect bounds({ 0.0f, 0.0f }, cellSize);

    std::vector<long long> findClosestTimes;
    findClosestTimes.reserve(horizCount * vertCount * 2);

    std::vector<float> diffs;
    diffs.reserve(horizCount * vertCount * 2);

    auto findClosestColor = [&](const sf::Color & targetColor) {
        const auto startTime { std::chrono::high_resolution_clock::now() };

        float nearestDiff { 99999999.0f };
        sf::Color nearestColor;

        for (const sf::Color & c : sfColors)
        {
            // const float currentDiff { (willUseHslDiff) ? diffHslEuclid(targetColor, c)
            //                                           : diffEuclidOpaque(targetColor, c) };

            const float currentDiff { diffEuclidOpaque(targetColor, c) };

            diffs.push_back(currentDiff);

            if (currentDiff < nearestDiff)
            {
                nearestDiff = currentDiff;
                nearestColor = c;
            }
        }

        findClosestTimes.push_back(getDuration(startTime));
        return nearestColor;
    };

    const auto loopStart { std::chrono::high_resolution_clock::now() };

    for (std::size_t horiz(0); horiz < horizCount; ++horiz)
    {
        // const float posLeft { static_cast<float>(horiz) * cellSize.x };
        bounds.left = (static_cast<float>(horiz) * cellSize.x);
        const float horizRatio { static_cast<float>(horiz) / (horizCountF - 1.0f) };

        for (std::size_t vert(0); vert < vertCount; ++vert)
        {
            // const float posTop { static_cast<float>(vert) * cellSize.y };
            bounds.top = (static_cast<float>(vert) * cellSize.y);
            const float vertRatio { static_cast<float>(vert) / (vertCountF - 1.0f) };

            const Hsla hslColor(horizRatio, 1.0, vertRatio);
            const sf::Color sfColor { hslColor.sfColor() };

            if (boolean)
            {
                appendQuadRect(bounds, sfColor);
            }
            else
            {
                appendQuadRect(bounds, findClosestColor(sfColor));
            }
        }
    }

    printDuration("loop", loopStart);

    appendWindowTitleText(title);

    std::cout << "find_closest_color=  " << stats::makeStats(findClosestTimes).toString()
              << std::endl;

    std::cout << "color_diffs=          " << stats::makeStats(diffs).toString() << std::endl;
}

// tests

void Resources::printColorRowDifferences()
{
    // auto colDiffStr = [&](const sf::Image & image,
    //                      const Row::Enum testRowEnum,
    //                      const unsigned horiz) {
    //    const ColorRow & testRow { colorRow(testRowEnum) };
    //    const ColorRow & videoCardRow { colorRow(Row::VideoCard) };
    //
    //    const float diffRatio { diffRatioOpaque(
    //        image.getPixel(horiz, testRow.middle), image.getPixel(horiz,
    //        videoCardRow.middle))
    //        };
    //
    //    std::ostringstream ss;
    //
    //    if (util::isRealZero(diffRatio))
    //    {
    //        return std::string();
    //    }
    //
    //    ss << std::setprecision(2) << std::setw(3) << std::right << (100.0f * diffRatio)
    //    <<
    //    '%'; return ss.str();
    //};

    // auto rowDiffStr = [&](const sf::Image & image, const Row::Enum testRowEnum) ->
    // std::string {
    //    const ColorRow & testRow { colorRow(testRowEnum) };
    //    const ColorRow & videoCardRow { colorRow(Row::VideoCard) };
    //
    //    float sum { 0.0f };
    //
    //    for (unsigned horiz(0); horiz < image.getSize().x; ++horiz)
    //    {
    //        const auto testColor { image.getPixel(horiz, testRow.middle) };
    //        const auto videoCardColor { image.getPixel(horiz, videoCardRow.middle) };
    //
    //        sum += diffWeightedEuclidOpaque(testColor, videoCardColor);
    //    }
    //
    //    std::ostringstream ss;
    //
    //    ss << "  " << std::setw(17) << std::left << testRow.name << std::setprecision(2)
    //       << std::setw(4) << std::right << ((sum * 100.0f) /
    //       static_cast<float>(image.getSize().x))
    //       << '%';
    //
    //    // auto printColDiff = [&](const unsigned horiz) {
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
    //
    //    return ss.str();
    //};

    // std::cout << "Gradient #" << horiz_index << std::endl;
    // for (std::size_t i(0); i < color_rows.size(); ++i)
    //{
    //    std::cout << rowDiffStr(image, static_cast<Row::Enum>(i)) << std::endl;
    //}
    //
    // std::cout << std::endl;
}

void testHsl()
{
    auto testHsla = [&](const sf::Color & color) {
        const Hsla hsl(color);
        const sf::Color colorNew { hsl.sfColor() };

        if (color == colorNew)
        {
            return;
        }

        std::cout << " hsl=" << std::setw(20) << std::left << hsl << ":\n\t" << color << "\n\t"
                  << colorNew << std::endl;
    };

    for (const sf::Color & color : color::pallete::rgb_8::Colors)
    {
        testHsla(color);
    }

    for (const sf::Color & color : color::pallete::websafe_139::Colors)
    {
        testHsla(color);
    }

    for (const sf::Color & color : color::pallete::xkcd_949::Colors)
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

        ss << "\n\torig=" << diffOrigLR << ((diffOrigLR == diffOrigRL) ? "  (sym)" : "
    (ASYM)"); ss << "\n\tnew =" << diffNewLR << ((diffNewLR == diffNewRL) ? "  (sym)" : "
    (ASYM)"); ss << "\n\tdiff=" << (diffOrigLR - diffNewLR);

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

//
/*
template <typename T>
void testPallete(const std::size_t expectedCount, const bool isLastWhite)
{
    std::cout << "Testing Palette \"" << T::palleteName << "\":" << std::endl;

    if (NAMEOF_TYPE(T) != T::palleteName)
    {
        std::cout << "Error:  Names do not match palleteName=\"" << T::palleteName
                  << "\", nameof=\"" << nameof::nameof_type<T>() << "\"" << std::endl;
    }

    if (static_cast<std::size_t>(T::Count) != T::Colors.size())
    {
        std::cout << "Error:  Color counts do not match.  Enum::Count=\"" << T::Count
                  << " but Colors.size()=" << T::Colors.size() << std::endl;
    }

    if (T::Colors.size() != expectedCount)
    {
        std::cout << "Error:  Color count=" << T::Colors.size() << " != expected=" << expectedCount
                  << std::endl;
    }

    if (T::black != sf::Color::Black.toInteger())
    {
        std::cout << "Error:  black" << T::black << " != sf::Color::Black" << sf::Color::Black
                  << std::endl;
    }

    if (T::white != sf::Color::White.toInteger())
    {
        std::cout << "Error:  white" << T::white << " != sf::Color::White" << sf::Color::White
                  << std::endl;
    }

    if (T::Colors.front() != sf::Color::Black)
    {
        std::cout << "Error:  Colors.front()" << T::Colors.front() << " != sf::Color::Black"
                  << sf::Color::Black << std::endl;
    }

    if (isLastWhite && (T::Colors.back() != sf::Color::White))
    {
        std::cout << "Error:  Colors.back()" << T::Colors.back() << " != sf::Color::White"
                  << sf::Color::White << std::endl;
    }

    // std::size_t longestName { 26 };
    // for (const sf::Color & color : T::Colors)
    //{
    //    const T::Enum enumeration { static_cast<T::Enum>(color.toInteger()) };
    //    const std::string nameFromNameof { nameof::nameof_enum(enumeration) };
    //    const std::string nameFromFunction { nameFromNameof }; //
    //    T::name::color(enumeration)
    //
    //    if (nameFromFunction != nameFromNameof)
    //    {
    //        std::cout << "Error: Names do not match nameFromFunction=\"" <<
    //        nameFromFunction
    //                  << "\", nameFromNameof=\"" << nameFromNameof << "\"" << std::endl;
    //    }
    //
    //    if (longestName < nameFromFunction.length())
    //    {
    //        longestName = nameFromFunction.length();
    //    }
    //}
    // std::cout << std::endl;

    auto colors = T::Colors;

    std::sort(std::begin(colors), std::end(colors), [](const auto & left, const auto & right) {
        return (left.toInteger() < right.toInteger());
    });

    // for (const auto & color : colors)
    //{
    //    std::cout << std::setfill(' ') << std::setw(longestName + 1) << std::left
    //              << "<insert name here>"
    //              << " = 0x" << std::hex << std::setfill('0') << std::right <<
    //              std::setw(8)
    //              << color.toInteger() << ", // " << std::dec << color << std::endl;
    //}

    std::cout << std::endl;
    std::cout << std::endl;
}

struct ColorName
{
    sf::Color color = sf::Color::Transparent;
    std::string name;
};

/*
void makePalleteCode(const std::string & headerFilePath)
{
    std::ifstream stream(headerFilePath);

    if (!stream.is_open())
    {
        std::cerr << "unable to open file: " << headerFilePath << std::endl;
        return;
    }

    auto isCharAny = [](const char) { return true; };

    auto isCharLowerAlpha = [](const char ch) { return ((ch >= 'a') && (ch <= 'z')); };

    auto isCharValidName = [](const char ch) {
        const bool isLowerAlpha { (ch >= 'a') && (ch <= 'z') };
        const bool isDigit { (ch >= '0') && (ch <= '9') };
        const bool isSupportedSpecial { ch == '_' };
        return (isLowerAlpha || isDigit || isSupportedSpecial);
    };

    auto readValidName = [](auto isCharValid, const std::string & input, std::size_t & offset) {
        std::string name;

        for (; offset < input.length(); ++offset)
        {
            const char ch { input.at(offset) };

            if ((ch < ' ') || (ch > 'z'))
            {
                break;
            }

            if (ch == ' ')
            {
                if (name.empty())
                {
                    continue;
                }
                else
                {
                    break;
                }
            }

            if (isCharValid(ch))
            {
                name += ch;
            }
        }

        return name;
    };

    auto getStructDeclaration = [&](const std::string & line) -> std::string {
        std::size_t offset { 0 };
        const std::string str(readValidName(isCharLowerAlpha, line, offset));

        if (str != "struct")
        {
            return "";
        }

        return readValidName(isCharValidName, line, offset);
    };

    auto getColorName = [&](const std::string & line) -> ColorName {
        std::size_t offset { 0 };
        const std::string name { readValidName(isCharValidName, line, offset) };
        if (name.empty())
        {
            return {};
        }

        const std::string equals { readValidName(isCharAny, line, offset) };
        if (equals != "=")
        {
            return {};
        }

        const std::string hexStr { readValidName(isCharValidName, line, offset) };

        const bool isNumberHex { (hexStr.length() == 10) && (hexStr[0] == '0') && (hexStr[1] == 'x')
                                 && (hexStr[8] == 'f') && (hexStr[9] == 'f') };

        if (!isNumberHex)
        {
            if (name == "white")
            {
                return { sf::Color::White, name };
            }

            if (hexStr == "black")
            {
                return { sf::Color::Black, name };
            }

            return {};
        }

        sf::Uint32 number { 0 };
        std::stringstream ss;
        ss << hexStr;
        ss >> std::hex >> number;

        return { sf::Color(number), name };
    };

    std::map<std::string, std::vector<ColorName>> palletesToColorNames;
    std::string currentPalleteName;
    std::string line;
    while (std::getline(stream, line))
    {
        const std::string structName { getStructDeclaration(line) };
        if (structName.empty())
        {
            if (line.find(" struct ") < line.size())
            {
                std::cout << " *** Probably missed a struct! on line: " << line << std::endl;
            }
        }
        else
        {
            currentPalleteName = structName;
            continue;
        }

        if (currentPalleteName.empty())
        {
            continue;
        }

        const ColorName colorName { getColorName(line) };
        if (colorName.name.empty() || (colorName.color == sf::Color::Transparent))
        {
            if (line.find("0x") < line.size())
            {
                std::cout << currentPalleteName << " " << colorName.color << " " << colorName.name
                          << " *** Probably missed a color! on line: " << line << std::endl;
            }

            continue;
        }

        if (colorName.color.a != 255)
        {
            std::cerr << " *** ERROR invalid alpha value: " << colorName.color << std::endl;
            continue;
        }

        palletesToColorNames[currentPalleteName].push_back(colorName);
    }

    std::cout << palletesToColorNames.size() << " palettes" << std::endl << std::endl;

    for (auto & [palleteName, colorNames] : palletesToColorNames)
    {
        const bool willSort { (palleteName == "rgb_8") || (palleteName == "websafe_16")
                              || (palleteName == "websafe_216") || (palleteName == "websafe_139")
                              || (palleteName == "x11_543") || (palleteName == "xkcd_949") };

        if (willSort)
        {
            std::sort(
                std::begin(colorNames),
                std::end(colorNames),
                [](const ColorName & left, const ColorName & right) {
                    return (left.color < right.color);
                });
        }

        std::size_t longestName { 0 };
        std::set<std::string> names;

        for (const ColorName & colorName : colorNames)
        {
            names.insert(colorName.name);

            if (longestName < colorName.name.length())
            {
                longestName = colorName.name.length();
            }
        }

        if (names.size() != colorNames.size())
        {
            std::cerr << " *** ERROR duplicate names in palette " << palleteName << std::endl;
        }

        std::cout << "   " << palleteName << " with " << colorNames.size()
                  << " colors and longest name=" << longestName << std::endl;

        // auto printColorAsHex = []() {};
        // auto printColorAsRGB = []() {};
    }

    // std::cout << "\n  0x" << std::hex << std::setfill('0') << std::right << std::setw(8)
    //          << color.toInteger() << std::dec << "  " << color << "  ";

    std::cout << std::endl << std::endl;
}
*/

//
// divideBounds
//
/*


// if borderSize <0 then it's a scale of the default thin.  (i.e. -10 is ten times the default thin)
inline std::vector<sf::FloatRect> subdivide(
    const std::vector<std::size_t> & colCountForEachRow,
    const sf::FloatRect & bounds,
    const float borderSizeOrig = 0.0f,
    const bool willForceSquare = false)
{
    if (colCountForEachRow.empty() || (bounds.width < 1.0f) || (bounds.height < 1.0f))
    {
        return {};
    }

    const float borderSize = [&]() {
        if (borderSizeOrig < 0.0f)
        {
            const float defaultBorder { std::pow((bounds.width + bounds.height), 0.25f) };
            return (defaultBorder * -borderSizeOrig);
        }
        else
        {
            return borderSizeOrig;
        }
    }();

    const std::size_t rowCount { colCountForEachRow.size() };
    const float borderHeightSum(static_cast<float>(rowCount + 1) * borderSize);
    const float rowHeight { (bounds.height - borderHeightSum) / static_cast<float>(rowCount) };
    if (rowHeight < 1.0f)
    {
        return {};
    }

    std::vector<sf::FloatRect> rects;
    for (std::size_t rowIndex(0); rowIndex < rowCount; ++rowIndex)
    {
        const std::size_t colCount { colCountForEachRow.at(rowIndex) };
        if (0 == colCount)
        {
            continue;
        }

        const float borderWidthSum(static_cast<float>(colCount + 1) * borderSize);
        const float colWidth { (bounds.width - borderWidthSum) / static_cast<float>(colCount) };

        const float top { bounds.top + (static_cast<float>(rowIndex + 1) * borderSize)
                          + (static_cast<float>(rowIndex) * rowHeight) };

        for (std::size_t colIndex(0); colIndex < colCount; ++colIndex)
        {
            const float left { bounds.left + (static_cast<float>(colIndex + 1) * borderSize)
                               + (static_cast<float>(colIndex) * colWidth) };

            const sf::FloatRect rect({ left, top }, { colWidth, rowHeight });

            if (willForceSquare)
            {
                const sf::Vector2f rectCenter { center(rect) };

                const float squareSideLength { util::min(colWidth, rowHeight) };
                const sf::Vector2f squareSize(squareSideLength, squareSideLength);

                const sf::FloatRect squareRect { (rectCenter - (squareSize * 0.5f)), squareSize };
                rects.push_back(squareRect);
            }
            else
            {
                rects.push_back(rect);
            }
        }
    }

    return rects;
}


using ColPerRowVec_t = std::vector<int>;

//

inline ColPerRowVec_t
    divideBounds_uniformTiles(const int count, const sf::FloatRect & bounds, const Squares allowed)
{
    if (count <= 0)
    {
        return {};
    }

    const bool isWiderThanTaller { bounds.width > bounds.height };

    if (count <= 2)
    {
        if (isWiderThanTaller)
        {
            return ColPerRowVec_t(1, count);
        }
        else
        {
            return ColPerRowVec_t(count, 1);
        }
    }

    const bool isOneRow { (bounds.height / bounds.width) < (1.0f / static_cast<float>(count / 2)) };
    if (isOneRow)
    {
        return ColPerRowVec_t(1, count);
    }

    const bool isOneColumn { (bounds.width / bounds.height)
                             < (1.0f / static_cast<float>(count / 2)) };

    if (isOneColumn)
    {
        return ColPerRowVec_t(count, 1);
    }

    if (isPerfectSquare(count))
    {
        const int square { static_cast<int>(std::sqrt(count)) };
        return ColPerRowVec_t(square, square);
    }

    if (Squares::Perfect == allowed)
    {
        const int square { static_cast<int>(std::ceil(std::sqrt(count))) };
        return ColPerRowVec_t(square, square);
    }
    else
    {
        if (isAlmostPerfectSquare(count))
        {
            const int square { static_cast<int>(std::sqrt(count)) };
            return ColPerRowVec_t(square, (square + 1));
        }

        if (Squares::Uniform == allowed)
        {
            const int nextPerfectCount { findNextSquare(count, Squares::Perfect) };
            const int nextUniformCount { findNextSquare(count, Squares::Uniform) };

            if (nextPerfectCount < nextUniformCount)
            {
                const int square { static_cast<int>(std::sqrt(nextPerfectCount)) };
                return ColPerRowVec_t(square, square);
            }
            else
            {
                const int square { static_cast<int>(std::sqrt(nextUniformCount)) };
                return ColPerRowVec_t(square, (square + 1));
            }
        }
    }

    return {};
}

inline ColPerRowVec_t divideBounds_irregularTiles(const int count)
{
    const SquaresFinder squares(count, Squares::Irregular);
    if (!squares.is_valid)
    {
        if (count > 0)
        {
            std::cout << "SquaresFinder constructor EPIC FAIL";
        }

        return {};
    }

    // clang-format off
    const bool will_use_prev{ (squares.prev > 0) && (squares.dist_to_prev <= squares.dist_to_next)
}; const int count_to_use  { (will_use_prev) ? squares.prev : squares.next }; const int
square_to_use { static_cast<int>(std::sqrt(count_to_use)) }; const int total         { (count_to_use
/ square_to_use) * square_to_use }; const int remaining     { static_cast<int>(count) - total };
    const int rows          { count_to_use / square_to_use };
    const int columns       { square_to_use };
    // clang-format on

    std::cout << "\n  using_prev      = " << std::boolalpha << will_use_prev;
    std::cout << "\n  count           = " << count_to_use;
    std::cout << "\n  square          = " << square_to_use;
    std::cout << "\n  rows            = " << rows;
    std::cout << "\n  columns         = " << columns;
    std::cout << "\n  remain          = " << remaining;

    ColPerRowVec_t colCountForEachRow(
        static_cast<std::size_t>(rows), static_cast<std::size_t>(columns));

    for (int i(0); i < std::abs(remaining); ++i)
    {
        if (remaining < 0)
        {
            colCountForEachRow.at(i)--;
        }
        else
        {
            colCountForEachRow.at(i)++;
        }
    }

    std::cout << "\n  rows       = {";

    for (const int colCount : colCountForEachRow)
    {
        std::cout << colCount << ",";
    }

    std::cout << "}" << std::endl;

    return colCountForEachRow;
}

//

inline ColPerRowVec_t
    divideBounds(const int count, const sf::FloatRect & bounds, const Squares allowed)
{
    if ((bounds.width * bounds.height) < static_cast<float>(count))
    {
        return {};
    }

    const ColPerRowVec_t unifom { divideBounds_uniformTiles(count, bounds, allowed) };
    if (!unifom.empty())
    {
        return unifom;
    }

    if (Squares::Irregular == allowed)
    {
        const ColPerRowVec_t irregular { divideBounds_irregularTiles(count) };
        if (!irregular.empty())
        {
            return irregular;
        }
    }

    return {};
}

// if borderSize <0 then it's a scale of the default thin.  (i.e. -10 is ten times the default
// thin)
inline std::vector<sf::FloatRect> subdivide_v1(
    const std::size_t count,
    const sf::FloatRect & bounds,
    const float borderSize = 0.0f,
    const bool willForceSquare = false)
{
    ColPerRowVec_t rowVecI { divideBounds(static_cast<int>(count), bounds, Squares::Irregular) };

    if (rowVecI.empty())
    {
        return {};
    }

    const std::vector<std::size_t> rowVecS(rowVecI.begin(), rowVecI.end());

    return subdivide(rowVecS, bounds, borderSize, willForceSquare);
}

// const std::size_t countSquareCeil { static_cast<std::size_t>(std::ceil(std::sqrt(count))) };
// const std::size_t ceilRowCount { (count / countSquareCeil) };
// const std::size_t remainder { count - (ceilRowCount * countSquareCeil) };
//
// std::cout << "new stuff FAIL:";
// std::cout << "\n  count        = " << count;
// std::cout << "\n  sqrtFloor    = " << static_cast<std::size_t>(std::sqrt(count));
// std::cout << "\n  sqrtCeil     = " << countSquareCeil;
// std::cout << "\n  ceilRowCount = " << ceilRowCount
//          << " (total=" << (ceilRowCount * countSquareCeil) << ")";
// std::cout << "\n  remainder    = " << remainder;
// std::cout << "\n  prevGood     = " << squares.prev;
// std::cout << "\n  nextGood     = " << squares.next;
// std::cout << std::endl;
//
// std::vector<std::size_t> colCountForEachRow(ceilRowCount, countSquareCeil);
//
// if (0 == remainder)
//{
//    // um...nothing
//}
// else if (remainder > (countSquareCeil / 2))
//{
//    colCountForEachRow.push_back(remainder);
//}
// else
//{
//    colCountForEachRow.back() += remainder;
//}
//// for (std::size_t i(0); i < remainder; ++i)
////{
////    colCountForEachRow.at(i)--;
////}
//
// return subdivide(colCountForEachRow, bounds, borderSize, willForceSquare);

// std::size_t colCount { static_cast<std::size_t>(std::ceil(std::sqrt(count))) };
//
// std::size_t rowCount { static_cast<std::size_t>(
//    std::ceil(count / static_cast<float>(colCount))) };
//
// if (bounds.width < bounds.height)
//{
//    std::swap(colCount, rowCount);
//    std::cout << rowCount << "x" << colCount << "  swap" << std::endl;
//}
// else
//{
//    std::cout << rowCount << "x" << colCount << std::endl;
//}
//
// const std::vector<std::size_t> colCountForEachRow(rowCount, colCount);
//
// return subdivide(colCountForEachRow, bounds, borderSize, willForceSquare);

//

/*
// if borderSize <0 then it's a scale of the default thin.  (i.e. -10 is ten times the default
thin) inline std::vector<sf::FloatRect> subdivide_z( const std::size_t count, const
sf::FloatRect & bounds, const float borderSize = 0.0f, const bool willForceSquare = false)
{
    if ((bounds.width < 1.0f) || (bounds.height < 1.0f))
    {
        return {};
    }

    if (count <= 2)
    {
        if (bounds.width > bounds.height)
        {
            const std::vector<std::size_t> colCountForEachRow(1, count);
            return subdivide(colCountForEachRow, bounds, borderSize, willForceSquare);
        }
        else
        {
            const std::vector<std::size_t> colCountForEachRow(count, 1);
            return subdivide(colCountForEachRow, bounds, borderSize, willForceSquare);
        }
    }

    const float squareF { std::sqrtf(count) };
    const std::size_t squareS { static_cast<std::size_t>(squareF) };

    const std::size_t horizFloorCount { std::clamp(
        static_cast<std::size_t>(std::sqrtf(bounds.width)), 1_st, squareS) };

    const std::size_t vertFloorCount { std::clamp(
        static_cast<std::size_t>(std::sqrtf(bounds.height)), 1_st, squareS) };

    const std::size_t floorCountTotal { horizFloorCount * vertFloorCount };

    const std::size_t countDiff { std::max(count, floorCountTotal)
                                  - std::min(count, floorCountTotal) };

    if (floorCountTotal == count)
    {
        std::cout << "LUCKY #1"
                  << "count=" << count << ", horiz_count=" << horizFloorCount
                  << ", vert_count=" << vertFloorCount << ", diff=" << countDiff << std::endl;

        const std::vector<std::size_t> colCountForEachRow(vertFloorCount, horizFloorCount);
        return subdivide(colCountForEachRow, bounds, borderSize, willForceSquare);
    }

    if ((countDiff % horizFloorCount) == 0)
    {
        std::cout << "LUCKY #2 (horiz offset)"
                  << "count=" << count << ", horiz_count=" << horizFloorCount
                  << ", vert_count=" << vertFloorCount << ", diff=" << countDiff << std::endl;

        if (bounds.width > bounds.height)
        {
            const std::vector<std::size_t> colCountForEachRow(
                vertFloorCount, (horizFloorCount + (countDiff / vertFloorCount)));

            return subdivide(colCountForEachRow, bounds, borderSize, willForceSquare);
        }
        else
        {
            const std::vector<std::size_t> colCountForEachRow(
                (vertFloorCount + (countDiff / horizFloorCount)), horizFloorCount);

            return subdivide(colCountForEachRow, bounds, borderSize, willForceSquare);
        }
    }

    if ((countDiff % vertFloorCount) == 0)
    {
        std::cout << "LUCKY #3 (vert offset)"
                  << "count=" << count << ", horiz_count=" << horizFloorCount
                  << ", vert_count=" << vertFloorCount << ", diff=" << countDiff << std::endl;

        if (bounds.width > bounds.height)
        {
            const std::vector<std::size_t> colCountForEachRow(
                vertFloorCount, (horizFloorCount + (countDiff / vertFloorCount)));

            return subdivide(colCountForEachRow, bounds, borderSize, willForceSquare);
        }
        else
        {
            const std::vector<std::size_t> colCountForEachRow(
                (vertFloorCount + (countDiff / horizFloorCount)), horizFloorCount);

            return subdivide(colCountForEachRow, bounds, borderSize, willForceSquare);
        }
    }

    if (horizFloorCount < vertFloorCount)
    {
        std::vector<std::size_t> colCountForEachRow(vertFloorCount, (horizFloorCount + 1));
        const std::size_t extra { (vertFloorCount * (horizFloorCount + 1)) - count };

        for (std::size_t i(0); i < extra; ++i)
        {
            colCountForEachRow.at(i)--;
        }

        return subdivide(colCountForEachRow, bounds, borderSize, willForceSquare);
    }
    else
    {
        std::vector<std::size_t> colCountForEachRow((vertFloorCount + 1), horizFloorCount);
        const std::size_t extra { ((vertFloorCount + 1) * horizFloorCount) - count };

        for (std::size_t i(0); i < extra; ++i)
        {
            colCountForEachRow.at(i)--;
        }

        return subdivide(colCountForEachRow, bounds, borderSize, willForceSquare);
    }
}

inline std::vector<sf::FloatRect> subdivide_v2(
    const std::size_t count,
    const sf::FloatRect & bounds,
    const float borderSize = 0.0f,
    const bool willForceSquare = false)
{
    if (0 == count)
    {
        return {};
    }

    const std::size_t colCount { static_cast<std::size_t>(std::ceil(std::sqrt(count))) };

    const std::size_t rowCount { static_cast<std::size_t>(
        std::ceil(count / static_cast<float>(colCount))) };

    std::cout << rowCount << "x" << colCount << std::endl;
    const std::vector<std::size_t> colCountForEachRow(rowCount, colCount);

    return subdivide(colCountForEachRow, bounds, borderSize, willForceSquare);


    //ceil(sqrt(n)) to compute the number of columns (since this is, as you apparently have
    //guessed, the smallest amount of columns you need in order to not have more rows than
    //columns). Then, the number of rows you need in order to accommodate n elements distributed
    //across numColumns columns will be given by ceil(n / (double)numColumns).

*/
