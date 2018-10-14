#ifndef LIGHTS_OUT_GAME_BOARD_HPP_INCLUDED
#define LIGHTS_OUT_GAME_BOARD_HPP_INCLUDED

#include "cell.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <vector>

namespace lightsout
{

    class GameBoard
    {
    public:
        GameBoard(const sf::FloatRect & REGION, const sf::Color & COLOR);

        std::vector<Cell> cells() const { return m_cells; }

        bool isGameOver() const { return m_isGameOver; }

        void handleMouseClick(const sf::Vector2f MOUSE_POSITION_V);

        void eventHandler(const sf::Event & EVENT);

        void update(const float ELAPSED_TIME)
        {
            for (Cell & cell : m_cells)
            {
                cell.update(ELAPSED_TIME);
            }
        }

    private:
        void undo();

        void flip();

        void reset();

        bool areAllCellsOff() const;

        std::vector<GridPos_t>
            makeGridPosToChange(const GridPos_t & GRID_POS_OF_CLICKED_CELL) const;

        void changeCellsWithTheseGridPositions(const std::vector<GridPos_t> & GRID_POS_TO_CHANGE);

        std::vector<Cell> m_cells;
        std::vector<std::vector<Cell>> m_history;
        bool m_isGameOver;
    };

} // namespace lightsout

#endif // LIGHTS_OUT_GAME_BOARD_HPP_INCLUDED
