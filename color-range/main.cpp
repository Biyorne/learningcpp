// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// main.cpp
//
#include "random.hpp"
#include "util.hpp"

#include "color-range.hpp"

#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

//
//
//
struct Context
{
    Context()
        : window {}
        , window_size { 0.0f, 0.0f }
        , window_rect { 0.0f, 0.0f, 0.0f, 0.0f }
    {
        window.create(sf::VideoMode::getDesktopMode(), "Color Range Demo", sf::Style::Fullscreen);
        window.setFramerateLimit(60);

        window_size = sf::Vector2f { window.getSize() };
        window_rect = sf::FloatRect { { 0.0f, 0.0f }, window_size };
    }

    sf::RenderWindow window;
    sf::Vector2f window_size;
    sf::FloatRect window_rect;
};

//
// helper function to demo the color-range feature by making a gradient
//
bool createHorizColorGradient(
    const std::vector<sf::Color> & colors,
    const std::size_t count,
    const sf::FloatRect & rect,
    std::vector<sf::Vertex> & quadVerts);

//
// helper function to show vertical stack of colors
//
void drawColorStack(
    Context & context, const std::size_t whichStack, const std::vector<sf::Color> & colors);

//
//
//
int main()
{
    // setup window
    Context context;

    // where the color range/gradient will be drawn on screen
    sf::FloatRect gradientRect { context.window_rect };
    const float rangeSizeRatio { 0.1f };
    gradientRect.top += (context.window_rect.height * rangeSizeRatio);
    gradientRect.left += (context.window_rect.width * rangeSizeRatio);
    gradientRect.width -= (context.window_rect.width * (rangeSizeRatio * 2.0f));
    gradientRect.height = (context.window_rect.height * rangeSizeRatio);

    // gradient to paint in the window
    const sf::Color orange { 255, 165, 0 };
    const sf::Color redBrown { 200, 82, 45 };
    const sf::Color redRedBrown { 255, 82, 45 };

    const std::vector<sf::Color> colors
        = { redBrown, orange, sf::Color::Yellow, sf::Color::White, redBrown };

    // how many colors are currently in the gradient
    std::size_t count { 256 };

    // the quads forming vertical lines across the window to draw the gradient
    std::vector<sf::Vertex> quadVerts;
    quadVerts.reserve(count * 4);

    // first draw of the full gradient with one pixel wide vertical line per color
    createHorizColorGradient(colors, count, gradientRect, quadVerts);

    // random colors
    util::Random random;
    std::vector<sf::Color> randomColors;
    for (std::size_t i(0); i < count; ++i)
    {
        randomColors.push_back(color_range::random(random));
    }
    // color_range::blendFill(randomColors, colors);

    randomColors.push_back(sf::Color::Black);
    randomColors.push_back(sf::Color::White);
    randomColors.push_back(sf::Color::Red);
    randomColors.push_back(sf::Color::Green);
    randomColors.push_back(sf::Color::Blue);
    randomColors.push_back(sf::Color::Yellow);
    randomColors.push_back(sf::Color::Cyan);
    randomColors.push_back(sf::Color::Magenta);

    // sorted by linear RGB
    std::vector<sf::Color> rgbSortedColors(randomColors);
    std::sort(
        std::begin(rgbSortedColors),
        std::end(rgbSortedColors),
        [&](const sf::Color & c1, const sf::Color & c2) {
            int c1Sum { c1.r };
            c1Sum += c1.g;
            c1Sum += c1.b;

            int c2Sum { c2.r };
            c2Sum += c2.g;
            c2Sum += c2.b;

            return (c1Sum < c2Sum);
        });

    // sorted by HSL
    std::vector<sf::Color> hslSortedColors(randomColors);
    std::sort(
        std::begin(hslSortedColors),
        std::end(hslSortedColors),
        [&](const sf::Color & c1, const sf::Color & c2) {
            return (color_range::brightness::Hsl(c1) < color_range::brightness::Hsl(c2));
        });

    // sorted by weighted brightness average
    std::vector<sf::Color> weightedMeanSortedColors(randomColors);
    std::sort(
        std::begin(weightedMeanSortedColors),
        std::end(weightedMeanSortedColors),
        [&](const sf::Color & c1, const sf::Color & c2) {
            return (
                color_range::brightness::WeightedMean(c1)
                < color_range::brightness::WeightedMean(c2));
        });

    // sorted by Perceived brightness
    std::vector<sf::Color> perceivedSortedColors(randomColors);
    std::sort(
        std::begin(perceivedSortedColors),
        std::end(perceivedSortedColors),
        [&](const sf::Color & c1, const sf::Color & c2) {
            return (
                color_range::brightness::W3Perceived(c1)
                < color_range::brightness::W3Perceived(c2));
        });

    // sorted by luminosity
    std::vector<sf::Color> luminRatioSortedColors(randomColors);
    std::sort(
        std::begin(luminRatioSortedColors),
        std::end(luminRatioSortedColors),
        [&](const sf::Color & c1, const sf::Color & c2) {
            return (
                color_range::brightness::Luminosity(c1) < color_range::brightness::Luminosity(c2));
        });

    while (context.window.isOpen())
    {
        //
        // handle events
        sf::Event event;
        while (context.window.pollEvent(event))
        {
            if (sf::Event::Closed == event.type)
            {
                return EXIT_SUCCESS;
            }

            if (sf::Event::KeyPressed != event.type)
            {
                continue;
            }

            if (sf::Keyboard::Up == event.key.code)
            {
                if (count < 1000)
                {
                    ++count;
                    createHorizColorGradient(colors, count, gradientRect, quadVerts);
                }
                else
                {
                    std::cout << (quadVerts.size() / 4) << " Colors\n";
                }
            }
            else if (sf::Keyboard::Down == event.key.code)
            {
                if (count > 0)
                {
                    if (count > 1000)
                    {
                        count -= 100;
                    }
                    else if (count > 300)
                    {
                        count -= 50;
                    }
                    else
                    {
                        --count;
                    }

                    createHorizColorGradient(colors, count, gradientRect, quadVerts);
                }
                else
                {
                    std::cout << (quadVerts.size() / 4) << " Colors\n";
                }
            }
            else if (sf::Keyboard::P == event.key.code)
            {
                std::cout << (quadVerts.size() / 4) << " Colors\n";

                // for (std::size_t i(0); i < quadVerts.size(); i += 4)
                //{
                //    std::cout << quadVerts.at(i).color << '\n';
                //}
            }
            else if (
                (sf::Keyboard::Q == event.key.code) || (sf::Keyboard::Escape == event.key.code))
            {
                context.window.close();
            }
        }

        //
        // update
        //  nothing to do here yet

        //
        // draw
        context.window.clear();

        if (!quadVerts.empty())
        {
            context.window.draw(&quadVerts[0], quadVerts.size(), sf::PrimitiveType::Quads);
        }

        drawColorStack(context, 0, hslSortedColors);
        drawColorStack(context, 1, perceivedSortedColors);
        drawColorStack(context, 2, weightedMeanSortedColors);
        drawColorStack(context, 3, luminRatioSortedColors);

        context.window.display();
    }

    return EXIT_SUCCESS;
}

