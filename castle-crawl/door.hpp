#ifndef CASTLECRAWL_DOOR_HPP_INCLUDED
#define CASTLECRAWL_DOOR_HPP_INCLUDED
//
// door.hpp
//
#include "pieces.hpp"

//

namespace castlecrawl
{
    using MapPos_t = sf::Vector2i;
    struct Context;

    class Door : public PieceBase
    {
      public:
        Door(Context & context, const MapPos_t & pos);

        bool isLocked() const { return isObstacle(); }
        void isLocked(const bool isLocked) { m_isObstacle = isLocked; }
    };

} // namespace castlecrawl

#endif // CASTLECRAWL_PLAYER_HPP_INCLUDED
