#include "meth-head-enum.hpp"
#include "meth-head.hpp"
#include "utils.hpp"

#include <cmath>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>

#include <SFML/Graphics.hpp>

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
        , cellCountU(static_cast<unsigned int>(column_count), static_cast<unsigned int>(row_count))
        , cellCountI(cellCountU)
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
    sf::Vector2u cellCountU;
    sf::Vector2i cellCountI;
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
    CellContent()
        : region()
        , meth_head_iden(methhead::MethHeadIden::none)
        , loot(0)
        , isValid(false)
    {}

    CellContent(const sf::Vector2f & pos, const sf::Vector2f & size)
        : region(pos, size)
        , meth_head_iden(methhead::MethHeadIden::none)
        , loot(0)
        , isValid(true)
    {}

    sf::FloatRect region;
    methhead::MethHeadIden meth_head_iden;
    int loot;
    bool isValid;
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
    // Banana b;
    // doSomethingTo(&b);

    sf::Texture lootTexture;
    lootTexture.loadFromFile("loot.png");

    sf::Sprite lootSprite(lootTexture);

    sf::VideoMode videoMode(1024, 768, sf::VideoMode::getDesktopMode().bitsPerPixel);
    sf::RenderWindow window(videoMode, "Meth Heads", sf::Style::Default);
    // window.setVerticalSyncEnabled(true);
    // window.setFramerateLimit(60);

    const DisplayConstants displayConstants(window.getSize());

    std::map<sf::Vector2i, CellContent> gameBoard;

    methhead::MethHead lazy(
        methhead::MethHeadIden::lazy,
        "head-1.png",
        sf::Vector2i(0, 0),
        gameBoard[sf::Vector2i(0, 0)].region);

    methhead::MethHead greedy(
        methhead::MethHeadIden::greedy,
        "head-2.png",
        displayConstants.cellCountI,
        gameBoard[displayConstants.cellCountI].region);

    for (const CellPositions & cellPositions : displayConstants.positions)
    {
        gameBoard.insert(std::make_pair(
            cellPositions.board, CellContent(cellPositions.screen, displayConstants.cell_size)));
    }

    gameBoard.find(sf::Vector2i(1, 1))->second.loot = 100;
    gameBoard.find(sf::Vector2i(5, 5))->second.loot = 100;
    gameBoard.find(sf::Vector2i(10, 10))->second.loot = 100;

    // Score Column Drawing Here
    std::size_t lazyScore(1);
    std::size_t greedyScore(1);

    sf::RectangleShape lazyScoreRectangle;
    lazyScoreRectangle.setFillColor(displayConstants.lazy_color);

    sf::RectangleShape greedyScoreRectangle;
    greedyScoreRectangle.setFillColor(displayConstants.greedy_color);

    const float secondsPerTurn(1.0f);

    sf::Clock frameClock;
    std::size_t frameCount(0);

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

        ++frameCount;
        const float elapsedTimeSec(frameClock.getElapsedTime().asSeconds());

        if (elapsedTimeSec > secondsPerTurn)
        {
            std::cout << "FPS: " << (static_cast<float>(frameCount) / elapsedTimeSec) << std::endl;

            frameCount = 0;

            // Take turn
            frameClock.restart();

            // Temp changing score to make framerate visible
            lazyScore += 2;
            greedyScore += 3;
        }

        scoreBarSetup(
            lazyScore, greedyScore, lazyScoreRectangle, greedyScoreRectangle, displayConstants);

        window.clear();
        for (const auto & rectangle : displayConstants.rectangles)
        {
            window.draw(rectangle);
        }

        window.draw(lazyScoreRectangle);
        window.draw(greedyScoreRectangle);

        for (const auto & posContentPair : gameBoard)
        {
            if (posContentPair.second.loot > 0)
            {
                methhead::setSpriteRegion(lootSprite, posContentPair.second.region);
                window.draw(lootSprite);
            }
        }

        // window.draw(lazy);

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
