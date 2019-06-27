#include <cmath>
#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

float calcY(const int plotNumber, const float X)
{
    switch (plotNumber)
    {
        // y = x
        case 0:
        {
            return X;
        }

        // y = 10x
        case 1:
        {
            return (10.0f * X);
        }

        // y = x/10
        case 2:
        {
            return (0.1f * X);
        }

        // y = X*X
        case 3:
        {
            return (X * X);
        }

        // y = sqrt(x)
        case 4:
        {
            return std::sqrt(X);
        }

        // (x - 2) / 4 = y
        case 5:
        {
            return (X - 2.0f) / 4.0f;
        }

        // all zeros if the number is invalid
        default:
        {
            return 0.0f;
        }
    }
}

void makePlotVertexArray(
    const int number,
    const sf::Color & color,
    const sf::Vector2f & windowSize,
    sf::VertexArray & points)
{
    points.setPrimitiveType(sf::PrimitiveType::LinesStrip);
    points.clear();

    const auto half(windowSize * 0.5f);

    float x(-half.x);

    while (x < half.x)
    {
        const float Y(calcY(number, x));
        points.append(sf::Vertex((sf::Vector2f(x, -Y) + half), color));
        x += 1.0f;
    }

    if ((points.getVertexCount() % 2) != 0)
    {
        points.append(points[points.getVertexCount() - 1]);
    }
}

int main()
{
    sf::RenderWindow window(
        sf::VideoMode(800, 600, sf::VideoMode::getDesktopMode().bitsPerPixel),
        "2D-Plot",
        sf::Style::Fullscreen);

    const sf::Vector2f windowSize(window.getSize());

    const sf::Color axisColor(sf::Color::White);
    const sf::Color plotColor(sf::Color::Red);

    sf::VertexArray axisLines(sf::PrimitiveType::Lines, 4);
    axisLines.append(sf::Vertex(sf::Vector2f((windowSize.x * 0.5f), 0.0f), axisColor));
    axisLines.append(sf::Vertex(sf::Vector2f((windowSize.x * 0.5f), windowSize.y), axisColor));
    axisLines.append(sf::Vertex(sf::Vector2f(0.0f, (windowSize.y * 0.5f)), axisColor));
    axisLines.append(sf::Vertex(sf::Vector2f(windowSize.x, (windowSize.y * 0.5f)), axisColor));

    float plotExtent(10.0f);
    int plotNumber(0);

    sf::VertexArray plotPoints;
    makePlotVertexArray(plotNumber, plotColor, windowSize, plotPoints);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if ((event.type == sf::Event::Closed) || (event.type == sf::Event::MouseButtonPressed)
                || ((event.type == sf::Event::KeyPressed)
                    && (event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    plotExtent *= 2.0f;
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    plotExtent *= 0.5f;
                }
                else if (
                    (event.key.code >= sf::Keyboard::Num1)
                    || (event.key.code <= sf::Keyboard::Num9))
                {
                    plotNumber = (event.key.code - sf::Keyboard::Num1);
                }

                makePlotVertexArray(plotNumber, plotColor, windowSize, plotPoints);
            }
        }

        window.clear();
        window.draw(axisLines);
        window.draw(plotPoints);
        window.display();
    }
}
