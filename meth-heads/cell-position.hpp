#ifndef METH_HEAD_CELL_POSITION_HPP_INCLUDED
#define METH_HEAD_CELL_POSITION_HPP_INCLUDED

#include <SFML/Graphics.hpp>

namespace methhead
{
    struct CellPositions
    {
        CellPositions(const sf::Vector2i & boardParam, const sf::Vector2f & screenParam)
            : board(boardParam)
            , screen(screenParam)
        {}

        sf::Vector2i board;
        sf::Vector2f screen;
    };

} // namespace methhead

#endif // METH_HEAD_CELL_POSITION_HPP_INCLUDED
