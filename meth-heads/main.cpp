#include "meth-head-enum.hpp"
#include "meth-head.hpp"
#include "utils.hpp"

#include <SFML/Graphics.hpp>

#include <cmath>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>

// Factors in lazy vs greedy
//    Simulation run time
//    Value maximum
//    Number of cell spaces
//    Number of loot items on screen at a time

// Things that can be in a single cell
//    sf::FloatRect AND Loot value OR one Meth Head

namespace sf
{
    bool operator<(const sf::Vector2i & L, const sf::Vector2i & R);
}

struct CellPositions
{
    CellPositions(const sf::Vector2i & boardParam, const sf::Vector2f & screenParam)
        : board(boardParam)
        , screen(screenParam)
    {}

    sf::Vector2i board;
    sf::Vector2f screen;
};

struct DisplayConstants
{
    explicit DisplayConstants(const sf::Vector2u & windowSize)
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
        , cell_dimm(window_size.y / static_cast<float>(std::max(column_count, row_count)))
        , cell_size(cell_dimm, cell_dimm)
        , score_rectangle_width(score_region.width * 0.5f)
        , positions(makeCellPositions())
        , rectangles(makeGrid())
    {}

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
    float cell_dimm;
    sf::Vector2f cell_size;
    float score_rectangle_width;
    std::vector<CellPositions> positions;
    std::vector<sf::RectangleShape> rectangles;

private:
    const sf::Vector2f cellToScreenPos(const sf::Vector2i & cellPos);

    const sf::RectangleShape
        makeGridRectangleShape(const sf::Vector2f & pos, const sf::Vector2f & size) const;

    std::vector<CellPositions> makeCellPositions();

    std::vector<sf::RectangleShape> makeGrid();
};

struct CellContent
{
    CellContent(const sf::Vector2f & pos, const sf::Vector2f & size)
        : region(pos, size)
        , meth_head_iden(methhead::MethHeadIden::none)
        , loot(0)
    {}

    sf::FloatRect region;
    methhead::MethHeadIden meth_head_iden;
    int loot;
};

// function declarations

void scoreBarSetup(
    std::size_t lazyScore,
    std::size_t greedyScore,
    sf::RectangleShape & lazyScoreRectangle,
    sf::RectangleShape & greedyScoreRectangle,
    const DisplayConstants & displayConstants);

//

int main()
{
    sf::Texture lootTexture;

    sf::Texture lazyMethHeadTexture;
    sf::Texture greedyMethHeadTexture;

    lazyMethHeadTexture.loadFromFile("head-1.png");
    greedyMethHeadTexture.loadFromFile("head-2.png");
    lootTexture.loadFromFile("loot.png");

    sf::Sprite lazyMethHeadSprite(lazyMethHeadTexture);
    sf::Sprite greedyMethHeadSprite(greedyMethHeadTexture);
    sf::Sprite lootSprite(lootTexture);

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Meth Heads", sf::Style::Fullscreen);

    const DisplayConstants displayConstants(window.getSize());

    std::map<sf::Vector2i, CellContent> cellPosToContent;

    for (const CellPositions & cellPositions : displayConstants.positions)
    {
        cellPosToContent.insert(std::make_pair(
            cellPositions.board, CellContent(cellPositions.screen, displayConstants.cell_size)));
    }

    cellPosToContent.find(sf::Vector2i(1, 1))->second.loot = 100;
    cellPosToContent.find(sf::Vector2i(5, 5))->second.loot = 100;
    cellPosToContent.find(sf::Vector2i(10, 10))->second.loot = 100;

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

        lazyScore += 2;
        greedyScore += 3;

        scoreBarSetup(
            lazyScore, greedyScore, lazyScoreRectangle, greedyScoreRectangle, displayConstants);

        window.clear();
        for (const auto & rectangle : displayConstants.rectangles)
        {
            window.draw(rectangle);
        }

        window.draw(lazyScoreRectangle);
        window.draw(greedyScoreRectangle);

        for (const auto & posContentPair : cellPosToContent)
        {
            if (posContentPair.second.loot > 0)
            {
                methhead::setSpriteRegion(lootSprite, posContentPair.second.region);
                window.draw(lootSprite);
            }
        }

        // window.draw(lazyMethHeadSprite);
        // window.draw(greedyMethHeadSprite);
        // window.draw(lootSprite);
        window.display();
    }
}

