#ifndef CASTLECRAWL_PLAYER_HPP_INCLUDED
#define CASTLECRAWL_PLAYER_HPP_INCLUDED
//
// player-piece.hpp
//
#include "pieces.hpp"

namespace castlecrawl
{

    class PlayerPiece : public PieceBase
    {
      public:
        void reset(Context & context, const MapPos_t & pos);
        void handleEvent(Context &, const sf::Event &) override;
    };

} // namespace castlecrawl

#endif // CASTLECRAWL_PLAYER_HPP_INCLUDED
