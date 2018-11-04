#include "game-board-model.hpp"
#include "screen-util.hpp"

namespace lightsout
{

    GameBoardModel::GameBoardModel(const sf::FloatRect & REGION)
        : m_cells()
        , m_history()
        , m_isGameOver(false)
    {
        for (const GridRegion & GRID_REGION : splitRegionIntoGrids(REGION, 3, 3))
        {
            CellModel cell(GRID_REGION.region, GRID_REGION.grid_pos);
            m_cells.push_back(cell);
        }
    }

    bool GameBoardModel::areAllCellsOff() const
    {
        return std::none_of(std::begin(m_cells), std::end(m_cells), [](const CellModel & CELL) {
            return CELL.isOn();
        });
    }

    void GameBoardModel::handleMouseClick(const sf::Vector2f MOUSE_POSITION_V)
    {
        const std::vector<CellModel>::const_iterator ITER_TO_CELL_CLICKED(std::find_if(
            std::begin(m_cells), std::end(m_cells), [&MOUSE_POSITION_V](const CellModel & CELL) {
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

    void GameBoardModel::undo()
    {
        if (m_history.empty() == false)
        {
            m_cells = m_history.back();
            m_history.pop_back();
        }
    }

    void GameBoardModel::flip()
    {
        m_history.push_back(m_cells);
        for (CellModel & cell : m_cells)
        {
            cell.change();
        }
    }

    void GameBoardModel::reset()
    {
        while (m_history.empty() == false)
        {
            undo();
        }
    }

    std::vector<GridPos_t>
        GameBoardModel::makeGridPosToChange(const GridPos_t & GRID_POS_OF_CLICKED_CELL) const
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

    void GameBoardModel::changeCellsWithTheseGridPositions(
        const std::vector<GridPos_t> & GRID_POS_TO_CHANGE)
    {
        m_history.push_back(m_cells);

        for (const GridPos_t & GRID_POS_V : GRID_POS_TO_CHANGE)
        {
            auto iterToCellWithGridPos(std::find_if(
                std::begin(m_cells), std::end(m_cells), [&GRID_POS_V](const CellModel & CELL) {
                    return (CELL.gridPos() == GRID_POS_V);
                }));

            if (iterToCellWithGridPos != std::end(m_cells))
            {
                iterToCellWithGridPos->change();
            }
        }
    }

    void GameBoardModel::eventHandler(const sf::Event & EVENT)
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