// function definitions

namespace sf
{
    bool operator<(const sf::Vector2i & L, const sf::Vector2i & R)
    {
        return std::tie(L.x, L.y) < std::tie(R.x, R.y);
    }
} // namespace sf

void scoreBarSetup(
    std::size_t lazyScore,
    std::size_t greedyScore,
    sf::RectangleShape & lazyScoreRectangle,
    sf::RectangleShape & greedyScoreRectangle,
    const DisplayConstants & displayConstants)
{
    if (lazyScore > greedyScore)
    {
        if (0 == lazyScore)
        {
            lazyScore = 1;
        }

        const float heightRatio(static_cast<float>(greedyScore) / static_cast<float>(lazyScore));

        lazyScoreRectangle.setSize(
            sf::Vector2f(displayConstants.score_rectangle_width, displayConstants.window_size.y));

        greedyScoreRectangle.setSize(sf::Vector2f(
            displayConstants.score_rectangle_width,
            (lazyScoreRectangle.getSize().y * heightRatio)));
    }
    else
    {
        if (0 == greedyScore)
        {
            greedyScore = 1;
        }

        const float heightRatio(static_cast<float>(lazyScore) / static_cast<float>(greedyScore));

        greedyScoreRectangle.setSize(
            sf::Vector2f(displayConstants.score_rectangle_width, displayConstants.window_size.y));

        lazyScoreRectangle.setSize(sf::Vector2f(
            displayConstants.score_rectangle_width,
            (greedyScoreRectangle.getSize().y * heightRatio)));
    }

    lazyScoreRectangle.setPosition(
        displayConstants.score_region.left,
        (displayConstants.score_region.height - lazyScoreRectangle.getSize().y));

    greedyScoreRectangle.setPosition(
        (displayConstants.score_region.left + displayConstants.score_rectangle_width),
        (displayConstants.score_region.height - greedyScoreRectangle.getSize().y));
}

const sf::Vector2f DisplayConstants::cellToScreenPos(const sf::Vector2i & cellPos)
{
    const sf::Vector2f boardPos(board_region.left, board_region.top);

    const sf::Vector2f gridTotalSize(
        (cell_size.x * static_cast<float>(column_count)),
        (cell_size.y * static_cast<float>(row_count)));

    const float centerOffsetHoriz((board_region.width - gridTotalSize.x) * 0.5f);
    const float centerOffsetVert((board_region.height - gridTotalSize.y) * 0.5f);
    const sf::Vector2f centerOffset(centerOffsetHoriz, centerOffsetVert);

    sf::Vector2f pos(
        (static_cast<float>(cellPos.x) * cell_size.x),
        (static_cast<float>(cellPos.y) * cell_size.y));

    pos += (boardPos + centerOffset);
    return pos;
}

const sf::RectangleShape DisplayConstants::makeGridRectangleShape(
    const sf::Vector2f & pos, const sf::Vector2f & size) const
{
    sf::RectangleShape temporary(size);
    temporary.setFillColor(cell_background_color);
    temporary.setOutlineColor(cell_line_color);
    temporary.setOutlineThickness(line_thickness);
    temporary.setPosition(pos);
    return temporary;
}

std::vector<CellPositions> DisplayConstants::makeCellPositions()
{
    std::vector<CellPositions> cellPositions;
    for (std::size_t c(0); c < column_count; ++c)
    {
        for (std::size_t r(0); r < row_count; ++r)
        {
            const sf::Vector2i cellPos(static_cast<int>(c), static_cast<int>(r));
            const sf::Vector2f cellScreenPos(cellToScreenPos(cellPos));
            const CellPositions cellPosition(cellPos, cellScreenPos);
            cellPositions.push_back(cellPosition);
        }
    }
    return cellPositions;
}

std::vector<sf::RectangleShape> DisplayConstants::makeGrid()
{
    std::vector<sf::RectangleShape> rectangleStack;

    for (const CellPositions & cellPositions : positions)
    {
        rectangleStack.push_back(makeGridRectangleShape((cellPositions.screen), cell_size));
    }

    return rectangleStack;
}
