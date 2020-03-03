#ifndef BOARDGAME_TYPES_HPP_INCLUDED
#define BOARDGAME_TYPES_HPP_INCLUDED
//
// types.hpp
//
#include <ostream>
#include <string>

namespace boardgame
{
    enum class Piece
    {
        Head = 0,
        Tail,
        Food,
        Wall,
        Count
    };

    inline std::string toString(const Piece piece)
    {
        switch (piece)
        {
            case Piece::Head: return "Head";
            case Piece::Tail: return "Tail";
            case Piece::Food: return "Food";
            case Piece::Wall: return "Wall";
            case Piece::Count:
            default: return "";
        }
    }

    inline std::ostream & operator<<(std::ostream & os, const Piece piece)
    {
        os << toString(piece);
        return os;
    }
} // namespace boardgame

#endif // #define BOARDGAME_TYPES_HPP_INCLUDED