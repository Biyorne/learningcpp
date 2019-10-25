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

    // util

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

    // don't use if non-linear!
    // template <typename Container_t>
    // void eraseAdjacentDuplicates(Container_t & container)
    //{
    //    container.erase(
    //        std::unique(std::begin(container), std::end(container)), std::end(container));
    //}
    //
    //// don't use if non-linear!
    // template <typename Container_t>
    // Container_t eraseAdjacentDuplicatesCopy(const Container_t & before)
    //{
    //    Container_t after(before);
    //    eraseAdjacentDuplicates(after);
    //    return after;
    //}

    // blend fills (where colors are equally spaced)

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
            // first: recurse and render to temp container "ideal" if dst was too small
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

    // class BlendCache
    //{
    // public:
    //    BlendCache(const std::size_t size, const sf::Color & from, const sf::Color & to)
    //        : m_colors(size)
    //    {
    //        blendFill(m_colors, from, to);
    //    }
    //
    //    BlendCache(const std::size_t size, const std::initializer_list<sf::Color> initList)
    //        : m_colors(initList.size())
    //    {
    //        blendFill(m_colors, initList);
    //    }
    //
    // private:
    //    std::vector<sf::Color> m_colors;
    //};

} // namespace color

//

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

struct Resources
{
    Resources();

    bool isWindowOpen() const { return window.isOpen(); }
    unsigned windowWidth() const { return window.getSize().x; }
    sf::Vector2f windowSize() const { return sf::Vector2f { window.getSize() }; }
    const std::vector<sf::Color> & gradient() const { return gradients.at(gradient_index); }
    const ColorRow & colorRow(const Row::Enum row) const { return color_rows.at(row); }

    void setupWindow();
    void redraw();
    void handleEvents();
    void draw();
    void setupColumns();

    float rotation { 0.0f };
    float rotation_speed { 0.0f };
    const float rotation_inc { 1.0f / 1000.0f };

private:
    void setupRows();
    void printColorDifferences();
    void gradientInc();
    void gradientDec();
    void columnInc();
    void columnDec();

    bool is_fullscreen { false };

    std::size_t column_count { 0 };

    sf::RenderWindow window;
    sf::RenderTexture side_texture;

    std::vector<sf::Vertex> quad_verts;
    std::vector<sf::Vertex> line_verts;

    std::size_t gradient_index { 0 };
    std::vector<std::vector<sf::Color>> gradients;

    std::vector<ColorRow> color_rows;

    const sf::Color orange { 255, 165, 0 };
    const sf::Color redBrown { 200, 82, 45 };
    const sf::Color redRedBrown { 255, 82, 45 };
};

//

int main()
{
    Resources res;

    while (res.isWindowOpen())
    {
        res.handleEvents();

        res.rotation += res.rotation_speed;
        res.setupColumns();

        res.draw();
    }

    return EXIT_SUCCESS;
}

//

using namespace color;

Resources::Resources()
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

    column_count = window.getSize().x;
    gradient_index = 0;

    redraw();
}

void Resources::gradientInc()
{
    ++gradient_index;

    if (gradient_index >= gradients.size())
    {
        gradient_index = 0;
    }
}

void Resources::gradientDec()
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

void Resources::columnInc()
{
    const auto inc { static_cast<std::size_t>(static_cast<float>(column_count) * 0.2f) };

    if (inc < 1)
    {
        column_count += 1;
    }
    else
    {
        column_count += inc;
    }
}

void Resources::columnDec()
{
    if (0 == column_count)
    {
        return;
    }

    const auto inc { static_cast<std::size_t>(static_cast<float>(column_count) * 0.2f) };

    if (inc < 1)
    {
        column_count -= 1;
    }
    else
    {
        column_count -= inc;
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

    window.setFramerateLimit(30);

    side_texture.create(window.getSize().x, window.getSize().y);

    column_count = window.getSize().x;

    std::cout << "Window setup as " << windowSize().x << "x" << windowSize().y << " "
              << ((is_fullscreen) ? "Fullscreen" : "Floating") << std::endl;
}

void Resources::redraw()
{
    setupRows();
    setupColumns();
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
            redraw();
        }
        else if (sf::Keyboard::R == event.key.code)
        {
            column_count = window.getSize().x;
            gradient_index = 0;
            redraw();
            rotation = 0.0f;
            rotation_speed = 0.0f;
        }
        else if (sf::Keyboard::Right == event.key.code)
        {
            if (event.key.shift)
            {
                rotation_speed -= rotation_inc;
                std::cout << "rotation at " << rotation << " speeding up to " << rotation_speed
                          << std::endl;
            }
            else
            {
                gradientInc();
                redraw();
            }
        }
        else if (sf::Keyboard::Left == event.key.code)
        {
            if (event.key.shift)
            {
                rotation_speed += rotation_inc;
                std::cout << "rotation at " << rotation << " slowing down to " << rotation_speed
                          << std::endl;
            }
            else
            {
                gradientDec();
                redraw();
            }
        }
        else if (sf::Keyboard::Up == event.key.code)
        {
            columnInc();
            redraw();
        }
        else if (sf::Keyboard::Down == event.key.code)
        {
            columnDec();
            redraw();
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
}

void Resources::setupColumns()
{
    quad_verts.clear();
    line_verts.clear();

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

    std::vector<sf::Color> blendNonLinearColors(column_count);
    //
    // clang-format off
    //std::vector<ColorAtRatio> colorsAt = {
    //    { sf::Color::Red,    0.00f },    
    //    { redRedBrown,       0.10f },     
    //    { orange,            (1.0f / 3.0f) + 0.10f },
    //    { sf::Color::Yellow, (2.0f / 3.0f) + 0.10f }, 
    //    { sf::Color::White,  1.00f }
    //};
    // clang-format on
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
            left, Row::BlendNonLinear, ratioFromRotation((ratio + rotation), blendNonLinearColors));
    }

    // std::cout << "window_width=" << window.getSize().x << ", columns=" << column_count << "/"
    //          << (100.0f * (static_cast<float>(column_count) / windowSize().x)) << "%"
    //          << ", colors=" << blendNonLinearColors.size() << "/"
    //          << (100.0f * (static_cast<float>(blendNonLinearColors.size()) / windowSize().x))
    //          << "%" << std::endl;
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

        if (math::isRealZero(diffRatio))
        {
            return std::string();
        }

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
        //    ss << "\t\tdiff at " << std::setw(4) << std::left << horiz << "=" << str << std::endl;
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

    std::cout << "Gradient #" << gradient_index << std::endl;
    for (std::size_t i(0); i < color_rows.size(); ++i)
    {
        std::cout << rowDiffStr(image, static_cast<Row::Enum>(i)) << std::endl;
    }

    std::cout << std::endl;
}
