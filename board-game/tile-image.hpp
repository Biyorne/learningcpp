#ifndef CASTLECRAWL_TILEIMAGE_HPP_INCLUDED
#define CASTLECRAWL_TILEIMAGE_HPP_INCLUDED
//
// tile-image.hpp
//
#include <cstddef>

#include <SFML/Graphics/Rect.hpp>

namespace castlecrawl
{
    // used to index so keep unsigned
    enum class TileImage : std::size_t
    {
        Empty = 0,
        Floor1,
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
        Door,
        Count
    };

    inline TileImage mapCharToTileImage(const char mapChar)
    {
        switch (mapChar)
        {
            case 'B': return TileImage::WallBlock;
            case 'C': return TileImage::WallTopLeft;
            case 'T': return TileImage::WallTop;
            case '-': return TileImage::WallHoriz;
            // skip shadow
            case '|': return TileImage::WallVert;
            case 'p': return TileImage::Player;
            case 's': return TileImage::Stairs;
            case 'b': return TileImage::Barrel;
            case '1': return TileImage::Floor1;
            case '2': return TileImage::Floor2;
            case '3': return TileImage::Floor3;
            case '4': return TileImage::Floor4;
            case '5': return TileImage::Floor5;
            case '6': return TileImage::Floor6;
            case 'D': return TileImage::Door;
            default: return TileImage::Empty;
        }
    }

    inline sf::IntRect tileImageToTileRect(const TileImage image)
    {
        // clang-format off
        switch (image)
        {
            case TileImage::WallBlock:       return sf::IntRect(64,32,32,32);
            case TileImage::WallTopLeft:     return sf::IntRect( 0, 0,32,32);
            case TileImage::WallTop:         return sf::IntRect(64, 0,32,32);
            case TileImage::WallHoriz:       return sf::IntRect(32, 0,32,32);
            case TileImage::WallHorizShadow: return sf::IntRect(32,32,32,32);
            case TileImage::WallVert:        return sf::IntRect( 0,32,32,32);
            case TileImage::Player:          return sf::IntRect(96,32,32,32);
            case TileImage::Stairs:          return sf::IntRect(96,64,32,32);
            case TileImage::Barrel:          return sf::IntRect(96, 0,32,32);
            case TileImage::Floor1:          return sf::IntRect( 0,64,32,32);
            case TileImage::Floor2:          return sf::IntRect(32,64,32,32);
            case TileImage::Floor3:          return sf::IntRect(64,64,32,32);
            case TileImage::Floor4:          return sf::IntRect( 0,96,32,32);
            case TileImage::Floor5:          return sf::IntRect(32,96,32,32);
            case TileImage::Floor6:          return sf::IntRect(64,96,32,32);
            case TileImage::Door:            return sf::IntRect(96,96,32,32);
            case TileImage::Empty:                 
            case TileImage::Count:
            default:                         return sf::IntRect(0,0,0,0);
        }
        // clang-format on
    }

} // namespace castlecrawl
#endif // #define CASTLECRAWL_TILEIMAGE_HPP_INCLUDED
