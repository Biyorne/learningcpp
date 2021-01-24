#ifndef CASTLECRAWL_MAPTYPES_HPP_INCLUDED
#define CASTLECRAWL_MAPTYPES_HPP_INCLUDED
//
// map-types.hpp
//
#include <string>
#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Vertex.hpp>

namespace castlecrawl
{
    using MapPos_t = sf::Vector2i;
    using MapChars_t = std::vector<std::string>;

    struct MapLink
    {
        MapLink(
            const MapPos_t & fromPos = { 0, 0 },
            const std::string & toName = "",
            const MapPos_t & toPos = { 0, 0 })
            : from_pos(fromPos)
            , to_name(toName)
            , to_pos(toPos)
        {}

        MapPos_t from_pos;
        std::string to_name;
        MapPos_t to_pos;
    };

    using MapLinks_t = std::vector<MapLink>;

    using VertVec_t = std::vector<sf::Vertex>;
} // namespace castlecrawl

#endif // CASTLECRAWL_MAPTYPES_HPP_INCLUDED
