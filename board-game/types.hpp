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
        Cell = 0,
        Count
    };

    inline std::string toString(const Piece piece)
    {
        switch (piece)
        {
            case Piece::Cell: return "Cell";
            case Piece::Count:
            default: return "";
        }
    }

    inline std::ostream & operator<<(std::ostream & os, const Piece piece)
    {
        os << toString(piece);
        return os;
    }

    inline Piece mapCharToPiece(const char) { return Piece::Cell; }
} // namespace boardgame

#endif // #define BOARDGAME_TYPES_HPP_INCLUDED