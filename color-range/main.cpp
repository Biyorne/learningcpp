// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// main.cpp
//
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <initializer_list>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <ostream>
#include <set>
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

namespace math
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

    // this lib is for simple/innaccurate/game/etc apps, so a simple multiple of epsilon works fine
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

} // namespace math

//

namespace sf
{

    inline bool operator<(const Color & left, const Color & right)
    {
        return (left.toInteger() < right.toInteger());
    }

    inline std::ostream & operator<<(std::ostream & os, const sf::Color & color)
    {
        os << '(' << int(color.r) << ',' << int(color.g) << ',' << int(color.b);

        if (color.a < 255)
        {
            os << ',' << int(color.a);
        }

        os << ')';

        return os;
    }

} // namespace sf

//

namespace color
{

    // color value diffs

    [[nodiscard]] inline constexpr int diff(const sf::Uint8 left, const sf::Uint8 right) noexcept
    {
        return (static_cast<int>(right) - static_cast<int>(left));
    }

    [[nodiscard]] inline constexpr int diffAbs(const sf::Uint8 left, const sf::Uint8 right) noexcept
    {
        return math::abs(diff(left, right));
    }

    [[nodiscard]] inline float diffRatio(const sf::Uint8 & left, const sf::Uint8 & right) noexcept
    {
        return (static_cast<float>(diffAbs(left, right)) / 255.0f);
    }

    // color diffs

    [[nodiscard]] inline int diffOpaque(const sf::Color & left, const sf::Color & right) noexcept
    {
        return (diffAbs(left.r, right.r) + diffAbs(left.g, right.g) + diffAbs(left.b, right.b));
    }

    // all color diffs are absolute
    [[nodiscard]] inline int diff(const sf::Color & left, const sf::Color & right) noexcept
    {
        return (diffOpaque(left, right) + diffAbs(left.a, right.a));
    }

    // all color diffs are absolute
    [[nodiscard]] inline int diffAbs(const sf::Color & left, const sf::Color & right) noexcept
    {
        return diff(left, right);
    }

    [[nodiscard]] inline float diffRatio(const sf::Color & left, const sf::Color & right) noexcept
    {
        return (static_cast<float>(diffAbs(left, right)) / static_cast<float>(255 * 4));
    }

    [[nodiscard]] inline float
        diffRatioOpaque(const sf::Color & left, const sf::Color & right) noexcept
    {
        return (static_cast<float>(diffOpaque(left, right)) / static_cast<float>(255 * 3));
    }

    // blends

    [[nodiscard]] constexpr sf::Uint8
        blend(const float ratio, const sf::Uint8 from, const sf::Uint8 to) noexcept
    {
        return static_cast<sf::Uint8>(
            math::mapRatioTo(ratio, static_cast<int>(from), static_cast<int>(to)));
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
    [[nodiscard]] sf::Color blend(const float ratio, const Container_t & src)
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

    // blend fills (where colors are equally spaced)

    template <typename OutColorIter_t>
    void blendFill(
        const OutColorIter_t dstBegin,
        const std::size_t count,
        const sf::Color & from,
        const sf::Color & to)
    {
        if (0 == count)
        {
            return;
        }

        if (1 == count)
        {
            *dstBegin = from;
            return;
        }

        for (std::size_t i(0); i < count; ++i)
        {
            *(dstBegin + i)
                = blend((static_cast<float>(i) / static_cast<float>(count - 1)), from, to);
        }

        *(dstBegin + (count - 1)) = to;
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
            colorsAt[i].color = *(srcBegin + i);
            colorsAt[i].ratio = (static_cast<float>(i) / divisor);
        }

        blendFillNonLinear(dst, colorsAt);
    }

    // non-linear blend fills (where colors are NOT equally spaced) (ColorAtRatios define spacing)

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

