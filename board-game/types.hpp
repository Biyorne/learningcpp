#ifndef BOARDGAME_TYPES_HPP_INCLUDED
#define BOARDGAME_TYPES_HPP_INCLUDED
//
// types.hpp
//
#include <ostream>
#include <string>

#include <SFML/Graphics/Color.hpp>

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

    inline Piece mapCharToPiece(const char mapChar)
    {
        if (mapChar == ' ')
        {
            return Piece::Count;
        }
        else
        {
            return Piece::Cell;
        }
    }

    inline char pieceToMapChar(const Piece piece)
    {
        switch (piece)
        {
            case Piece::Cell: return 'C';
            case Piece::Count:
            default: return ' ';
        }
    }

    inline sf::Color toColor(const Piece piece)
    {
        switch (piece)
        {
            case Piece::Cell: return sf::Color(110, 115, 235);
            case Piece::Count:
            default: return sf::Color::Black;
        }
    }
} // namespace boardgame
#endif // #define BOARDGAME_TYPES_HPP_INCLUDED