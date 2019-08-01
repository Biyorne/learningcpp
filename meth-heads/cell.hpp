#ifndef METH_HEAD_CELL_HPP_INCLUDED
#define METH_HEAD_CELL_HPP_INCLUDED

#include "error-handling.hpp"
#include "meth-head-enum.hpp"

#include <SFML/Graphics.hpp>

namespace methhead
{
    struct Cell
    {
        Cell()
            : board_pos(-1, -1)
            , rectangle()
            , motivation(methhead::Motivation::none)
            , loot(0)
            , is_valid(false)
        {}

        Cell(const sf::Vector2i & boardPos, const sf::RectangleShape & rectangleParam)
            : board_pos(boardPos)
            , rectangle(rectangleParam)
            , motivation(methhead::Motivation::none)
            , loot(0)
            , is_valid(true)

        {}

        sf::FloatRect bounds() const { return rectangle.getGlobalBounds(); }

        // TODO checking is_valid not intuitive or a good idea, but expecting to eliminate is_valid
        bool isOccupied() const { return (Motivation::none != motivation) || (loot > 0); }

        // TODO only added to find a bug REMOVE
        void throwIfOccupied() const
        {
            assertOrThrow(
                (is_valid),
                "MethHead::spawnLoot()'s makeUnoccupiedCellPositions() failed, because one of the "
                "returned positions was invalid.");

            assertOrThrow(
                (motivation == Motivation::none),
                "MethHead::spawnLoot()'s makeUnoccupiedCellPositions() failed, because one of the "
                "returned positions had a methhead in it.");

            assertOrThrow(
                (loot == 0),
                "MethHead::spawnLoot()'s makeUnoccupiedCellPositions() failed, because one of the "
                "returned positions had loot.");

            assertOrThrow(
                (!isOccupied()),
                "MethHead::spawnLoot()'s makeUnoccupiedCellPositions() failed, because one of the "
                "returned THOUGHT it was isOccupied() when it really was not!");
        }

        sf::Vector2i board_pos;
        sf::RectangleShape rectangle;
        methhead::Motivation motivation;
        int loot;
        bool is_valid;
    };

    using BoardMap_t = std::map<sf::Vector2i, Cell>;

} // namespace methhead

#endif // METH_HEAD_CELL_HPP_INCLUDED