    template <typename DstContainer_t, typename SrcContainer_t>
    void blendFillNonLinear(DstContainer_t & dst, const SrcContainer_t & src)
    {
        const std::size_t srcSize { src.size() };
        const std::size_t dstSize { dst.size() };

        if ((0 == srcSize) || (0 == dstSize))
        {
            return;
        }

        const auto srcBegin { std::begin(src) };
        const auto dstBegin { std::begin(dst) };

        if ((1 == srcSize) || (1 == dstSize))
        {
            std::fill_n(dstBegin, dstSize, srcBegin->color);
            return;
        }

        std::size_t srcIndex { 0 };
        std::size_t dstIndex { 0 };
        while ((srcIndex < (srcSize - 1)) && (dstIndex < dstSize))
        {
            const std::size_t dstSizeRemaining { dstSize - dstIndex };

            const ColorAtRatio & from { *(srcBegin + srcIndex) };
            const ColorAtRatio & to { *(srcBegin + srcIndex + 1) };

            const float ratioDiffAbs { std::clamp(math::abs(to.ratio - from.ratio), 0.0f, 1.0f) };

            const std::size_t blendCountOrig { static_cast<std::size_t>(
                ratioDiffAbs * static_cast<float>(dstSize)) };

            const std::size_t blendCount { std::clamp(blendCountOrig, 0_st, dstSizeRemaining) };

            blendFill((dstBegin + dstIndex), blendCount, from.color, to.color);

            ++srcIndex;
            dstIndex += blendCount;
        }

        *(dstBegin + (dstSize - 1)) = (srcBegin + (srcSize - 1))->color;
    }

    //

    /*
    class BlendCache
    {
    public:
        BlendCache() = default;

        sf::Color atClamped(const float ratio) const noexcept
        {
            if (m_colors.empty())
            {
                return sf::Color::Transparent;
            }

            if (!(ratio > 0.0f))
            {
                return m_colors[0];
            }

            const std::size_t index { static_cast<std::size_t>(
                ratio * static_cast<float>(m_colors.size())) };

            if (index >= m_colors.size())
            {
                return m_colors.back();
            }

            return m_colors[index];
        }

    private:
        std::vector<sf::Color> m_colors;
    };
    */

} // namespace color

//

namespace Row
{

    enum Enum
    {
        VideoCard,
        Blend2,
        BlendN,
        BlendFill2,
        BlendFillN,
        BlendNonLinear,
        Count
    };

    std::string toString(const Enum row)
    {
        // clang-format off
        switch(row)
        {
            case VideoCard:       { return "VideoCard"     ; }
            case Blend2:          { return "Blend2"        ; }
            case BlendN:          { return "BlendN"        ; }
            case BlendFill2:      { return "BlendFill2"    ; }
            case BlendFillN:      { return "BlendFillN"    ; }
            case BlendNonLinear:  { return "BlendNonLinear"; }
            case Count:           { return "row_enum_error__count"; }
            default:              { return ("row_enum_error__out_of_bounds=" + std::to_string(int(row))); }
        }
        // clang-format on
    }

} // namespace Row

struct ColorRow
{
    explicit ColorRow(const Row::Enum row = Row::Count)
        : row(row)
        , name(Row::toString(row))
    {}

    Row::Enum row;
    std::string name;
    float top = 0.0f;
    float bottom = 0.0f;
    unsigned middle = 0;
};

//

struct Resources
{
    Resources();

    bool isWindowOpen() const { return window.isOpen(); }
    unsigned windowWidth() const { return window.getSize().x; }
    sf::Vector2f windowSize() const { return sf::Vector2f { window.getSize() }; }
    const std::vector<sf::Color> & colors() const { return gradients.at(gradient_index); }
    const ColorRow & colorRow(const Row::Enum row) const { return color_rows.at(row); }

    void setupWindow();
    void setupDrawPrint();
    void handleEvents();
    void draw();

private:
    void setupRows();
    void setupLines();
    void printColorDifferences();
    void colorsInc();
    void colorsDec();

    bool is_fullscreen { false };

    sf::RenderWindow window;
    sf::RenderTexture side_texture;

    std::vector<sf::Vertex> quad_verts;
    std::vector<sf::Vertex> line_verts;

    std::size_t gradient_index { 0 };
    std::vector<std::vector<sf::Color>> gradients;

    std::vector<ColorRow> color_rows;
};

//

// TODO
// cached
// rotation

int main()
{
    Resources res;

    res.setupWindow();
    res.setupDrawPrint();

    while (res.isWindowOpen())
    {
        res.handleEvents();
        res.draw();
    }

    return EXIT_SUCCESS;
}

