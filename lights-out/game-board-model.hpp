#ifndef LIGHTS_OUT_GAME_BOARD_MODEL_HPP_INCLUDED
#define LIGHTS_OUT_GAME_BOARD_MODEL_HPP_INCLUDED

#include "cell-model.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <vector>

namespace lightsout
{

    class GameBoardModel
    {
    public:
        explicit GameBoardModel(const sf::FloatRect & REGION);

        std::vector<CellModel> cells() const { return m_cells; }

        void handleMouseClick(const sf::Vector2f MOUSE_POSITION_V);

        bool areAllCellsOff() const;

        void reset();

    private:
        void undo();

        void flip();

        std::vector<GridPos_t>
            makeGridPosToChange(const GridPos_t & GRID_POS_OF_CLICKED_CELL) const;

        void changeCellsWithTheseGridPositions(const std::vector<GridPos_t> & GRID_POS_TO_CHANGE);

        std::vector<CellModel> m_cells;
        std::vector<std::vector<CellModel>> m_history;
    };

} // namespace lightsout

#endif // LIGHTS_OUT_GAME_BOARD_MODEL_HPP_INCLUDED
