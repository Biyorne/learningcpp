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
        Wood1,
        Wood2,
        Wood3,
        Wood4,
        Wood5,
        Wood6,
        Stone1,
        Stone2,
        Stone3,
        Stone4,
        Stone5,
        Stone6,
        Lava1,
        Lava2,
        Water,
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
            case 'D': return TileImage::Door; // door can be upper or lower case
            case 'd': return TileImage::Door;
            case '0': return TileImage::Wood1;
            case '1': return TileImage::Wood2;
            case '2': return TileImage::Wood3;
            case '3': return TileImage::Wood4;
            case '4': return TileImage::Wood5;
            case '5': return TileImage::Wood6;
            case '6': return TileImage::Stone1;
            case '7': return TileImage::Stone2;
            case '8': return TileImage::Stone3;
            case '9': return TileImage::Stone4;
            case ':': return TileImage::Stone5;
            case ';': return TileImage::Stone6;
            case 'l': return TileImage::Lava1;
            case 'L': return TileImage::Lava2;
            case 'W': return TileImage::Water;
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
            case TileImage::Door:            return sf::IntRect(96,96,32,32);
            //
            case TileImage::Wood1:           return sf::IntRect( 0,64,32,32);
            case TileImage::Wood2:           return sf::IntRect(32,64,32,32);
            case TileImage::Wood3:           return sf::IntRect(64,64,32,32);
            case TileImage::Wood4:           return sf::IntRect( 0,96,32,32);
            case TileImage::Wood5:           return sf::IntRect(32,96,32,32);
            case TileImage::Wood6:           return sf::IntRect(64,96,32,32);
            //
            case TileImage::Stone1:          return sf::IntRect( 0,128,32,32);
            case TileImage::Stone2:          return sf::IntRect(32,128,32,32);
            case TileImage::Stone3:          return sf::IntRect(64,128,32,32);
            case TileImage::Stone4:          return sf::IntRect( 0,160,32,32);
            case TileImage::Stone5:          return sf::IntRect(32,160,32,32);
            case TileImage::Stone6:          return sf::IntRect(64,160,32,32);
            //
            case TileImage::Lava1:           return sf::IntRect( 0,192,32,32);
            case TileImage::Lava2:           return sf::IntRect(32,192,32,32);
            case TileImage::Water:           return sf::IntRect(64,192,32,32);
            //
            case TileImage::Empty:                 
            case TileImage::Count:
            default:                         return sf::IntRect(0,0,0,0);
        }
        // clang-format on
    }

} // namespace castlecrawl
#endif // #define CASTLECRAWL_TILEIMAGE_HPP_INCLUDED
