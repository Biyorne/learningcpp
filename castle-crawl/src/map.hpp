#ifndef CASTLECRAWL_MAP_HPP_INCLUDED
#define CASTLECRAWL_MAP_HPP_INCLUDED
//
// map.hpp
//
#include "context.hpp"
#include "map-types.hpp"

#include <map>
#include <vector>

#include <SFML/Graphics/RenderTarget.hpp>

namespace castlecrawl
{

    class Map
    {
      public:
        Map();
        Map(const Context &, const bool isFloorStone, const MapChars_t &, const MapLinks_t &);

        void load(Context & context);

        MapPos_t size() const;
        bool empty() const { return !((size().x > 0) && (size().y > 0)); }

        bool isPosValid(const MapPos_t & pos) const
        {
            return ((pos.x >= 0) && (pos.y >= 0) && (pos.x < size().x) && (pos.y < size().y));
        }

        bool isPosValid(const int x, const int y) const { return isPosValid(MapPos_t{ x, y }); }

        char getChar(const MapPos_t & pos) const;
        char getChar(const int x, const int y) const { return getChar(MapPos_t{ x, y }); }
        void setChar(const int x, const int y, const char newChar);

        void draw(const Context &, sf::RenderTarget &, sf::RenderStates) const;
        void drawChars(
            const Context &, sf::RenderTarget &, sf::RenderStates, const MapChars_t &) const;

        void drawBorderChars(
            const Context &, sf::RenderTarget &, sf::RenderStates, const MapChars_t &) const;

        void drawStoneTransitionChars(const Context &, sf::RenderTarget &, sf::RenderStates) const;

        const MapLinks_t & links() const { return m_links; }

      private:
        void addWalls();
        void addWallCorners();
        void randomizeFloorTiles(const Context & context);
        void makeDoors(Context &);

      private:
        bool m_isFloorStone;
        MapChars_t m_chars;
        MapChars_t m_floorChars;
        MapLinks_t m_links;
    };

    using NameMapPair_t = std::pair<std::string, Map>;
    using Maps_t = std::map<std::string, Map>;

} // namespace castlecrawl

#endif // CASTLECRAWL_MAP_HPP_INCLUDED
