#ifndef LIGHTS_OUT_GAME_BOARD_HPP_INCLUDED
#define LIGHTS_OUT_GAME_BOARD_HPP_INCLUDED

#include "cell.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <vector>

class GameBoard
{
public:
    GameBoard(const sf::FloatRect & REGION, const sf::Color & COLOR);

    std::vector<Cell> cells() const { return m_cells; }

    bool isGameBoardOver() const;

    void handleMouseClick(const sf::Vector2f MOUSE_POSITION_V);

    void undo();

    void flip();

    void reset();

private:
    std::vector<GridPos_t> makeGridPosToChange(const GridPos_t & GRID_POS_OF_CLICKED_CELL) const;

    void changeCellsWithTheseGridPositions(const std::vector<GridPos_t> & GRID_POS_TO_CHANGE);

    std::vector<Cell> m_cells;
    std::vector<std::vector<Cell>> m_history;
};

#endif // LIGHTS_OUT_GAME_BOARD_HPP_INCLUDED
