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
        Eater = 0,
        Food,
        Wall,
        Count
    };

    inline std::string toString(const Piece piece)
    {
        switch (piece)
        {
            case Piece::Eater: return "Eater";
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

    inline Piece mapCharToPiece(const char mapChar)
    {
        switch (mapChar)
        {
            case 'E': return Piece::Eater;
            case 'F': return Piece::Food;
            case 'W': return Piece::Wall;
            case ' ':
            default: return Piece::Count;
        }
    }

    inline char pieceToMapChar(const Piece piece)
    {
        switch (piece)
        {
            case Piece::Eater: return 'E';
            case Piece::Food: return 'F';
            case Piece::Wall: return 'W';
            case Piece::Count:
            default: return ' ';
        }
    }

    inline sf::Color toColor(const Piece piece)
    {
        switch (piece)
        {
            case Piece::Eater: return sf::Color::Red;
            case Piece::Food: return sf::Color::Yellow;
            case Piece::Wall: return sf::Color(100, 60, 5); // brown
            case Piece::Count:
            default: return sf::Color::Transparent;
        }
    }
} // namespace boardgame
#endif // #define BOARDGAME_TYPES_HPP_INCLUDED