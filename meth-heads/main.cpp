#include <SFML/Graphics.hpp>

#include <cmath>
#include <iostream>

// Factors in lazy vs greedy
//    Simulation run time
//    Value maximum
//    Number of cell spaces
//    Number of loot items on screen at a time

struct BoardView
{
    BoardView(const sf::Vector2u & windowSize)
        : pad_ratio(0.1f)
        , window_size(windowSize)
        , score_region(0.0f, 0.0f, (0.2f * window_size.x), window_size.y)
        , board_region(
              (score_region.left + score_region.width),
              score_region.top,
              (window_size.x - score_region.width),
              score_region.height)
        , background_color { 32, 32, 32 }
        , foreground_color { 220, 220, 220 }
        , column_count { 20 }
        , row_count { column_count }
        , line_thickness_ratio(1.0f / 600.0f)
        , line_thickness(line_thickness_ratio * window_size.y)
        , rectangles()
    {
        makeGrid();
    }

    float pad_ratio;
    sf::Vector2f window_size;
    sf::FloatRect score_region;
    sf::FloatRect board_region;
    sf::Color background_color;
    sf::Color foreground_color;
    std::size_t column_count;
    std::size_t row_count;
    float line_thickness_ratio;
    float line_thickness;
    std::vector<sf::RectangleShape> rectangles;

private:
    inline const sf::RectangleShape
        makeRectangleShape(const sf::Vector2f & pos, const sf::Vector2f & size) const
    {
        sf::RectangleShape temporary(size);
        temporary.setFillColor(background_color);
        temporary.setOutlineColor(foreground_color);
        temporary.setOutlineThickness(line_thickness);
        temporary.setPosition(pos);
        return temporary;
    }

    inline void makeGrid()
    {
        const float cellDimm(window_size.y / static_cast<float>(std::max(column_count, row_count)));
        const sf::Vector2f cellSize(cellDimm, cellDimm);

        const sf::Vector2f boardPos(board_region.left, board_region.top);

        for (std::size_t c(0); c < column_count; ++c)
        {
            for (std::size_t r(0); r < row_count; ++r)
            {
                const sf::Vector2f cellPos((c * cellSize.x), (r * cellSize.y));

                float left(board_region.width);

                const sf::Vector2f centerOffset(, );

                rectangles.push_back(makeRectangleShape((boardPos + cellPos), cellSize));
            }
        }
    }
};

int main()
{

    sf::Texture lazyMethHeadTexture;
    sf::Texture greedyMethHeadTexture;
    sf::Texture lootCheapTexture;
    sf::Texture lootValuableTexture;

    lazyMethHeadTexture.loadFromFile("head-1.png");
    greedyMethHeadTexture.loadFromFile("head-2.png");
    lootCheapTexture.loadFromFile("loot.png");

    sf::Sprite lazyMethHeadSprite(lazyMethHeadTexture);
    sf::Sprite greedyMethHeadSprite(greedyMethHeadTexture);
    sf::Sprite lootCheapSprite(lootCheapTexture);

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Meth Heads", sf::Style::Fullscreen);

    const BoardView boardView(window.getSize());

    // sf::RectangleShape rectIWant = boardView.makeRectangleShape(sf::Vector2f(100.0f, 100.0f));

    const sf::Vector2f lazyMethHeadPos(
        (boardView.window_size.x * 0.25f),
        (greedyMethHeadSprite.getGlobalBounds().top
         + greedyMethHeadSprite.getGlobalBounds().height));

    const sf::Vector2f lootCheapPos(
        0,
        (greedyMethHeadSprite.getGlobalBounds().top
         + greedyMethHeadSprite.getGlobalBounds().height));

    const sf::Vector2f lootValuablePos((boardView.window_size.x * 0.25f), 0);

    const sf::Vector2f greedyMethHeadStartPos;

    using Pos_t = sf::Vector2i;

    lazyMethHeadSprite.setPosition(lazyMethHeadPos);
    lootCheapSprite.setPosition(lootCheapPos);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if ((event.type == sf::Event::Closed) || (event.type == sf::Event::MouseButtonPressed))
            {
                window.close();
            }
        }

        window.clear();
        for (const auto & rectangle : boardView.rectangles)
        {
            window.draw(rectangle);
        }

        // window.draw(lazyMethHeadSprite);
        // window.draw(greedyMethHeadSprite);
        // window.draw(lootCheapSprite);
        window.display();
    }
}
