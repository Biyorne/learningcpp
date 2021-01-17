#ifndef CASTLECRAWL_DOOR_HPP_INCLUDED
#define CASTLECRAWL_DOOR_HPP_INCLUDED
//
// door.hpp
//
#include "pieces.hpp"

namespace castlecrawl
{
    class DoorPiece : public PieceBase
    {
      public:
        void reset(Context & context, const MapPos_t & pos, const bool isLocked);

        bool isLocked() const { return isObstacle(); }
        void isLocked(const bool isLocked) { m_isObstacle = isLocked; }
    };

} // namespace castlecrawl

#endif // CASTLECRAWL_PLAYER_HPP_INCLUDED
