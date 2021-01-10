#ifndef BOARDGAME_TYPES_HPP_INCLUDED
#define BOARDGAME_TYPES_HPP_INCLUDED
//
// types.hpp
//
#include <cstddef>
#include <string>
#include <vector>

#include <SFML/Graphics/Rect.hpp>

namespace boardgame
{
    using BoardPos_t = sf::Vector2i;
    using MapChars_t = std::vector<std::string>;

    // used to index so keep unsigned
    enum class Piece : std::size_t
    {
        Empty = 0,
        Floor1 = 1,
        Floor2,
        Floor3,
        Floor4,
        Floor5,
        Floor6,
        WallBlock,
        WallTopLeft,
        WallTop,
        WallHoriz,
        WallHorizShadow,
        WallVert,
        Player,
        Stairs,
        Barrel,
        Count
    };

    // inline std::string toString(const Piece piece)
    //{
    //    switch (piece)
    //    {
    //        case Piece::Eater: return "Eater";
    //        case Piece::Food: return "Food";
    //        case Piece::Wall: return "Wall";
    //        case Piece::Count:
    //        default: return "";
    //    }
    //}
    //
    // inline std::ostream & operator<<(std::ostream & os, const Piece piece)
    //{
    //    os << toString(piece);
    //    return os;
    //}

    inline Piece mapCharToPiece(const char mapChar)
    {
        switch (mapChar)
        {
            case 'B': return Piece::WallBlock;
            case 'C': return Piece::WallTopLeft;
            case 'T': return Piece::WallTop;
            case 'H': return Piece::WallHoriz;
            // skip shadow
            case 'V': return Piece::WallVert;
            case 'p': return Piece::Player;
            case 's': return Piece::Stairs;
            case 'b': return Piece::Barrel;
            case '1': return Piece::Floor1;
            case '2': return Piece::Floor2;
            case '3': return Piece::Floor3;
            case '4': return Piece::Floor4;
            case '5': return Piece::Floor5;
            case '6': return Piece::Floor6;
            default: return Piece::Empty;
        }
    }

    inline sf::IntRect pieceToTileRect(const Piece piece)
    {
        // clang-format off
        switch (piece)
        {
            case Piece::WallBlock:       return sf::IntRect(64,32,32,32);
            case Piece::WallTopLeft:     return sf::IntRect( 0, 0,32,32);
            case Piece::WallTop:         return sf::IntRect(64, 0,32,32);
            case Piece::WallHoriz:       return sf::IntRect(32, 0,32,32);
            case Piece::WallHorizShadow: return sf::IntRect(32,32,32,32);
            case Piece::WallVert:        return sf::IntRect( 0,32,32,32);
            case Piece::Player:          return sf::IntRect(96,32,32,32);
            case Piece::Stairs:          return sf::IntRect(96,64,32,32);
            case Piece::Barrel:          return sf::IntRect(96, 0,32,32);
            case Piece::Floor1:          return sf::IntRect( 0,64,32,32);
            case Piece::Floor2:          return sf::IntRect(32,64,32,32);
            case Piece::Floor3:          return sf::IntRect(64,64,32,32);
            case Piece::Floor4:          return sf::IntRect( 0,96,32,32);
            case Piece::Floor5:          return sf::IntRect(32,96,32,32);
            case Piece::Floor6:          return sf::IntRect(64,96,32,32);
            case Piece::Empty:                 
            default:                     return sf::IntRect(96,96,32,32);
        }
        // clang-format on
    }

    // inline char pieceToMapChar(const Piece piece)
    //{
    //    // clang-format off
    //    switch (piece)
    //    {
    //        case Piece::WallBlock:   return  'B';
    //        case Piece::WallTopLeft: return  'C';
    //        case Piece::WallTop:     return  'T';
    //        case Piece::WallHoriz:   return  'H';
    //        case Piece::WallVert:    return  'V';
    //        case Piece::Player:      return  'p';
    //        case Piece::Stairs:      return  's';
    //        case Piece::Barrel:      return  'b';
    //        case Piece::Empty:
    //        default:                 return  ' ';
    //    }
    //     // clang-format on
    //}

} // namespace boardgame
#endif // #define BOARDGAME_TYPES_HPP_INCLUDED
