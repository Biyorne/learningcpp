#include "cell.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <iostream>
#include <vector>

std::vector<Cell> makeCells(const unsigned int SCREEN_WIDTH, const unsigned int SCREEN_HEIGHT);

void eventHandler(const sf::Event & EVENT, sf::RenderWindow & window, std::vector<Cell> & cells);

bool isGameOver(const std::vector<Cell> & CELLS);

void changeCellsWithTheseGridPositions(
    std::vector<Cell> & cells, const std::vector<GridPos_t> & GRID_POS_TO_CHANGE);

std::vector<GridPos_t> makeGridPosToChange(const GridPos_t & GRID_POS_OF_CLICKED_CELL);

void handleMouseClick(
    const sf::Vector2f MOUSE_POSITION_V, std::vector<Cell> & cells, sf::RenderWindow & window);

int main()
{
    const unsigned int SCREEN_WIDTH(800);
    const unsigned int SCREEN_HEIGHT(600);

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Lights Out");

    std::vector<Cell> cells(makeCells(SCREEN_WIDTH, SCREEN_HEIGHT));

    const sf::Color CELL_OUTLINE_COLOR(sf::Color::Black);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            eventHandler(event, window, cells);
        }

        window.clear(CELL_OUTLINE_COLOR);

        for (const Cell & CELL : cells)
        {
            window.draw(CELL.rectangle());
        }

        window.display();
    }

    return EXIT_SUCCESS;
}

std::vector<Cell> makeCells(const unsigned int SCREEN_WIDTH, const unsigned int SCREEN_HEIGHT)
{
    const std::size_t CELL_COUNT_HORIZ(3);
    const std::size_t CELL_COUNT_VERT(CELL_COUNT_HORIZ);
    const std::size_t CELL_COUNT(CELL_COUNT_HORIZ * CELL_COUNT_VERT);

    const float PAD_SCREEN_FRACTION(0.01f);
    const float SCREEN_DIMENSION_AVG(static_cast<float>(SCREEN_WIDTH + SCREEN_HEIGHT) * 0.5f);
    const float CELL_PAD(SCREEN_DIMENSION_AVG * PAD_SCREEN_FRACTION);

    const float CELL_WIDTH((SCREEN_WIDTH - ((CELL_COUNT_HORIZ + 1) * CELL_PAD)) / CELL_COUNT_HORIZ);
    const float CELL_HEIGHT((SCREEN_HEIGHT - ((CELL_COUNT_VERT + 1) * CELL_PAD)) / CELL_COUNT_VERT);

    const sf::Vector2f CELL_SIZE_V(CELL_WIDTH, CELL_HEIGHT);

    std::vector<Cell> cells;
    for (std::size_t row(0); row < CELL_COUNT_VERT; ++row)
    {
        const float ROW_FLOAT(static_cast<float>(row));

        for (std::size_t column(0); column < CELL_COUNT_HORIZ; ++column)
        {
            const float COLUMN_FLOAT(static_cast<float>(column));

            const float LEFT((CELL_PAD * (COLUMN_FLOAT + 1.0f)) + (CELL_WIDTH * COLUMN_FLOAT));
            const float TOP((CELL_PAD * (ROW_FLOAT + 1.0f)) + (CELL_HEIGHT * ROW_FLOAT));
            const sf::Vector2f CELL_POSITION_V(LEFT, TOP);

            const GridPos_t CELL_GRID_POSITION_V(static_cast<int>(column), static_cast<int>(row));

            Cell cell(CELL_SIZE_V, CELL_POSITION_V, CELL_GRID_POSITION_V);

            cells.push_back(cell);
        }
    }
    return cells;
}

void eventHandler(const sf::Event & EVENT, sf::RenderWindow & window, std::vector<Cell> & cells)
{
    if (EVENT.type == sf::Event::Closed)
    {
        window.close();
    }

    if (EVENT.type == sf::Event::KeyPressed)
    {
        window.close();
    }

    if (EVENT.type == sf::Event::MouseButtonPressed)
    {
        const sf::Vector2f MOUSE_POSITION_V(sf::Vector2i(EVENT.mouseButton.x, EVENT.mouseButton.y));
        handleMouseClick(MOUSE_POSITION_V, cells, window);
    }
}

void handleMouseClick(
    const sf::Vector2f MOUSE_POSITION_V, std::vector<Cell> & cells, sf::RenderWindow & window)
{
    const std::vector<Cell>::const_iterator ITER_TO_CELL_CLICKED(
        std::find_if(std::begin(cells), std::end(cells), [&MOUSE_POSITION_V](const Cell & CELL) {
            return CELL.doesContain(MOUSE_POSITION_V);
        }));

    if (ITER_TO_CELL_CLICKED != std::cend(cells))
    {
        const GridPos_t GRID_POS_OF_CLICKED_CELL(ITER_TO_CELL_CLICKED->gridPos());

        const std::vector<GridPos_t> CELL_GRID_POS_TO_CHANGE(
            makeGridPosToChange(GRID_POS_OF_CLICKED_CELL));

        changeCellsWithTheseGridPositions(cells, CELL_GRID_POS_TO_CHANGE);

        if (isGameOver(cells))
        {
            window.close();
        }
    }
}

bool isGameOver(const std::vector<Cell> & CELLS)
{
    return std::none_of(
        std::begin(CELLS), std::end(CELLS), [](const Cell & CELL) { return CELL.isOn(); });
}

void changeCellsWithTheseGridPositions(
    std::vector<Cell> & cells, const std::vector<GridPos_t> & GRID_POS_TO_CHANGE)
{
    for (const GridPos_t & GRID_POS_V : GRID_POS_TO_CHANGE)
    {
        auto iterToCellWithGridPos(
            std::find_if(std::begin(cells), std::end(cells), [&GRID_POS_V](const Cell & CELL) {
                return (CELL.gridPos() == GRID_POS_V);
            }));

        if (iterToCellWithGridPos != std::end(cells))
        {
            iterToCellWithGridPos->change();
        }
    }
}

std::vector<GridPos_t> makeGridPosToChange(const GridPos_t & GRID_POS_OF_CLICKED_CELL)
{
    std::vector<GridPos_t> cellGridPosToChange;
    for (int row(-1); row <= 1; ++row)
    {
        for (int column(-1); column <= 1; ++column)
        {
            const GridPos_t GRID_POS_V(
                GRID_POS_OF_CLICKED_CELL.x + column, GRID_POS_OF_CLICKED_CELL.y + row);

            cellGridPosToChange.push_back(GRID_POS_V);
        }
    }
    return cellGridPosToChange;
}
