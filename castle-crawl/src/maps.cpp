// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// maps.hpp
//
#include "maps.hpp"

namespace castlecrawl
{

    void Maps::load(const Context & context)
    {
        m_maps.clear();

        // clang-format off
     
        m_maps["level-1-entry-hall"] = Map {
            context,
            true,
            MapChars_t {
                ".........",
                ".       .",
                ".       .",
                ".       .",
                ".       .",
                ".       .",
                ".       .",
                ".       .",
                ".       .",
                ".... ....",
                ".... ....",
                ".... ....",
                ".... ....",
                "....d....",
             },
            MapLinks_t {
                { { 4, 14 }, "level-1-store-room", { 4, 1 } }
                }
            };

        m_maps["level-1-store-room"] = Map {
            context,
            false,
            MapChars_t {
                "....d............................",
                ".... ............................",
                ".... .........     d       ......",
                ".... .........  bbb....... ......",
                ".... .........   bb....... ......",
                ".       ......    b.......d......",
                ".       ......     .......    ...",
                ".       d          .......    ...",
                ".       ..................    ...",
                ".       ..................d......",
                ".       .................. ......",
                ".       .................. ......",
                ".       ......b    b...... ......",
                ".       ......      ...... ......",
                ".       D           ...... ......",
                ".... .........      ......       ",
                ".... .........b    b.............",
                ".... ............................",
                ".... ............................",
                ".... ............................",
                ".... ............................"
                },
            MapLinks_t {
                { { 4, -1 }, "level-1-entry-hall",  { 4, 12 } },
                { { 4, 21 }, "level-1-boiler-room", { 4,  0 } },
                { { 33, 15 }, "level-1-back-room", { 0,  8 } }
                }
            };

        m_maps["level-1-boiler-room"] = Map {
            context,
            true,
            MapChars_t {
                ".... ............................",
                ".... .LLLL  llll.................",
                ".... .LLLL  llll.................",
                ".... .b        bbb|  bb..........",
                ".... .          bb|   b..........",
                ".... .--|        b|    ..........",
                "....    |         |    ..........",
                "....... |------D--|    ..........",
                "....... |              ..........",
                "....... |              ..........",
                ".......d|              ..........",
                ".                      ..........",
                ".              |-------..........",
                ".              | bbWWWWWWW.......",
                ".              D   WWW...........",
                ".              |   WWW...........",
                "......----- ---..................",
                "......   b     ..................",
                "......bb      b..................",
                "......bbbb  bbb..................",
                "................................."
                },
            MapLinks_t {
                { { 4,  -1 }, "level-1-store-room", { 4, 20 } }
                }
            };

        m_maps["level-1-back-room"] = Map {
            context,
            true,
            MapChars_t {
                ".................................",
                "...................      ........",
                "........   ........      ........",
                "........   .......... ...........",
                "....b      .......... ...........",
                "....... ............. ...........",
                "....... ............. ...........",
                ".......D............. ...........",
                "        D             ...........",
                ".......D.........................",
                "....... .........................",
                "....... .........................",
                "....... .........................",
                "....... .........................",
                "..           ....................",
                "..          b....................",
                "..         bb....................",
                "..        bbb....................",
                ".................................",
                ".................................",
                "................................."
                },
            MapLinks_t {
                { { -1, 8 }, "level-1-store-room", { 32, 15 } }
                }
            };

        // clang-format on
    }

} // namespace castlecrawl
