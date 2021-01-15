// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// context.hpp
//
#include "context.hpp"

#include "maps.hpp"

namespace castlecrawl
{

    Map & Context::map() { return maps.maps()[map_name]; }

    const Map & Context::map() const { return maps.maps()[map_name]; }

} // namespace castlecrawl
