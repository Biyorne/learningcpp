// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// meth-head.cpp
//
#include "pieces.hpp"

#include "check-macros.hpp"
#include "context.hpp"
#include "keys.hpp"
#include "random.hpp"
#include "resources.hpp"
#include "settings.hpp"
#include "sound-player.hpp"

#include <cassert>

namespace boardgame
{

    PieceBase::PieceBase(const MapPos_t & pos, const char mapChar, const bool isObstacle)
        : m_mapChar(mapChar)
        , m_isObstacle(isObstacle)
        , m_position(pos)
    {}

} // namespace boardgame
