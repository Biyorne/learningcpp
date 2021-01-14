#ifndef CASTLECRAWL_MAP_HPP_INCLUDED
#define CASTLECRAWL_MAP_HPP_INCLUDED
//
// map.hpp
//
#include "context.hpp"
#include "map-types.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace castlecrawl
{

    class Map
    {
      public:
        //
        void reset(const Context & context, const MapChars_t & mapChars);
        void randomizeFloorTiles(const Context & context);
        void addWallCorners();
        void addWalls();

        //
        MapPos_t size() const;
        bool empty() const { return !((size().x > 0) && (size().y > 0)); }

        bool isPosValid(const MapPos_t & pos) const
        {
            return ((pos.x >= 0) && (pos.y >= 0) && (pos.x < size().x) && (pos.y < size().y));
        }

        bool isPosValid(const int x, const int y) const { return isPosValid(MapPos_t{ x, y }); }

        //
        char getChar(const MapPos_t & pos) const;
        char getChar(const int x, const int y) const { return getChar(MapPos_t{ x, y }); }
        void setChar(const int x, const int y, const char newChar);

        //
        void draw(Context &, sf::RenderTarget &, sf::RenderStates) const;
        void drawChars(Context &, sf::RenderTarget &, sf::RenderStates, const MapChars_t &) const;

      private:
        MapChars_t m_mapChars;
        MapChars_t m_floorChars;
    };

} // namespace castlecrawl

#endif // CASTLECRAWL_MAP_HPP_INCLUDED
