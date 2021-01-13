#ifndef CASTLECRAWL_PLAYER_HPP_INCLUDED
#define CASTLECRAWL_PLAYER_HPP_INCLUDED
//
// player.hpp
//
#include "pieces.hpp"

namespace castlecrawl
{

    class Player : public PieceBase
    {
      public:
        Player(Context & context, const MapPos_t & pos);
        void handleEvent(Context &, const sf::Event &) override;
    };

} // namespace castlecrawl

#endif // CASTLECRAWL_PLAYER_HPP_INCLUDED
