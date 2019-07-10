#ifndef METH_HEAD_CELL_CONTENT_HPP_INCLUDED
#define METH_HEAD_CELL_CONTENT_HPP_INCLUDED

#include "meth-head-enum.hpp"

#include <SFML/Graphics.hpp>

namespace methhead
{
    struct CellContent
    {
        CellContent()
            : region()
            , motivation(methhead::Motivation::none)
            , loot(0)
            , isValid(false)
        {}

        CellContent(const sf::Vector2f & pos, const sf::Vector2f & size)
            : region(pos, size)
            , motivation(methhead::Motivation::none)
            , loot(0)
            , isValid(true)
        {}

        sf::FloatRect region;
        methhead::Motivation motivation;
        int loot;
        bool isValid;
    };

    using BoardMap_t = std::map<sf::Vector2i, CellContent>;

} // namespace methhead

#endif // METH_HEAD_CELL_CONTENT_HPP_INCLUDED