//

using namespace color;

Resources::Resources()
{
    const sf::Color redBrown { 200, 82, 45 };
    const sf::Color orange { 255, 165, 0 };

    gradients.push_back({ sf::Color::Black, sf::Color::White });
    gradients.push_back({ sf::Color::Yellow, sf::Color::Blue });
    gradients.push_back({ sf::Color::Red, sf::Color::Green, sf::Color::Blue });
    gradients.push_back({ redBrown, orange, sf::Color::Yellow, sf::Color::White });

    for (int i(0); i < Row::Count; ++i)
    {
        color_rows.push_back(ColorRow(static_cast<Row::Enum>(i)));
    }
}

void Resources::colorsInc()
{
    ++gradient_index;

    if (gradient_index >= gradients.size())
    {
        gradient_index = 0;
    }
}

void Resources::colorsDec()
{
    if (gradient_index > 0)
    {
        --gradient_index;
    }
    else
    {
        gradient_index = (gradients.size() - 1);
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

    window.setFramerateLimit(10);

    side_texture.create(window.getSize().x, window.getSize().y);

    std::cout << "Window setup as " << windowSize().x << "x" << windowSize().y << " "
              << ((is_fullscreen) ? "Fullscreen" : "Floating") << std::endl;
}

void Resources::setupDrawPrint()
{
    setupLines();
    draw();
    printColorDifferences();
}

void Resources::handleEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (sf::Event::Closed == event.type)
        {
            window.close();
        }
        else if (sf::Event::KeyPressed != event.type)
        {
            continue;
        }
        else if (sf::Keyboard::Escape == event.key.code)
        {
            window.close();
        }
        else if (sf::Keyboard::F == event.key.code)
        {
            is_fullscreen = !is_fullscreen;
            setupWindow();
            setupDrawPrint();
        }
        else if (sf::Keyboard::Right == event.key.code)
        {
            colorsInc();
            setupDrawPrint();
        }
        else if (sf::Keyboard::Left == event.key.code)
        {
            colorsDec();
            setupDrawPrint();
        }
    }
}

void Resources::draw()
{
    sf::RenderStates states;
    states.blendMode = sf::BlendNone;

    side_texture.clear();

    if (!line_verts.empty())
    {
        side_texture.draw(&line_verts[0], line_verts.size(), sf::PrimitiveType::Lines, states);
    }

    if (!quad_verts.empty())
    {
        side_texture.draw(&quad_verts[0], quad_verts.size(), sf::PrimitiveType::Quads, states);
    }

    side_texture.display();

    window.clear();
    window.draw(sf::Sprite(side_texture.getTexture()), states);
    window.display();
}

void Resources::setupRows()
{
    const float lineHeight { (windowSize().y / static_cast<float>(color_rows.size()))
                             - static_cast<float>(color_rows.size() - 1) };

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
            row.top = prevRow.bottom + 2.0f;
        }

        row.bottom = row.top + lineHeight;
        row.middle = static_cast<unsigned>(row.top + (lineHeight * 0.5f));
    }
}

// VideoCard,
// Blend2,
// BlendN,
// BlendFill2,
// BlendFillN,
// BlendNonLinear,

