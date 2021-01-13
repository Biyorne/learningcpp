// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// door.cpp
//
#include "door.hpp"

//

namespace castlecrawl
{
    Door::Door(Context & context, const MapPos_t & pos)
        : PieceBase(context, pos, 'D', true)
    {}

} // namespace castlecrawl
