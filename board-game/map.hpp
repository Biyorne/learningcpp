#ifndef BOARDGAME_MAP_HPP_INCLUDED
#define BOARDGAME_MAP_HPP_INCLUDED
//
// map.hpp
//
#include "context.hpp"
#include "random.hpp"
#include "types.hpp"

#include <string>

namespace boardgame
{

    class Map
    {
      public:
        void reset(Context & context, const MapChars_t & mapChars)
        {
            m_mapChars = mapChars;
            m_floorChars = mapChars;
            randomizeFloorTiles(context);
        }

        void randomizeFloorTiles(Context & context)
        {
            for (std::string & str : m_floorChars)
            {
                for (char & ch : str)
                {
                    if ('`' != ch)
                    {
                        ch = '0' + context.random.fromTo<char>(1, 6);
                    }
                }
            }
        }

        bool empty() const { return m_mapChars.empty(); }

        // private:
        MapChars_t m_mapChars;
        MapChars_t m_floorChars;
    };

} // namespace boardgame

#endif // BOARDGAME_MAP_HPP_INCLUDED
