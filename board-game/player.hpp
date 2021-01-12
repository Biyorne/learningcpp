#ifndef BOARDGAME_PLAYER_HPP_INCLUDED
#define BOARDGAME_PLAYER_HPP_INCLUDED
//
// player.hpp
//
#include "context.hpp"
#include "map.hpp"
#include "pieces.hpp"

#include <SFML/Window/Event.hpp>

//

namespace boardgame
{

    class Player : public PieceBase
    {
      public:
        Player(Context & context, const MapPos_t & pos)
            : PieceBase(context, pos, 'p', false)
        {}

        void handleEvent(Context &, const sf::Event &) override;
    };

} // namespace boardgame

#endif // BOARDGAME_PLAYER_HPP_INCLUDED
