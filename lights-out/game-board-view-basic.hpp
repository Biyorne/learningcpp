#ifndef LIGHTS_OUT_GAME_BOARD_VIEW_BASIC_HPP_INCLUDED
#define LIGHTS_OUT_GAME_BOARD_VIEW_BASIC_HPP_INCLUDED

#include "game-board-model.hpp"
#include "window.hpp"

namespace lightsout
{
    class GameBoardViewBasic
    {
    public:
        void draw(const GameBoardModel & GAME_BOARD, Window & window) const
        {
            window.clear();
            for (const CellModel & CELL : GAME_BOARD.cells())
            {
                if (CELL.isOn())
                {
                    window.drawRectangle(CELL.region(), sf::Color::Red);
                }
                else
                {
                    window.drawRectangle(CELL.region(), sf::Color::Blue);
                }
            }
            window.display();
        }
    };
} // namespace lightsout

#endif // LIGHTS_OUT_GAME_BOARD_VIEW_BASIC_HPP_INCLUDED
