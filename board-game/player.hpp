#ifndef BOARDGAME_PLAYER_HPP_INCLUDED
#define BOARDGAME_PLAYER_HPP_INCLUDED
//
// player.hpp
//
#include "context.hpp"
#include "map.hpp"

#include <SFML/Window/Event.hpp>

//

namespace boardgame
{

    class Player
    {
      public:
        MapPos_t position() const { return m_mapPos; }
        void handleEvent(Context & context, const sf::Event & event);

      private:
        MapPos_t m_mapPos;
    };

} // namespace boardgame

#endif // BOARDGAME_PLAYER_HPP_INCLUDED
