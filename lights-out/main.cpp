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

    const sf::Vector2f CELL_SIZE_V(
        (SCREEN_SIZE_V.x / static_cast<float>(CELL_COUNT_HORIZ)),
        (SCREEN_SIZE_V.y / static_cast<float>(CELL_COUNT_VERT)));

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Lights Out");

    const sf::Color CELL_ON_COLOR(255, 0, 0);
    const sf::Color CELL_OFF_COLOR(255, 0, 0, 127);
    const sf::Color CELL_OUTLINE_COLOR(sf::Color::Black);

    const float CELL_OUTLINE_THICKNESS(5.0f);
    const sf::Vector2f CELL_OUTLINE_THICKNESS_V(CELL_OUTLINE_THICKNESS, CELL_OUTLINE_THICKNESS);

    std::vector<sf::RectangleShape> rectangles;

    for (std::size_t row(0); row < CELL_COUNT_VERT; ++row)
    {
        for (std::size_t column(0); column < CELL_COUNT_HORIZ; ++column)
        {
            sf::RectangleShape cellShape;
            cellShape.setOutlineColor(CELL_OUTLINE_COLOR);
            cellShape.setOutlineThickness(CELL_OUTLINE_THICKNESS);
            cellShape.setFillColor(CELL_ON_COLOR);
            cellShape.setSize(CELL_SIZE_V - CELL_OUTLINE_THICKNESS_V);

            const sf::Vector2f CELL_POSITION_V(
                (static_cast<float>(column) * CELL_SIZE_V.x),
                (static_cast<float>(row) * CELL_SIZE_V.y));

            cellShape.setPosition(CELL_POSITION_V + CELL_OUTLINE_THICKNESS_V);

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
