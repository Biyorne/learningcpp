#ifndef BOARDGAME_TYPES_HPP_INCLUDED
#define BOARDGAME_TYPES_HPP_INCLUDED
//
// types.hpp
//
#include <array>
#include <string>

#include <SFML/System/Vector2.hpp>

namespace boardgame
{
    using BoardPos_t = sf::Vector2i;

    enum class Piece
    {
        Empty,
        Player,
        Villan,
        Pickup,
        Obstacle,
    };
} // namespace boardgame

#endif // #define BOARDGAME_TYPES_HPP_INCLUDED