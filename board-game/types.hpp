#ifndef BOARDGAME_TYPES_HPP_INCLUDED
#define BOARDGAME_TYPES_HPP_INCLUDED
//
// types.hpp
//
#include <array>

#include <SFML/System/Vector2.hpp>

namespace boardgame
{
    using BoardPos_t = sf::Vector2i;

    enum class Piece
    {
        None = 0,
        Hero,
        Demon,
        Child,
        Wall
    };

    inline std::string name(const Piece piece)
    {
        switch (piece)
        {
            case Piece::None: return "None";
            case Piece::Hero: return "Hero";
            case Piece::Demon: return "Demon";
            case Piece::Child: return "Child";
            case Piece::Wall: return "Wall";
            default: return "";
        }
    }

    // these are used by the pieces that move to help find where they want to go
    struct WalkDistance
    {
        int dist{ 0 };
        BoardPos_t pos{ 0, 0 };
    };

    using MovesArray_t = std::array<WalkDistance, 4>;
} // namespace boardgame

#endif // #define BOARDGAME_TYPES_HPP_INCLUDED