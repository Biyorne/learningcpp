// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// meth-head.cpp
//
#include "cell.hpp"

namespace methhead
{

    Cell::Cell()
        : board_pos(-1, -1)
        , rectangle()
        , motivation(methhead::Motivation::none)
        , loot(0)
        , is_valid(false)
    {}

    Cell::Cell(
        const sf::Vector2i & boardPos, const sf::Vector2f & windowPos, const sf::Vector2f & size)
        : board_pos(boardPos)
        , rectangle()
        , motivation(methhead::Motivation::none)
        , loot(0)
        , is_valid(true)
    {
        rectangle.setPosition(windowPos);
        rectangle.setSize(size);
        rectangle.setOutlineThickness(size.x * 0.025f);
        rectangle.setFillColor(background_color);
        rectangle.setOutlineColor(line_color);
    }

} // namespace methhead
