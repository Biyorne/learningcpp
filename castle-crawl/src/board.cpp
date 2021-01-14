// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// pieces.cpp
//
#include "board.hpp"

#include <SFML/Graphics.hpp>

namespace castlecrawl
{

    void Board::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        for (const Door & door : doors)
        {
            target.draw(door, states);
        }

        target.draw(player, states);
    }
} // namespace castlecrawl
