#include "game-board.hpp"

GameBoard::GameBoard(const sf::FloatRect & REGION, const sf::Color & COLOR)
    : m_cells()
{
    const std::size_t CELL_COUNT_HORIZ(3);
    const std::size_t CELL_COUNT_VERT(CELL_COUNT_HORIZ);
    const std::size_t CELL_COUNT(CELL_COUNT_HORIZ * CELL_COUNT_VERT);
    m_cells.reserve(CELL_COUNT);

    const float PAD_SCREEN_FRACTION(0.01f);
    const float SCREEN_DIMENSION_AVG((REGION.width + REGION.height) * 0.5f);
    const float CELL_PAD(SCREEN_DIMENSION_AVG * PAD_SCREEN_FRACTION);

    const float CELL_WIDTH((REGION.width - ((CELL_COUNT_HORIZ + 1) * CELL_PAD)) / CELL_COUNT_HORIZ);
    const float CELL_HEIGHT((REGION.height - ((CELL_COUNT_VERT + 1) * CELL_PAD)) / CELL_COUNT_VERT);

    const sf::Vector2f CELL_SIZE_V(CELL_WIDTH, CELL_HEIGHT);

    for (std::size_t row(0); row < CELL_COUNT_VERT; ++row)
    {
        const float ROW_FLOAT(static_cast<float>(row));

        for (std::size_t column(0); column < CELL_COUNT_HORIZ; ++column)
        {
            const float COLUMN_FLOAT(static_cast<float>(column));

            const float LEFT_OFFSET(
                (CELL_PAD * (COLUMN_FLOAT + 1.0f)) + (CELL_WIDTH * COLUMN_FLOAT));
            const float TOP_OFFSET((CELL_PAD * (ROW_FLOAT + 1.0f)) + (CELL_HEIGHT * ROW_FLOAT));
            const sf::Vector2f CELL_POSITION_V(
                (REGION.left + LEFT_OFFSET), (REGION.top + TOP_OFFSET));

            const GridPos_t CELL_GRID_POSITION_V(static_cast<int>(column), static_cast<int>(row));

            const sf::FloatRect CELL_REGION(CELL_POSITION_V, CELL_SIZE_V);
            Cell cell(CELL_REGION, CELL_GRID_POSITION_V, COLOR);

            m_cells.push_back(cell);
        }
    }
}

bool GameBoard::isGameBoardOver() const
{
    return std::none_of(
        std::begin(m_cells), std::end(m_cells), [](const Cell & CELL) { return CELL.isOn(); });
}

void GameBoard::handleMouseClick(const sf::Vector2f MOUSE_POSITION_V)
{
    const std::vector<Cell>::const_iterator ITER_TO_CELL_CLICKED(std::find_if(
        std::begin(m_cells), std::end(m_cells), [&MOUSE_POSITION_V](const Cell & CELL) {
            return CELL.doesContain(MOUSE_POSITION_V);
        }));

    if (ITER_TO_CELL_CLICKED != std::cend(m_cells))
    {
        const GridPos_t GRID_POS_OF_CLICKED_CELL(ITER_TO_CELL_CLICKED->gridPos());

        const std::vector<GridPos_t> CELL_GRID_POS_TO_CHANGE(
            makeGridPosToChange(GRID_POS_OF_CLICKED_CELL));

        changeCellsWithTheseGridPositions(CELL_GRID_POS_TO_CHANGE);
    }
}

void GameBoard::undo()
{
    if (m_history.empty() == false)
    {
        m_cells = m_history.back();
        m_history.pop_back();
    }
}

void GameBoard::flip()
{
    m_history.push_back(m_cells);
    for (Cell & cell : m_cells)
    {
        cell.change();
    }
}

void GameBoard::reset()
{
    while (m_history.empty() == false)
    {
        undo();
    }
}

std::vector<GridPos_t>
    GameBoard::makeGridPosToChange(const GridPos_t & GRID_POS_OF_CLICKED_CELL) const
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

void GameBoard::changeCellsWithTheseGridPositions(const std::vector<GridPos_t> & GRID_POS_TO_CHANGE)
{
    m_history.push_back(m_cells);

    for (const GridPos_t & GRID_POS_V : GRID_POS_TO_CHANGE)
    {
        auto iterToCellWithGridPos(
            std::find_if(std::begin(m_cells), std::end(m_cells), [&GRID_POS_V](const Cell & CELL) {
                return (CELL.gridPos() == GRID_POS_V);
            }));

        if (iterToCellWithGridPos != std::end(m_cells))
        {
            iterToCellWithGridPos->change();
        }
    }
}
