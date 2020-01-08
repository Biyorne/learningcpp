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
    // TODO move to board class
    using BoardPos_t = sf::Vector2i;

    const BoardPos_t InvalidBoardPos = { std::numeric_limits<int>::lowest(),
                                         std::numeric_limits<int>::lowest() };

    enum class Piece
    {
        Empty,
        Hero,
        Demon,
        Child,
        Wall
    };

    inline std::string pieceName(const Piece piece)
    {
        switch (piece)
        {
            case Piece::Empty: return "None";
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