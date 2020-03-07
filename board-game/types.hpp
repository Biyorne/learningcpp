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
        On = 0,
        Off,
        Count
    };

    inline std::string toString(const Piece piece)
    {
        switch (piece)
        {
            case Piece::On: return "On";
            case Piece::Off: return "Off";
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
            return Piece::Off;
        }
        else
        {
            return Piece::On;
        }
    }

    inline sf::Color toColor(const Piece piece)
    {
        switch (piece)
        {
            case Piece::On: return sf::Color(110, 115, 235);
            case Piece::Off:
            case Piece::Count:
            default: return sf::Color::Black;
        }
    }
} // namespace boardgame
#endif // #define BOARDGAME_TYPES_HPP_INCLUDED