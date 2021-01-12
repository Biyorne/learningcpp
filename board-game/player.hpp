#ifndef CASTLECRAWL_PLAYER_HPP_INCLUDED
#define CASTLECRAWL_PLAYER_HPP_INCLUDED
//
// player.hpp
//
#include "context.hpp"
#include "map.hpp"
#include "pieces.hpp"

#include <SFML/Window/Event.hpp>

//

namespace castlecrawl
{

    class Player : public PieceBase
    {
      public:
        Player(Context & context, const MapPos_t & pos)
            : PieceBase(context, pos, 'p', false)
        {}

        void handleEvent(Context &, const sf::Event &) override;
    };

} // namespace castlecrawl

#endif // CASTLECRAWL_PLAYER_HPP_INCLUDED
