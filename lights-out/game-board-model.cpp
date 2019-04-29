// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "game-board-model.hpp"

#include "screen-util.hpp"

namespace lightsout
{

    GameBoardModel::GameBoardModel(const sf::FloatRect & REGION)
        : m_cells()
        , m_history()
        , m_random()
    {
        for (const GridRegion & GRID_REGION : splitRegionIntoGrids(REGION, 5, 5))
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

    void GameBoardModel::resetForValidNewGame()
    {
        m_history.clear();

        for (auto & cell : m_cells)
        {
            cell.set(m_random.rollBool());
        }

        if (areAllCellsOff())
        {
            const std::size_t CELLS_TO_TURN_ON(m_random.rollInteger(m_cells.size() - 1) + 1);

            for (std::size_t i(0); i < CELLS_TO_TURN_ON; ++i)
            {
                m_random.select(m_cells).set(true);
            }
        }
    }

    std::vector<GridPos_t>
        GameBoardModel::makeGridPosToChange(const GridPos_t & GRID_POS_OF_CLICKED_CELL) const
    {
        std::vector<GridPos_t> cellGridPosToChange;

        cellGridPosToChange.push_back(GRID_POS_OF_CLICKED_CELL);
        cellGridPosToChange.push_back(GRID_POS_OF_CLICKED_CELL + GridPos_t(-1, 0));
        cellGridPosToChange.push_back(GRID_POS_OF_CLICKED_CELL + GridPos_t(1, 0));
        cellGridPosToChange.push_back(GRID_POS_OF_CLICKED_CELL + GridPos_t(0, -1));
        cellGridPosToChange.push_back(GRID_POS_OF_CLICKED_CELL + GridPos_t(0, 1));

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

} // namespace lightsout
