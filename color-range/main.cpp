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
// helper function to demo the color-range feature by making a gradient
//
bool createHorizColorGradient(
    const std::vector<sf::Color> & colors,
    const std::size_t count,
    const sf::FloatRect & rect,
    std::vector<sf::Vertex> & quadVerts);

//
// main()
//

int main()
{
    // setup window
    const unsigned windowWidth { 1600 };
    const unsigned windowHeight { 200 };
    const sf::Vector2f windowSize { sf::Vector2f(windowWidth, windowHeight) };
    const sf::FloatRect windowRect { { 0.0f, 0.0f }, windowSize };

    const sf::VideoMode videoMode { sf::VideoMode(
        windowWidth, windowHeight, sf::VideoMode::getDesktopMode().bitsPerPixel) };

    sf::RenderWindow window;
    window.create(videoMode, "Color Range Demo", sf::Style::Default);
    window.setFramerateLimit(60);

    // gradient to paint in the window
    const sf::Color orange { 255, 165, 0 };
    const sf::Color redBrown { 200, 82, 45 };
    const sf::Color redRedBrown { 255, 82, 45 };

    const std::vector<sf::Color> colors
        = { redBrown, orange, sf::Color::Yellow, sf::Color::White, redBrown };

    // how many colors are currently making the gradient
    // which, after drawing, should always equal (quadVerts.size() * 4)
    std::size_t count { windowWidth };

    // the quads forming vertical lines across the window to draw the gradient
    std::vector<sf::Vertex> quadVerts;
    quadVerts.reserve(windowWidth * 4);

    // first draw of the full gradient with one pixel wide vertical line per color
    bool success = createHorizColorGradient(colors, count, windowRect, quadVerts);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
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
                if (count < windowWidth)
                {
                    ++count;
                    success = createHorizColorGradient(colors, count, windowRect, quadVerts);

                    std::cout << "count=" << count << ", success=" << std::boolalpha << success
                              << '\n';
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
                    else if (count > 500)
                    {
                        count -= 50;
                    }
                    else
                    {
                        --count;
                    }

                    success = createHorizColorGradient(colors, count, windowRect, quadVerts);

                    std::cout << "count=" << count << ", success=" << std::boolalpha << success
                              << '\n';
                }
            }
            else if (sf::Keyboard::P == event.key.code)
            {
                std::cout << (quadVerts.size() / 4) << " Colors:\n";

                for (std::size_t i(0); i < quadVerts.size(); i += 4)
                {
                    std::cout << quadVerts.at(i).color << '\n';
                }
            }
        }

        // update
        if (!success)
        {
            quadVerts.clear();
            count = 0;
        }

        // draw
        window.clear();

        if (!quadVerts.empty())
        {
            window.draw(&quadVerts[0], quadVerts.size(), sf::PrimitiveType::Quads);
        }

        window.display();
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