//
//
//
bool createHorizColorGradient(
    const std::vector<sf::Color> & colors,
    const std::size_t count,
    const sf::FloatRect & rect,
    std::vector<sf::Vertex> & quadVerts)
{
    quadVerts.clear();

    if ((0 == count) || colors.empty())
    {
        return false;
    }

    // create as many colors as specified from the gradient
    std::vector<sf::Color> vertLineColors;
    vertLineColors.resize(count);
    color_range::blendFill(vertLineColors, colors);

    // create all the quads that will draw each of those colors
    float posLeft { rect.left };
    const float widthPerColor { rect.width / static_cast<float>(vertLineColors.size()) };

    for (const sf::Color & color : vertLineColors)
    {
        if (posLeft > (rect.left + rect.width))
        {
            break;
        }

        const sf::FloatRect colorRect(posLeft, rect.top, widthPerColor, rect.height);
        posLeft += widthPerColor;

        util::appendQuadRect(quadVerts, colorRect, color);
    }

    return true;
}

//
//
//
void drawColorStack(
    Context & context, const std::size_t whichStack, const std::vector<sf::Color> & colors)
{
    sf::FloatRect rect { context.window_rect };
    rect.top = (context.window_rect.height * 0.3f);

    rect.left = 1.0f + ((static_cast<float>(whichStack) * (context.window_rect.width * 0.2f)));

    rect.width = (context.window_rect.width * 0.15f);
    rect.height = (context.window_rect.height - rect.top - 1.0f);

    sf::RectangleShape rectShape;
    rectShape.setOutlineColor(sf::Color::White);
    rectShape.setOutlineThickness(1.0f);
    rectShape.setPosition(rect.left, rect.top);
    rectShape.setSize(sf::Vector2f { rect.width, rect.height });
    rectShape.setFillColor(sf::Color::Transparent);

    context.window.draw(rectShape);

    if (colors.empty())
    {
        return;
    }

    // pre-loop rectangle shape setup
    rectShape.setPosition(rect.left, rect.top);
    rectShape.setOutlineColor(sf::Color::Transparent);
    const float lineHeight { rect.height / static_cast<float>(colors.size()) };
    const sf::Vector2f lineSize { rect.width, lineHeight };
    rectShape.setSize(lineSize);

    for (const sf::Color & color : colors)
    {
        rectShape.setFillColor(color);
        context.window.draw(rectShape);
        rectShape.move(0.0f, lineHeight);
    }
}
