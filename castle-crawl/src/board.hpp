#ifndef CASTLECRAWL_BOARD_HPP_INCLUDED
#define CASTLECRAWL_BOARD_HPP_INCLUDED
//
// board.hpp
//
#include "door-piece.hpp"
#include "player-piece.hpp"

#include <vector>

#include <SFML/Graphics/RenderTarget.hpp>

namespace castlecrawl
{
    struct Board : public sf::Drawable
    {
        PlayerPiece player;
        std::vector<DoorPiece> doors;

        void draw(sf::RenderTarget &, sf::RenderStates) const override;
    };

} // namespace castlecrawl

#endif // CASTLECRAWL_PLAYER_HPP_INCLUDED