void Resources::setupLines()
{
    setupRows();

    quad_verts.clear();
    line_verts.clear();

    auto ratioToContained = [](const float ratio, const std::vector<sf::Color> & container) {
        if (container.empty())
        {
            return sf::Color::Transparent;
        }

        if (!(ratio > 0.0f))
        {
            return container[0];
        }

        const std::size_t index { static_cast<std::size_t>(
            ratio * static_cast<float>(container.size())) };

        if (index >= container.size())
        {
            return container.back();
        }

        return container[index];
    };

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

    const float singleColorWidth { windowSize().x / static_cast<float>(colors().size() - 1) };
    const std::size_t singleColorWidthST { static_cast<std::size_t>(singleColorWidth) };

    float horizStart { 0.0f };
    for (std::size_t colorFromIndex { 0 }; colorFromIndex < colors().size(); ++colorFromIndex)
    {
        if ((colorFromIndex + 1) >= colors().size())
        {
            break;
        }

        const auto & colorFrom { colors().at(colorFromIndex) };
        const auto & colorTo { colors().at(colorFromIndex + 1) };

        std::vector<sf::Color> blendFill2Colors(singleColorWidthST);
        blendFill(blendFill2Colors, colorFrom, colorTo);

        appendColorRowQuad(horizStart, singleColorWidth, Row::VideoCard, colorFrom, colorTo);

        for (float horiz(0.0f); math::isRealCloseOrLess(horiz, singleColorWidth); horiz += 0.999f)
        {
            const float left { horizStart + horiz };
            const float ratio { horiz / singleColorWidth };

            appendColorRowLine(left, Row::Blend2, blend(ratio, colorFrom, colorTo));
            appendColorRowLine(left, Row::BlendFill2, ratioToContained(ratio, blendFill2Colors));
        }

        horizStart += singleColorWidth;
    }

    std::vector<sf::Color> blendFillNColors(window.getSize().x);
    blendFill(blendFillNColors, colors());

    std::vector<sf::Color> blendNonLinearColors(window.getSize().x);

    std::vector<ColorAtRatio> colorsAt;
    for (std::size_t i(0); i < colors().size(); ++i)
    {
        const sf::Color & color { colors().at(i) };
        const float ratio { static_cast<float>(i) / static_cast<float>(colors().size() - 1) };
        colorsAt.push_back({ color, ratio });
    }

    blendFillNonLinear(blendNonLinearColors, colorsAt);

    for (std::size_t i(0); i < window.getSize().x; ++i)
    {
        const float left { static_cast<float>(i) };
        const float ratio { left / windowSize().x };

        appendColorRowLine(left, Row::BlendN, blend(ratio, colors()));
        appendColorRowLine(left, Row::BlendFillN, ratioToContained(ratio, blendFillNColors));

        appendColorRowLine(
            left, Row::BlendNonLinear, ratioToContained(ratio, blendNonLinearColors));
    }
}

void Resources::printColorDifferences()
{
    auto colDiffStr = [&](const sf::Image & image,
                          const Row::Enum testRowEnum,
                          const unsigned horiz) {
        const ColorRow & testRow { colorRow(testRowEnum) };
        const ColorRow & videoCardRow { colorRow(Row::VideoCard) };

        const float diffRatio { diffRatioOpaque(
            image.getPixel(horiz, testRow.middle), image.getPixel(horiz, videoCardRow.middle)) };

        std::ostringstream ss;
        ss << std::setprecision(2) << std::setw(3) << std::right << (100.0f * diffRatio) << '%';

        return ss.str();
    };

    auto rowDiffStr = [&](const sf::Image & image, const Row::Enum testRowEnum) -> std::string {
        const ColorRow & testRow { colorRow(testRowEnum) };
        const ColorRow & videoCardRow { colorRow(Row::VideoCard) };

        float sum { 0.0f };

        for (unsigned horiz(0); horiz < image.getSize().x; ++horiz)
        {
            const auto testColor { image.getPixel(horiz, testRow.middle) };
            const auto videoCardColor { image.getPixel(horiz, videoCardRow.middle) };

            sum += diffRatioOpaque(testColor, videoCardColor);
        }

        std::ostringstream ss;

        ss << "\t" << std::setw(17) << std::left << testRow.name;

        ss << std::setprecision(2) << std::setw(4) << std::right
           << ((sum * 100.0f) / static_cast<float>(image.getSize().x)) << '%';

        ss << "\n\t\tdiff first = " << colDiffStr(image, testRowEnum, 0);
        ss << "\n\t\tdiff middle= " << colDiffStr(image, testRowEnum, (image.getSize().x / 2));
        ss << "\n\t\tdiff last  = " << colDiffStr(image, testRowEnum, (image.getSize().x - 1));

        return ss.str();
    };

    static sf::Image image;
    image = side_texture.getTexture().copyToImage();

    std::cout << "Gradient #" << gradient_index << std::endl;
    for (std::size_t i(0); i < color_rows.size(); ++i)
    {
        std::cout << rowDiffStr(image, static_cast<Row::Enum>(i)) << std::endl;
    }

    std::cout << std::endl;
}
