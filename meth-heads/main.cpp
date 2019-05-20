#include <SFML/Graphics.hpp>

#include <cmath>
#include <iostream>

// Factors in lazy vs greedy
//    Simulation run time
//    Value maximum
//    Number of cell spaces
//    Number of loot items on screen at a time

struct DisplayConstants
{
    DisplayConstants(const sf::Vector2u & windowSize)
        : pad_ratio(0.1f)
        , window_size(windowSize)
        , score_region(0.0f, 0.0f, (0.2f * window_size.x), window_size.y)
        , board_region(
              (score_region.left + score_region.width),
              score_region.top,
              (window_size.x - score_region.width),
              score_region.height)
        , cell_background_color { 32, 32, 32 }
        , cell_line_color { 220, 220, 220 }
        , lazy_color { 100, 100, 255 }
        , greedy_color { 100, 255, 100 }
        , prize_color { 255, 255, 100 }
        , column_count { 20 }
        , row_count { column_count }
        , line_thickness_ratio(1.0f / 600.0f)
        , line_thickness(line_thickness_ratio * window_size.y)
        , rectangles()
        , cell_dimm(window_size.y / static_cast<float>(std::max(column_count, row_count)))
        , cell_size(cell_dimm, cell_dimm)
        , score_rectangle_width(score_region.width * 0.5f)
    {
        makeGrid();
    }

    float pad_ratio;
    sf::Vector2f window_size;
    sf::FloatRect score_region;
    sf::FloatRect board_region;
    sf::Color cell_background_color;
    sf::Color cell_line_color;
    sf::Color lazy_color;
    sf::Color greedy_color;
    sf::Color prize_color;
    std::size_t column_count;
    std::size_t row_count;
    float line_thickness_ratio;
    float line_thickness;
    std::vector<sf::RectangleShape> rectangles;
    float cell_dimm;
    sf::Vector2f cell_size;
    float score_rectangle_width;

private:
    inline const sf::RectangleShape
        makeGridRectangleShape(const sf::Vector2f & pos, const sf::Vector2f & size) const
    {
        sf::RectangleShape temporary(size);
        temporary.setFillColor(cell_background_color);
        temporary.setOutlineColor(cell_line_color);
        temporary.setOutlineThickness(line_thickness);
        temporary.setPosition(pos);
        return temporary;
    }

    inline void makeGrid()
    {
        const sf::Vector2f boardPos(board_region.left, board_region.top);

        const sf::Vector2f gridTotalSize(
            (cell_size.x * static_cast<float>(column_count)),
            (cell_size.y * static_cast<float>(row_count)));

        const float centerOffsetHoriz((board_region.width - gridTotalSize.x) * 0.5f);
        const float centerOffsetVert((board_region.height - gridTotalSize.y) * 0.5f);
        const sf::Vector2f centerOffset(centerOffsetHoriz, centerOffsetVert);

        for (std::size_t c(0); c < column_count; ++c)
        {
            for (std::size_t r(0); r < row_count; ++r)
            {
                const sf::Vector2f cellPos((c * cell_size.x), (r * cell_size.y));

                float left(board_region.width);

                rectangles.push_back(
                    makeGridRectangleShape((boardPos + centerOffset + cellPos), cell_size));
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

    const DisplayConstants displayConstants(window.getSize());

    // Score Column Drawing Here
    std::size_t lazyScore(1);
    std::size_t greedyScore(1);

    sf::RectangleShape lazyScoreRectangle;
    lazyScoreRectangle.setFillColor(displayConstants.lazy_color);

    sf::RectangleShape greedyScoreRectangle;
    greedyScoreRectangle.setFillColor(displayConstants.greedy_color);

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

        // Setup Score Rectangles

        lazyScore += 2;
        greedyScore += 3;

        lazyScoreRectangle.setSize(
            sf::Vector2f(displayConstants.score_rectangle_width, static_cast<float>(lazyScore)));

        greedyScoreRectangle.setSize(
            sf::Vector2f(displayConstants.score_rectangle_width, static_cast<float>(greedyScore)));

        float greedyHeightRatio(greedyScoreRectangle.getSize().y / lazyScoreRectangle.getSize().y);

        lazyScoreRectangle.setPosition(
            displayConstants.score_region.left,
            (displayConstants.score_region.height - lazyScoreRectangle.getSize().y));

        greedyScoreRectangle.setPosition(
            (displayConstants.score_region.left + displayConstants.score_rectangle_width),
            (displayConstants.score_region.height - greedyScoreRectangle.getSize().y));

        window.clear();
        for (const auto & rectangle : displayConstants.rectangles)
        {
            window.draw(rectangle);
        }

        window.draw(lazyScoreRectangle);
        window.draw(greedyScoreRectangle);

        // window.draw(lazyMethHeadSprite);
        // window.draw(greedyMethHeadSprite);
        // window.draw(lootCheapSprite);
        window.display();
    }
}
