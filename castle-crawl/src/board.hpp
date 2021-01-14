#ifndef CASTLECRAWL_BOARD_HPP_INCLUDED
#define CASTLECRAWL_BOARD_HPP_INCLUDED
//
// board.hpp
//
#include "door.hpp"
#include "player.hpp"

#include <memory>
#include <vector>

#include <SFML/Graphics/RenderTarget.hpp>

namespace castlecrawl
{
    struct Board : public sf::Drawable
    {
        Player player;
        std::vector<Door> doors;

        void draw(sf::RenderTarget &, sf::RenderStates) const override;
    };

} // namespace castlecrawl

#endif // CASTLECRAWL_PLAYER_HPP_INCLUDED
