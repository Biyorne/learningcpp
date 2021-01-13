#ifndef CASTLECRAWL_MAPTYPES_HPP_INCLUDED
#define CASTLECRAWL_MAPTYPES_HPP_INCLUDED
//
// map-types.hpp
//
#include <string>
#include <vector>

#include <SFML/Graphics/Rect.hpp>

namespace castlecrawl
{
    using MapPos_t = sf::Vector2i;
    using MapChars_t = std::vector<std::string>;

} // namespace castlecrawl

#endif // CASTLECRAWL_MAPTYPES_HPP_INCLUDED
