// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// context.hpp
//
#include "context.hpp"

#include "board.hpp"
#include "check-macros.hpp"
#include "maps.hpp"
#include "settings.hpp"

namespace castlecrawl
{

    Map & Context::map() { return maps.maps()[map_name]; }

    const Map & Context::map() const { return maps.maps()[map_name]; }

    void Context::switchToMap(const MapLink & link)
    {
        const std::string fromMapName = map_name;
        map_name = link.to_name;

        M_CHECK_SS(
            (!map().empty()),
            "Map is empty after load() in Context::switchToMap: from=\""
                << fromMapName << "\" to " << link.to_pos << " in \"" << link.to_name << "\"");

        layout.setupBoard(map().size());
        map().load(*this);
        board.player.reset(*this, link.to_pos);
    }

} // namespace castlecrawl
