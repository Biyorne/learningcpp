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
    using Score_t = int;

    using BoardPos_t = sf::Vector2i;

    struct Piece
    {
        enum Enum : std::size_t
        {
            Player = 0,
            Demon,
            Victim,
            Wall,
            Count
        };

        static std::string name(const Enum piece)
        {
            switch (piece)
            {
                case Player: return "Player";
                case Demon: return "Demon";
                case Victim: return "Victim";
                case Wall: return "Wall";
                case Count: return "Count";
                default: return "";
            }
        }
    };

    // these are used by the pieces that move to help find where they want to go
    struct WalkDistance
    {
        int dist{ 0 };
        BoardPos_t pos{ 0, 0 };
    };

    using MovesArray_t = std::array<WalkDistance, 4>;
} // namespace boardgame

#endif // #define BOARDGAME_TYPES_HPP_INCLUDED