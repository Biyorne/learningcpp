#ifndef METH_HEAD_CELL_HPP_INCLUDED
#define METH_HEAD_CELL_HPP_INCLUDED

#include "error-handling.hpp"
#include "meth-head-enum.hpp"

#include <SFML/Graphics.hpp>

namespace methhead
{
    // TODO this should be a class.
    struct Cell
    {
        // TODO Get rid of this.
        Cell()
            : board_pos(-1, -1)
            , rectangle()
            , motivation(methhead::Motivation::none)
            , loot(0)
            , is_valid(false)
        {}

        Cell(
            const sf::Vector2i & boardPos,
            const sf::Vector2f & windowPos,
            const sf::Vector2f & size,
            const float lineThickness)
            : board_pos(boardPos)
            , rectangle()
            , motivation(methhead::Motivation::none)
            , loot(0)
            , is_valid(true)
        {
            rectangle.setPosition(windowPos);
            rectangle.setSize(size);
            rectangle.setOutlineThickness(lineThickness);
            rectangle.setFillColor(background_color);
            rectangle.setOutlineColor(line_color);
        }

        sf::FloatRect bounds() const { return rectangle.getGlobalBounds(); }

        bool isOccupied() const { return (Motivation::none != motivation) || (loot > 0); }

        sf::Vector2i board_pos;
        sf::RectangleShape rectangle;
        methhead::Motivation motivation; // TODO is this the right way to track where actors are?
        int loot;
        bool is_valid; // TODO this should go away

        static inline sf::Color background_color { 32, 32, 32 };
        static inline sf::Color line_color { 220, 220, 220 };
    };

    using BoardMap_t = std::map<sf::Vector2i, Cell>;

} // namespace methhead

#endif // METH_HEAD_CELL_HPP_INCLUDED
