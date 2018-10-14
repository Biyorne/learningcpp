#include "game-board.hpp"
#include "screen-util.hpp"

namespace lightsout
{

    GameBoard::GameBoard(const sf::FloatRect & REGION, const sf::Color & COLOR)
        : m_cells()
        , m_isGameOver(false)
    {
        for (const GridRegion & GRID_REGION : splitRegionIntoGrids(REGION, 3, 3))
        {
            Cell cell(GRID_REGION.region, GRID_REGION.grid_pos, COLOR);
            m_cells.push_back(cell);
        }
    }

    bool GameBoard::areAllCellsOff() const
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

    void GameBoard::changeCellsWithTheseGridPositions(
        const std::vector<GridPos_t> & GRID_POS_TO_CHANGE)
    {
        m_history.push_back(m_cells);

        for (const GridPos_t & GRID_POS_V : GRID_POS_TO_CHANGE)
        {
            auto iterToCellWithGridPos(std::find_if(
                std::begin(m_cells), std::end(m_cells), [&GRID_POS_V](const Cell & CELL) {
                    return (CELL.gridPos() == GRID_POS_V);
                }));

            if (iterToCellWithGridPos != std::end(m_cells))
            {
                iterToCellWithGridPos->change();
            }
        }
    }

    void GameBoard::eventHandler(const sf::Event & EVENT)
    {
        if (EVENT.type == sf::Event::KeyPressed)
        {
            if (EVENT.key.code == sf::Keyboard::U)
            {
                undo();
            }
            else if (EVENT.key.code == sf::Keyboard::F)
            {
                flip();
            }
            else if (EVENT.key.code == sf::Keyboard::R)
            {
                reset();
            }
            else
            {
                m_isGameOver = true;
            }
        }

        if (EVENT.type == sf::Event::MouseButtonPressed)
        {
            const sf::Vector2f MOUSE_POSITION_V(
                sf::Vector2i(EVENT.mouseButton.x, EVENT.mouseButton.y));

            handleMouseClick(MOUSE_POSITION_V);
        }

        if (areAllCellsOff())
        {
            m_isGameOver = true;
        }
    }

} // namespace lightsout
