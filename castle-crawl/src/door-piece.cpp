// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// door.cpp
//
#include "door-piece.hpp"

namespace castlecrawl
{
    void DoorPiece::reset(Context & context, const MapPos_t & pos, const bool isLocked)
    {
        PieceBase::reset(context, pos, 'D', isLocked);
    }

} // namespace castlecrawl
