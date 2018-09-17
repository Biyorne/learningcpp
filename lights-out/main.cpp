#include <SFML/Graphics.hpp>
#include <vector>

int main()
{
    const unsigned int SCREEN_WIDTH(800);
    const unsigned int SCREEN_HEIGHT(600);
    const sf::Vector2f SCREEN_SIZE_V(sf::Vector2u(SCREEN_WIDTH, SCREEN_HEIGHT));

    const std::size_t CELL_COUNT_HORIZ(3);
    const std::size_t CELL_COUNT_VERT(CELL_COUNT_HORIZ);
    const std::size_t CELL_COUNT(CELL_COUNT_HORIZ * CELL_COUNT_VERT);

    const float CELL_PAD(10);

    const float CELL_WIDTH((SCREEN_WIDTH - ((CELL_COUNT_HORIZ + 1) * CELL_PAD)) / CELL_COUNT_HORIZ);
    const float CELL_HEIGHT((SCREEN_HEIGHT - ((CELL_COUNT_VERT + 1) * CELL_PAD)) / CELL_COUNT_VERT);

    const sf::Vector2f CELL_SIZE_V(CELL_WIDTH, CELL_HEIGHT);

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Lights Out");

    const sf::Color CELL_ON_COLOR(255, 0, 0);
    const sf::Color CELL_OFF_COLOR(255, 0, 0, 127);
    const sf::Color CELL_OUTLINE_COLOR(sf::Color::Black);

    std::vector<sf::RectangleShape> rectangles;

    for (std::size_t row(0); row < CELL_COUNT_VERT; ++row)
    {
        const float ROW_FLOAT(static_cast<float>(row));

        for (std::size_t column(0); column < CELL_COUNT_HORIZ; ++column)
        {
            const float COLUMN_FLOAT(static_cast<float>(column));

            sf::RectangleShape cellShape;
            cellShape.setFillColor(CELL_ON_COLOR);
            cellShape.setSize(CELL_SIZE_V);

            const float LEFT((CELL_PAD * (COLUMN_FLOAT + 1.0f)) + (CELL_WIDTH * COLUMN_FLOAT));
            const float TOP((CELL_PAD * (ROW_FLOAT + 1.0f)) + (CELL_HEIGHT * ROW_FLOAT));
            const sf::Vector2f CELL_POSITION_V(LEFT, TOP);
            cellShape.setPosition(CELL_POSITION_V);

            rectangles.push_back(cellShape);
        }
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed)
            {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                const sf::Vector2f MOUSE_POSITION_V(
                    sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

                for (sf::RectangleShape & rectangle : rectangles)
                {
                    if (rectangle.getGlobalBounds().contains(MOUSE_POSITION_V))
                    {
                        if (rectangle.getFillColor() == CELL_ON_COLOR)
                        {
                            rectangle.setFillColor(CELL_OFF_COLOR);
                        }
                        else
                        {
                            rectangle.setFillColor(CELL_ON_COLOR);
                        }
                        break;
                    }
                }
            }
        }

        window.clear();

        for (const sf::RectangleShape & RECTANGLE : rectangles)
        {
            window.draw(RECTANGLE);
        }

        window.display();
    }

    return EXIT_SUCCESS;
}
