#ifndef SFUTIL_COLOR_NAMES_HPP_INCLUDED
#define SFUTIL_COLOR_NAMES_HPP_INCLUDED
//
// color-names.hpp
//
#include <array>

#include <SFML/Graphics/Color.hpp>

namespace color
{

    namespace pallete
    {

        // a classic full 16 color pallete
        namespace rgbfull
        {

            // clang-format off
            enum Enum : sf::Uint32
            {
                black   = 0x000000ff,
                red     = 0xff0000ff,
                green   = 0x00ff00ff,
                blue    = 0x0000ffff,
                yellow  = 0xffff00ff,
                magenta = 0xff00ffff,
                cyan    = 0x00ffffff,
                white   = 0xffffffff
            };

            const std::array<sf::Color, 8> Array{
                sf::Color(black),
                sf::Color(red),
                sf::Color(green),
                sf::Color(blue),
                sf::Color(yellow),
                sf::Color(magenta),
                sf::Color(cyan),
                sf::Color(white)
            };

            constexpr std::size_t Count{ Array.size() };
            // clang-format on

        } // namespace rgbfull

        // a classic full 16 color pallete
        namespace cga
        {

            // clang-format off
            enum Enum : sf::Uint32
            {
                black        = 0x000000ff,
                blue         = 0x0000aaff,
                green        = 0x00aa00ff,
                cyan         = 0x00aaaaff,
                red          = 0xaa0000ff,
                magenta      = 0xaa00aaff,
                brown        = 0xaa5500ff,
                lightgrey    = 0xaaaaaaff,
                darkgrey     = 0x555555ff,
                lightblue    = 0x5555ffff,
                lightgreen   = 0x55ff55ff,
                lightcyan    = 0x55ffffff,
                lightred     = 0xff5555ff,
                lightmagenta = 0xff55ffff,
                yellow       = 0xffff55ff,
                white        = 0xffffffff
            };


            const std::array<sf::Color, 16> Array{
                sf::Color(black),
                sf::Color(blue),
                sf::Color(green),
                sf::Color(cyan),
                sf::Color(red),
                sf::Color(magenta),
                sf::Color(brown),
                sf::Color(lightgrey),
                sf::Color(darkgrey),
                sf::Color(lightblue),
                sf::Color(lightgreen),
                sf::Color(lightcyan),
                sf::Color(lightred),
                sf::Color(lightmagenta),
                sf::Color(yellow),
                sf::Color(white)
            };

            constexpr std::size_t Count{ Array.size() };
            // clang-format on

        } // namespace cga

        // the classic 8-bit home computer released in 1982
        namespace commodore64
        {
            // clang-format off
            enum Enum : sf::Uint32
            {
                black 	   = 0x000000ff,
                white 	   = 0xffffffff,
                red 	   = 0x880000ff,
                cyan 	   = 0xaaffeeff,
                purple 	   = 0xcc44ccff,
                green 	   = 0x00cc55ff,
                blue 	   = 0x0000aaff,
                yellow 	   = 0xeeee77ff,
                orange 	   = 0xdd8855ff,
                brown 	   = 0x664400ff,
                lightred   = 0xff7777ff,
                darkgrey   = 0x333333ff,
                grey       = 0x777777ff,
                lightgreen = 0xaaff66ff,
                lightblue  = 0x0088ffff,
                lightgrey  = 0xbbbbbbff
            };

            const std::array<sf::Color, 16> Array{
                sf::Color(black),
                sf::Color(white),
                sf::Color(red),
                sf::Color(cyan),
                sf::Color(purple),
                sf::Color(green),
                sf::Color(blue),
                sf::Color(yellow),
                sf::Color(orange),
                sf::Color(brown),
                sf::Color(lightred),
                sf::Color(darkgrey),
                sf::Color(grey),
                sf::Color(lightgreen),
                sf::Color(lightblue),
                sf::Color(lightgrey)
            };

            constexpr std::size_t Count{ Array.size() };
            // clang-format on

        } // namespace commodore64

        // from the old windows 3.1 days
        namespace oldwindows
        {

            // clang-format off
            enum Enum : sf::Uint32
            {
                black   = 0x000000ff,
                maroon  = 0x7e0000ff,
                green   = 0x047e00ff,
                olive   = 0x7e7e00ff,
                navy    = 0x00007eff,
                purple  = 0x7e007eff,
                teal    = 0x047e7eff,
                silver  = 0xbebebeff,
                //
                grey    = 0x7e7e7eff,
                red     = 0xfe0000ff,
                lime    = 0x06ff04ff,
                yellow  = 0xffff04ff,
                blue    = 0x0000ffff,
                fuchsia = 0xfe00ffff,
                aqua    = 0x06ffffff,
                white   = 0xffffffff
            };

            const std::array<sf::Color, 16> Array{
                sf::Color(black),
                sf::Color(maroon),
                sf::Color(green),
                sf::Color(olive),
                sf::Color(navy),
                sf::Color(purple),
                sf::Color(teal),
                sf::Color(silver),
                sf::Color(grey),
                sf::Color(red),
                sf::Color(lime),
                sf::Color(yellow),
                sf::Color(blue),
                sf::Color(fuchsia),
                sf::Color(aqua),
                sf::Color(white)
            };

            constexpr std::size_t Count{ Array.size() };
            // clang-format on

        } // namespace oldwindows

        // from w3c.com (HTML v5)
        namespace websafe
        {

            // clang-format off
            enum Enum : sf::Uint32
            {
                black                = 0x000000ff,
                navy                 = 0x000080ff,
                darkblue             = 0x00008bff,
                mediumblue           = 0x0000cdff,
                blue                 = 0x0000ffff,
                darkgreen            = 0x006400ff,
                green                = 0x008000ff,
                teal                 = 0x008080ff,
                darkcyan             = 0x008b8bff,
                deepskyblue          = 0x00bfffff,
                darkturquoise        = 0x00ced1ff,
                mediumspringgreen    = 0x00fa9aff,
                lime                 = 0x00ff00ff,
                springgreen          = 0x00ff7fff,
                cyan                 = 0x00ffffff,
                midnightblue         = 0x191970ff,
                dodgerblue           = 0x1e90ffff,
                lightseagreen        = 0x20b2aaff,
                forestgreen          = 0x228b22ff,
                seagreen             = 0x2e8b57ff,
                darkslategrey        = 0x2f4f4fff,
                limegreen            = 0x32cd32ff,
                mediumseagreen       = 0x3cb371ff,
                turquoise            = 0x40e0d0ff,
                royalblue            = 0x4169e1ff,
                steelblue            = 0x4682b4ff,
                darkslateblue        = 0x483d8bff,
                mediumturquoise      = 0x48d1ccff,
                indigo               = 0x4b0082ff,
                darkolivegreen       = 0x556b2fff,
                cadetblue            = 0x5f9ea0ff,
                cornflowerblue       = 0x6495edff,
                rebeccapurple        = 0x663399ff,
                mediumaquamarine     = 0x66cdaaff,
                dimgrey              = 0x696969ff,
                slateblue            = 0x6a5acdff,
                olivedrab            = 0x6b8e23ff,
                slategrey            = 0x708090ff,
                lightslategrey       = 0x778899ff,
                mediumslateblue      = 0x7b68eeff,
                lawngreen            = 0x7cfc00ff,
                chartreuse           = 0x7fff00ff,
                aquamarine           = 0x7fffd4ff,
                maroon               = 0x800000ff,
                purple               = 0x800080ff,
                olive                = 0x808000ff,
                grey                 = 0x808080ff,
                skyblue              = 0x87ceebff,
                lightskyblue         = 0x87cefaff,
                blueviolet           = 0x8a2be2ff,
                darkred              = 0x8b0000ff,
                darkmagenta          = 0x8b008bff,
                saddlebrown          = 0x8b4513ff,
                darkseagreen         = 0x8fbc8fff,
                lightgreen           = 0x90ee90ff,
                mediumpurple         = 0x9370dbff,
                darkviolet           = 0x9400d3ff,
                palegreen            = 0x98fb98ff,
                darkorchid           = 0x9932ccff,
                yellowgreen          = 0x9acd32ff,
                sienna               = 0xa0522dff,
                brown                = 0xa52a2aff,
                darkgrey             = 0xa9a9a9ff,
                lightblue            = 0xadd8e6ff,
                greenyellow          = 0xadff2fff,
                paleturquoise        = 0xafeeeeff,
                lightsteelblue       = 0xb0c4deff,
                powderblue           = 0xb0e0e6ff,
                firebrick            = 0xb22222ff,
                darkgoldenrod        = 0xb8860bff,
                mediumorchid         = 0xba55d3ff,
                rosybrown            = 0xbc8f8fff,
                darkkhaki            = 0xbdb76bff,
                silver               = 0xc0c0c0ff,
                mediumvioletred      = 0xc71585ff,
                indianred            = 0xcd5c5cff,
                peru                 = 0xcd853fff,
                chocolate            = 0xd2691eff,
                tan                  = 0xd2b48cff,
                lightgrey            = 0xd3d3d3ff,
                thistle              = 0xd8bfd8ff,
                orchid               = 0xda70d6ff,
                goldenrod            = 0xdaa520ff,
                palevioletred        = 0xdb7093ff,
                crimson              = 0xdc143cff,
                gainsboro            = 0xdcdcdcff,
                plum                 = 0xdda0ddff,
                burlywood            = 0xdeb887ff,
                lightcyan            = 0xe0ffffff,
                lavender             = 0xe6e6faff,
                darksalmon           = 0xe9967aff,
                violet               = 0xee82eeff,
                palegoldenrod        = 0xeee8aaff,
                lightcoral           = 0xf08080ff,
                khaki                = 0xf0e68cff,
                aliceblue            = 0xf0f8ffff,
                honeydew             = 0xf0fff0ff,
                azure                = 0xf0ffffff,
                sandybrown           = 0xf4a460ff,
                wheat                = 0xf5deb3ff,
                beige                = 0xf5f5dcff,
                whitesmoke           = 0xf5f5f5ff,
                mintcream            = 0xf5fffaff,
                ghostwhite           = 0xf8f8ffff,
                salmon               = 0xfa8072ff,
                antiquewhite         = 0xfaebd7ff,
                linen                = 0xfaf0e6ff,
                lightgoldenrodyellow = 0xfafad2ff,
                oldlace              = 0xfdf5e6ff,
                red                  = 0xff0000ff,
                magenta              = 0xff00ffff,
                deeppink             = 0xff1493ff,
                orangered            = 0xff4500ff,
                tomato               = 0xff6347ff,
                hotpink              = 0xff69b4ff,
                coral                = 0xff7f50ff,
                darkorange           = 0xff8c00ff,
                lightsalmon          = 0xffa07aff,
                orange               = 0xffa500ff,
                lightpink            = 0xffb6c1ff,
                pink                 = 0xffc0cbff,
                gold                 = 0xffd700ff,
                peachpuff            = 0xffdab9ff,
                navajowhite          = 0xffdeadff,
                moccasin             = 0xffe4b5ff,
                bisque               = 0xffe4c4ff,
                mistyrose            = 0xffe4e1ff,
                blanchedalmond       = 0xffebcdff,
                papayawhip           = 0xffefd5ff,
                lavenderblush        = 0xfff0f5ff,
                seashell             = 0xfff5eeff,
                cornsilk             = 0xfff8dcff,
                lemonchiffon         = 0xfffacdff,
                floralwhite          = 0xfffaf0ff,
                snow                 = 0xfffafaff,
                yellow               = 0xffff00ff,
                lightyellow          = 0xffffe0ff,
                ivory                = 0xfffff0ff,
                white                = 0xffffffff,
            };

            const std::array<sf::Color, 139> Array{
                sf::Color(black),
                sf::Color(navy),
                sf::Color(darkblue),
                sf::Color(mediumblue),
                sf::Color(blue),
                sf::Color(darkgreen),
                sf::Color(green),
                sf::Color(teal),
                sf::Color(darkcyan),
                sf::Color(deepskyblue),
                sf::Color(darkturquoise),
                sf::Color(mediumspringgreen),
                sf::Color(lime),
                sf::Color(springgreen),
                sf::Color(cyan),
                sf::Color(midnightblue),
                sf::Color(dodgerblue),
                sf::Color(lightseagreen),
                sf::Color(forestgreen),
                sf::Color(seagreen),
                sf::Color(darkslategrey),
                sf::Color(limegreen),
                sf::Color(mediumseagreen),
                sf::Color(turquoise),
                sf::Color(royalblue),
                sf::Color(steelblue),
                sf::Color(darkslateblue),
                sf::Color(mediumturquoise),
                sf::Color(indigo),
                sf::Color(darkolivegreen),
                sf::Color(cadetblue),
                sf::Color(cornflowerblue),
                sf::Color(rebeccapurple),
                sf::Color(mediumaquamarine),
                sf::Color(dimgrey),
                sf::Color(slateblue),
                sf::Color(olivedrab),
                sf::Color(slategrey),
                sf::Color(lightslategrey),
                sf::Color(mediumslateblue),
                sf::Color(lawngreen),
                sf::Color(chartreuse),
                sf::Color(aquamarine),
                sf::Color(maroon),
                sf::Color(purple),
                sf::Color(olive),
                sf::Color(grey),
                sf::Color(skyblue),
                sf::Color(lightskyblue),
                sf::Color(blueviolet),
                sf::Color(darkred),
                sf::Color(darkmagenta),
                sf::Color(saddlebrown),
                sf::Color(darkseagreen),
                sf::Color(lightgreen),
                sf::Color(mediumpurple),
                sf::Color(darkviolet),
                sf::Color(palegreen),
                sf::Color(darkorchid),
                sf::Color(yellowgreen),
                sf::Color(sienna),
                sf::Color(brown),
                sf::Color(darkgrey),
                sf::Color(lightblue),
                sf::Color(greenyellow),
                sf::Color(paleturquoise),
                sf::Color(lightsteelblue),
                sf::Color(powderblue),
                sf::Color(firebrick),
                sf::Color(darkgoldenrod),
                sf::Color(mediumorchid),
                sf::Color(rosybrown),
                sf::Color(darkkhaki),
                sf::Color(silver),
                sf::Color(mediumvioletred),
                sf::Color(indianred),
                sf::Color(peru),
                sf::Color(chocolate),
                sf::Color(tan),
                sf::Color(lightgrey),
                sf::Color(thistle),
                sf::Color(orchid),
                sf::Color(goldenrod),
                sf::Color(palevioletred),
                sf::Color(crimson),
                sf::Color(gainsboro),
                sf::Color(plum),
                sf::Color(burlywood),
                sf::Color(lightcyan),
                sf::Color(lavender),
                sf::Color(darksalmon),
                sf::Color(violet),
                sf::Color(palegoldenrod),
                sf::Color(lightcoral),
                sf::Color(khaki),
                sf::Color(aliceblue),
                sf::Color(honeydew),
                sf::Color(azure),
                sf::Color(sandybrown),
                sf::Color(wheat),
                sf::Color(beige),
                sf::Color(whitesmoke),
                sf::Color(mintcream),
                sf::Color(ghostwhite),
                sf::Color(salmon),
                sf::Color(antiquewhite),
                sf::Color(linen),
                sf::Color(lightgoldenrodyellow),
                sf::Color(oldlace),
                sf::Color(red),
                sf::Color(magenta),
                sf::Color(deeppink),
                sf::Color(orangered),
                sf::Color(tomato),
                sf::Color(hotpink),
                sf::Color(coral),
                sf::Color(darkorange),
                sf::Color(lightsalmon),
                sf::Color(orange),
                sf::Color(lightpink),
                sf::Color(pink),
                sf::Color(gold),
                sf::Color(peachpuff),
                sf::Color(navajowhite),
                sf::Color(moccasin),
                sf::Color(bisque),
                sf::Color(mistyrose),
                sf::Color(blanchedalmond),
                sf::Color(papayawhip),
                sf::Color(lavenderblush),
                sf::Color(seashell),
                sf::Color(cornsilk),
                sf::Color(lemonchiffon),
                sf::Color(floralwhite),
                sf::Color(snow),
                sf::Color(yellow),
                sf::Color(lightyellow),
                sf::Color(ivory),
                sf::Color(white)
            };

            constexpr std::size_t Count{ Array.size() };
            // clang-format on

        } // namespace websafe

        // www.xkcd.com/color/rgb.txt (CC0)
        namespace xkcd
        {

            // clang-format off
            enum Enum : sf::Uint32
            {
                black                      = 0x000000ff, // (0, 0, 0)
                verydarkblue               = 0x000133ff, // (0, 1, 51)
                darknavyblue               = 0x00022eff, // (0, 2, 46)
                darkblue1                  = 0x00035bff, // (0, 3, 91)
                darknavy                   = 0x000435ff, // (0, 4, 53)
                navyblue                   = 0x001146ff, // (0, 17, 70)
                darkforestgreen            = 0x002d04ff, // (0, 45, 4)
                prussianblue               = 0x004577ff, // (0, 69, 119)
                darkbluegreen              = 0x005249ff, // (0, 82, 73)
                deepteal                   = 0x00555aff, // (0, 85, 90)
                petrol                     = 0x005f6aff, // (0, 95, 106)
                kelleygreen                = 0x009337ff, // (0, 147, 55)
                greenishturquoise          = 0x00fbb0ff, // (0, 251, 176)
                cyan                       = 0x00ffffff, // (0, 255, 255)
                trueblue                   = 0x010fccff, // (1, 15, 204)
                navy                       = 0x01153eff, // (1, 21, 62)
                marineblue                 = 0x01386aff, // (1, 56, 106)
                darkishblue                = 0x014182ff, // (1, 65, 130)
                racinggreen                = 0x014600ff, // (1, 70, 0)
                darkteal                   = 0x014d4eff, // (1, 77, 78)
                deepseablue                = 0x015482ff, // (1, 84, 130)
                brightblue                 = 0x0165fcff, // (1, 101, 252)
                peacockblue                = 0x016795ff, // (1, 103, 149)
                darkaquamarine             = 0x017371ff, // (1, 115, 113)
                deepturquoise              = 0x017374ff, // (1, 115, 116)
                bluegreen1                 = 0x017a79ff, // (1, 122, 121)
                ocean                      = 0x017b92ff, // (1, 123, 146)
                tealblue                   = 0x01889fff, // (1, 136, 159)
                irishgreen                 = 0x019529ff, // (1, 149, 41)
                emerald                    = 0x01a049ff, // (1, 160, 73)
                shamrock                   = 0x01b44cff, // (1, 180, 76)
                greenblue1                 = 0x01c08dff, // (1, 192, 141)
                brightteal                 = 0x01f9c6ff, // (1, 249, 198)
                brightgreen                = 0x01ff07ff, // (1, 255, 7)
                midnightblue               = 0x020035ff, // (2, 0, 53)
                pureblue                   = 0x0203e2ff, // (2, 3, 226)
                darkroyalblue              = 0x02066fff, // (2, 6, 111)
                richblue                   = 0x021bf9ff, // (2, 27, 249)
                deepgreen                  = 0x02590fff, // (2, 89, 15)
                emeraldgreen               = 0x028f1eff, // (2, 143, 30)
                teal                       = 0x029386ff, // (2, 147, 134)
                kellygreen                 = 0x02ab2eff, // (2, 171, 46)
                shamrockgreen              = 0x02c14dff, // (2, 193, 77)
                brightskyblue              = 0x02ccfeff, // (2, 204, 254)
                aquablue                   = 0x02d8e9ff, // (2, 216, 233)
                midnight                   = 0x03012dff, // (3, 1, 45)
                darkblue2                  = 0x030764ff, // (3, 7, 100)
                cobaltblue                 = 0x030aa7ff, // (3, 10, 167)
                blue                       = 0x0343dfff, // (3, 67, 223)
                darkgreen1                 = 0x033500ff, // (3, 53, 0)
                vibrantblue                = 0x0339f8ff, // (3, 57, 248)
                oceanblue                  = 0x03719cff, // (3, 113, 156)
                deepblue                   = 0x040273ff, // (4, 2, 115)
                nightblue                  = 0x040348ff, // (4, 3, 72)
                marine                     = 0x042e60ff, // (4, 46, 96)
                bottlegreen                = 0x044a05ff, // (4, 74, 5)
                darkturquoise              = 0x045c5aff, // (4, 92, 90)
                seablue                    = 0x047495ff, // (4, 116, 149)
                junglegreen                = 0x048243ff, // (4, 130, 67)
                cerulean                   = 0x0485d1ff, // (4, 133, 209)
                aquamarine1                = 0x04d8b2ff, // (4, 216, 178)
                neonblue                   = 0x04d9ffff, // (4, 217, 255)
                turquoisegreen             = 0x04f489ff, // (4, 244, 137)
                royalblue                  = 0x0504aaff, // (5, 4, 170)
                evergreen                  = 0x05472aff, // (5, 71, 42)
                britishracinggreen         = 0x05480dff, // (5, 72, 13)
                darkgreen2                 = 0x054907ff, // (5, 73, 7)
                darkaqua                   = 0x05696bff, // (5, 105, 107)
                ceruleanblue               = 0x056eeeff, // (5, 110, 238)
                brightseagreen             = 0x05ffa6ff, // (5, 255, 166)
                verydarkgreen              = 0x062e03ff, // (6, 46, 3)
                forestgreen                = 0x06470cff, // (6, 71, 12)
                electricblue               = 0x0652ffff, // (6, 82, 255)
                azure                      = 0x069af3ff, // (6, 154, 243)
                turquoiseblue              = 0x06b1c4ff, // (6, 177, 196)
                greenblue2                 = 0x06b48bff, // (6, 180, 139)
                turquoise                  = 0x06c2acff, // (6, 194, 172)
                almostblack                = 0x070d0dff, // (7, 13, 13)
                primaryblue                = 0x0804f9ff, // (8, 4, 249)
                deepaqua                   = 0x08787fff, // (8, 120, 127)
                truegreen                  = 0x089404ff, // (8, 148, 4)
                fluorescentgreen           = 0x08ff08ff, // (8, 255, 8)
                twilightblue               = 0x0a437aff, // (10, 67, 122)
                pinegreen                  = 0x0a481eff, // (10, 72, 30)
                spruce                     = 0x0a5f38ff, // (10, 95, 56)
                darkcyan                   = 0x0a888aff, // (10, 136, 138)
                vibrantgreen               = 0x0add08ff, // (10, 221, 8)
                flurogreen                 = 0x0aff02ff, // (10, 255, 2)
                huntergreen                = 0x0b4008ff, // (11, 64, 8)
                forest                     = 0x0b5509ff, // (11, 85, 9)
                greenishblue               = 0x0b8b87ff, // (11, 139, 135)
                mintygreen                 = 0x0bf77dff, // (11, 247, 125)
                brightaqua                 = 0x0bf9eaff, // (11, 249, 234)
                strongblue                 = 0x0c06f7ff, // (12, 6, 247)
                royal                      = 0x0c1793ff, // (12, 23, 147)
                greenteal                  = 0x0cb577ff, // (12, 181, 119)
                tealishgreen               = 0x0cdc73ff, // (12, 220, 115)
                neongreen                  = 0x0cff0cff, // (12, 255, 12)
                deepskyblue                = 0x0d75f8ff, // (13, 117, 248)
                waterblue                  = 0x0e87ccff, // (14, 135, 204)
                bluegreen2                 = 0x0f9b8eff, // (15, 155, 142)
                brightturquoise            = 0x0ffef9ff, // (15, 254, 249)
                niceblue                   = 0x107ab0ff, // (16, 122, 176)
                bluishgreen                = 0x10a674ff, // (16, 166, 116)
                darkseagreen               = 0x11875dff, // (17, 135, 93)
                aquagreen                  = 0x12e193ff, // (18, 225, 147)
                bluegreen3                 = 0x137e6dff, // (19, 126, 109)
                topaz                      = 0x13bbafff, // (19, 187, 175)
                aqua                       = 0x13eac9ff, // (19, 234, 201)
                vividblue                  = 0x152effff, // (21, 46, 255)
                forrestgreen               = 0x154406ff, // (21, 68, 6)
                lightnavy                  = 0x155084ff, // (21, 80, 132)
                green                      = 0x15b01aff, // (21, 176, 26)
                ultramarineblue            = 0x1805dbff, // (24, 5, 219)
                seaweed                    = 0x18d17bff, // (24, 209, 123)
                dark                       = 0x1b2431ff, // (27, 36, 49)
                highlightergreen           = 0x1bfc06ff, // (27, 252, 6)
                verydarkbrown              = 0x1d0200ff, // (29, 2, 0)
                azul                       = 0x1d5decff, // (29, 93, 236)
                cobalt                     = 0x1e488fff, // (30, 72, 143)
                viridian                   = 0x1e9167ff, // (30, 145, 103)
                spearmint                  = 0x1ef876ff, // (30, 248, 118)
                darkindigo                 = 0x1f0954ff, // (31, 9, 84)
                darkbluegrey               = 0x1f3b4dff, // (31, 59, 77)
                darkgreenblue              = 0x1f6357ff, // (31, 99, 87)
                jade                       = 0x1fa774ff, // (31, 167, 116)
                darkseafoam                = 0x1fb57aff, // (31, 181, 122)
                ultramarine                = 0x2000b1ff, // (32, 0, 177)
                darkmintgreen              = 0x20c073ff, // (32, 192, 115)
                wintergreen                = 0x20f986ff, // (32, 249, 134)
                sapphire                   = 0x2138abff, // (33, 56, 171)
                darkslateblue              = 0x214761ff, // (33, 71, 97)
                algaegreen                 = 0x21c36fff, // (33, 195, 111)
                electricgreen              = 0x21fc0dff, // (33, 252, 13)
                blueblue                   = 0x2242c7ff, // (34, 66, 199)
                greenblue3                 = 0x23c48bff, // (35, 196, 139)
                clearblue                  = 0x247afdff, // (36, 122, 253)
                tealish                    = 0x24bca8ff, // (36, 188, 168)
                tealgreen                  = 0x25a36fff, // (37, 163, 111)
                hotgreen                   = 0x25ff29ff, // (37, 255, 41)
                duskblue                   = 0x26538dff, // (38, 83, 141)
                brightlightblue            = 0x26f7fdff, // (38, 247, 253)
                midblue                    = 0x276ab3ff, // (39, 106, 179)
                midnightpurple             = 0x280137ff, // (40, 1, 55)
                darkishgreen               = 0x287c37ff, // (40, 124, 55)
                darkgreyblue               = 0x29465bff, // (41, 70, 91)
                bluish                     = 0x2976bbff, // (41, 118, 187)
                verydarkpurple             = 0x2a0134ff, // (42, 1, 52)
                treegreen                  = 0x2a7e19ff, // (42, 126, 25)
                greenishcyan               = 0x2afeb7ff, // (42, 254, 183)
                pine                       = 0x2b5d34ff, // (43, 93, 52)
                jadegreen                  = 0x2baf6aff, // (43, 175, 106)
                blueygreen                 = 0x2bb179ff, // (43, 177, 121)
                mediumblue                 = 0x2c6fbbff, // (44, 111, 187)
                radioactivegreen           = 0x2cfa1fff, // (44, 250, 31)
                brightlightgreen           = 0x2dfe54ff, // (45, 254, 84)
                lightnavyblue              = 0x2e5a88ff, // (46, 90, 136)
                aquamarine2                = 0x2ee8bbff, // (46, 232, 187)
                vividgreen                 = 0x2fef10ff, // (47, 239, 16)
                uglyblue                   = 0x31668aff, // (49, 102, 138)
                greenishteal               = 0x32bf84ff, // (50, 191, 132)
                coolgreen                  = 0x33b864ff, // (51, 184, 100)
                darkviolet                 = 0x34013fff, // (52, 1, 63)
                darkbrown                  = 0x341c02ff, // (52, 28, 2)
                charcoal                   = 0x343837ff, // (52, 56, 55)
                darkpurple                 = 0x35063eff, // (53, 6, 62)
                navygreen                  = 0x35530aff, // (53, 83, 10)
                seaweedgreen               = 0x35ad6bff, // (53, 173, 107)
                deeppurple                 = 0x36013fff, // (54, 1, 63)
                darkgrey                   = 0x363737ff, // (54, 55, 55)
                darkolive                  = 0x373e02ff, // (55, 62, 2)
                windowsblue                = 0x3778bfff, // (55, 120, 191)
                indigo                     = 0x380282ff, // (56, 2, 130)
                eggplant                   = 0x380835ff, // (56, 8, 53)
                darkgrassgreen             = 0x388004ff, // (56, 128, 4)
                mediumgreen                = 0x39ad48ff, // (57, 173, 72)
                indigoblue                 = 0x3a18b1ff, // (58, 24, 177)
                lightroyalblue             = 0x3a2efeff, // (58, 46, 254)
                weirdgreen                 = 0x3ae57fff, // (58, 229, 127)
                denimblue                  = 0x3b5b92ff, // (59, 91, 146)
                denim                      = 0x3b638cff, // (59, 99, 140)
                mutedblue                  = 0x3b719fff, // (59, 113, 159)
                darkmaroon                 = 0x3c0008ff, // (60, 0, 8)
                charcoalgrey               = 0x3c4142ff, // (60, 65, 66)
                darkolivegreen             = 0x3c4d03ff, // (60, 77, 3)
                flatblue                   = 0x3c73a8ff, // (60, 115, 168)
                sea                        = 0x3c9992ff, // (60, 153, 146)
                aubergine                  = 0x3d0734ff, // (61, 7, 52)
                chocolate                  = 0x3d1c02ff, // (61, 28, 2)
                lightishblue               = 0x3d7afdff, // (61, 122, 253)
                oceangreen                 = 0x3d9973ff, // (61, 153, 115)
                dodgerblue                 = 0x3e82fcff, // (62, 130, 252)
                darkseafoamgreen           = 0x3eaf76ff, // (62, 175, 118)
                darkplum                   = 0x3f012cff, // (63, 1, 44)
                dirtyblue                  = 0x3f829dff, // (63, 130, 157)
                grassgreen                 = 0x3f9b0bff, // (63, 155, 11)
                greenish                   = 0x40a368ff, // (64, 163, 104)
                poisongreen                = 0x40fd14ff, // (64, 253, 20)
                deepbrown                  = 0x410200ff, // (65, 2, 0)
                chocolatebrown             = 0x411900ff, // (65, 25, 0)
                grassygreen                = 0x419c03ff, // (65, 156, 3)
                brightcyan                 = 0x41fdfeff, // (65, 253, 254)
                greenyblue                 = 0x42b395ff, // (66, 179, 149)
                eggplantpurple             = 0x430541ff, // (67, 5, 65)
                frenchblue                 = 0x436badff, // (67, 107, 173)
                darkskyblue                = 0x448ee4ff, // (68, 142, 228)
                blueberry                  = 0x464196ff, // (70, 65, 150)
                duskyblue                  = 0x475f94ff, // (71, 95, 148)
                darkmint                   = 0x48c072ff, // (72, 192, 114)
                deepviolet                 = 0x490648ff, // (73, 6, 72)
                dullblue                   = 0x49759cff, // (73, 117, 156)
                coolblue                   = 0x4984b8ff, // (73, 132, 184)
                mahogany                   = 0x4a0100ff, // (74, 1, 0)
                royalpurple                = 0x4b006eff, // (75, 0, 110)
                driedblood                 = 0x4b0101ff, // (75, 1, 1)
                warmblue                   = 0x4b57dbff, // (75, 87, 219)
                armygreen                  = 0x4b5d16ff, // (75, 93, 22)
                camouflagegreen            = 0x4b6113ff, // (75, 97, 19)
                dustyteal                  = 0x4c9085ff, // (76, 144, 133)
                lawngreen                  = 0x4da409ff, // (77, 164, 9)
                plumpurple                 = 0x4e0550ff, // (78, 5, 80)
                twilight                   = 0x4e518bff, // (78, 81, 139)
                dusk                       = 0x4e5481ff, // (78, 84, 129)
                cadetblue                  = 0x4e7496ff, // (78, 116, 150)
                lightneongreen             = 0x4efd54ff, // (78, 253, 84)
                metallicblue               = 0x4f738eff, // (79, 115, 142)
                lightforestgreen           = 0x4f9153ff, // (79, 145, 83)
                stormyblue                 = 0x507b9cff, // (80, 123, 156)
                midgreen                   = 0x50a747ff, // (80, 167, 71)
                violetblue                 = 0x510ac9ff, // (81, 10, 201)
                slate                      = 0x516572ff, // (81, 101, 114)
                cornflowerblue             = 0x5170d7ff, // (81, 112, 215)
                leafygreen                 = 0x51b73bff, // (81, 183, 59)
                camogreen                  = 0x526525ff, // (82, 101, 37)
                bluewithahintofpurple      = 0x533cc6ff, // (83, 60, 198)
                gunmetal                   = 0x536267ff, // (83, 98, 103)
                seagreen                   = 0x53fca1ff, // (83, 252, 161)
                lightbrightgreen           = 0x53fe5cff, // (83, 254, 92)
                greenbrown                 = 0x544e03ff, // (84, 78, 3)
                ferngreen                  = 0x548d44ff, // (84, 141, 68)
                algae                      = 0x54ac68ff, // (84, 172, 104)
                blurple                    = 0x5539ccff, // (85, 57, 204)
                offblue                    = 0x5684aeff, // (86, 132, 174)
                darkpastelgreen            = 0x56ae57ff, // (86, 174, 87)
                lightgreenblue             = 0x56fca2ff, // (86, 252, 162)
                bluepurple1                = 0x5729ceff, // (87, 41, 206)
                plum                       = 0x580f41ff, // (88, 15, 65)
                froggreen                  = 0x58bc08ff, // (88, 188, 8)
                slategrey                  = 0x59656dff, // (89, 101, 109)
                darksage                   = 0x598556ff, // (89, 133, 86)
                bluepurple2                = 0x5a06efff, // (90, 6, 239)
                steelblue                  = 0x5a7d9aff, // (90, 125, 154)
                dustyblue                  = 0x5a86adff, // (90, 134, 173)
                slateblue                  = 0x5b7c99ff, // (91, 124, 153)
                sapgreen                   = 0x5c8b15ff, // (92, 139, 21)
                leafgreen                  = 0x5ca904ff, // (92, 169, 4)
                grass                      = 0x5cac2dff, // (92, 172, 45)
                kermitgreen                = 0x5cb200ff, // (92, 178, 0)
                blueviolet                 = 0x5d06e9ff, // (93, 6, 233)
                grapepurple                = 0x5d1451ff, // (93, 20, 81)
                purpleblue1                = 0x5d21d0ff, // (93, 33, 208)
                greyishblue                = 0x5e819dff, // (94, 129, 157)
                greyteal                   = 0x5e9b8aff, // (94, 155, 138)
                greenapple                 = 0x5edc1fff, // (94, 220, 31)
                purpleyblue                = 0x5f34e7ff, // (95, 52, 231)
                dullteal                   = 0x5f9e8fff, // (95, 158, 143)
                mutedgreen                 = 0x5fa052ff, // (95, 160, 82)
                purplishblue               = 0x601ef9ff, // (96, 30, 249)
                mudbrown                   = 0x60460fff, // (96, 70, 15)
                mudgreen                   = 0x606602ff, // (96, 102, 2)
                bluegrey4                  = 0x607c8eff, // (96, 124, 142)
                burgundy                   = 0x610023ff, // (97, 0, 35)
                purpleishblue              = 0x6140efff, // (97, 64, 239)
                toxicgreen                 = 0x61de2aff, // (97, 222, 42)
                lightishgreen              = 0x61e160ff, // (97, 225, 96)
                blueypurple                = 0x6241c7ff, // (98, 65, 199)
                iris                       = 0x6258c4ff, // (98, 88, 196)
                purpleblue2                = 0x632de9ff, // (99, 45, 233)
                mossygreen                 = 0x638b27ff, // (99, 139, 39)
                fern                       = 0x63a950ff, // (99, 169, 80)
                boringgreen                = 0x63b365ff, // (99, 179, 101)
                lightgreenishblue          = 0x63f7b4ff, // (99, 247, 180)
                olivebrown                 = 0x645403ff, // (100, 84, 3)
                greyblue1                  = 0x647d8eff, // (100, 125, 142)
                softblue                   = 0x6488eaff, // (100, 136, 234)
                maroon                     = 0x650021ff, // (101, 0, 33)
                brown                      = 0x653700ff, // (101, 55, 0)
                muddygreen                 = 0x657432ff, // (101, 116, 50)
                mossgreen                  = 0x658b38ff, // (101, 139, 56)
                fadedblue                  = 0x658cbbff, // (101, 140, 187)
                slategreen                 = 0x658d6dff, // (101, 141, 109)
                tea                        = 0x65ab7cff, // (101, 171, 124)
                brightlimegreen            = 0x65fe08ff, // (101, 254, 8)
                purplyblue                 = 0x661aeeff, // (102, 26, 238)
                darkperiwinkle             = 0x665fd1ff, // (102, 95, 209)
                militarygreen              = 0x667c3eff, // (102, 124, 62)
                dirtygreen                 = 0x667e2cff, // (102, 126, 44)
                purplebrown                = 0x673a3fff, // (103, 58, 63)
                olivegreen                 = 0x677a04ff, // (103, 122, 4)
                claret                     = 0x680018ff, // (104, 0, 24)
                burple                     = 0x6832e3ff, // (104, 50, 227)
                greenybrown                = 0x696006ff, // (105, 96, 6)
                greenishbrown              = 0x696112ff, // (105, 97, 18)
                swamp                      = 0x698339ff, // (105, 131, 57)
                flatgreen                  = 0x699d4cff, // (105, 157, 76)
                freshgreen                 = 0x69d84fff, // (105, 216, 79)
                brownishgreen              = 0x6a6e09ff, // (106, 110, 9)
                cornflower                 = 0x6a79f7ff, // (106, 121, 247)
                purplishbrown              = 0x6b4247ff, // (107, 66, 71)
                battleshipgrey             = 0x6b7c85ff, // (107, 124, 133)
                greyblue2                  = 0x6b8ba4ff, // (107, 139, 164)
                offgreen                   = 0x6ba353ff, // (107, 163, 83)
                grape                      = 0x6c3461ff, // (108, 52, 97)
                murkygreen                 = 0x6c7a0eff, // (108, 122, 14)
                lightindigo                = 0x6d5acfff, // (109, 90, 207)
                robinsegg                  = 0x6dedfdff, // (109, 237, 253)
                reddybrown                 = 0x6e1005ff, // (110, 16, 5)
                olive                      = 0x6e750eff, // (110, 117, 14)
                apple                      = 0x6ecb3cff, // (110, 203, 60)
                brownygreen                = 0x6f6c0aff, // (111, 108, 10)
                olivedrab                  = 0x6f7632ff, // (111, 118, 50)
                poopgreen                  = 0x6f7c00ff, // (111, 124, 0)
                steelgrey                  = 0x6f828aff, // (111, 130, 138)
                softgreen                  = 0x6fc276ff, // (111, 194, 118)
                bluishpurple               = 0x703be7ff, // (112, 59, 231)
                browngreen                 = 0x706c11ff, // (112, 108, 17)
                nastygreen                 = 0x70b23fff, // (112, 178, 63)
                greyishteal                = 0x719f91ff, // (113, 159, 145)
                leaf                       = 0x71aa34ff, // (113, 170, 52)
                richpurple                 = 0x720058ff, // (114, 0, 88)
                khakigreen                 = 0x728639ff, // (114, 134, 57)
                darkyellowgreen            = 0x728f02ff, // (114, 143, 2)
                merlot                     = 0x730039ff, // (115, 0, 57)
                dirtypurple                = 0x734a65ff, // (115, 74, 101)
                mud                        = 0x735c12ff, // (115, 92, 18)
                steel                      = 0x738595ff, // (115, 133, 149)
                chestnut                   = 0x742802ff, // (116, 40, 2)
                swampgreen                 = 0x748500ff, // (116, 133, 0)
                bluishgrey                 = 0x748b97ff, // (116, 139, 151)
                drabgreen                  = 0x749551ff, // (116, 149, 81)
                dullgreen                  = 0x74a662ff, // (116, 166, 98)
                velvet                     = 0x750851ff, // (117, 8, 81)
                darkishpurple              = 0x751973ff, // (117, 25, 115)
                shitgreen                  = 0x758000ff, // (117, 128, 0)
                bluegrey1                  = 0x758da3ff, // (117, 141, 163)
                turtlegreen                = 0x75b84fff, // (117, 184, 79)
                skyblue                    = 0x75bbfdff, // (117, 187, 253)
                lightergreen               = 0x75fd63ff, // (117, 253, 99)
                brownishpurple             = 0x76424eff, // (118, 66, 78)
                moss                       = 0x769958ff, // (118, 153, 88)
                dustygreen                 = 0x76a973ff, // (118, 169, 115)
                applegreen                 = 0x76cd26ff, // (118, 205, 38)
                lightbluishgreen           = 0x76fda8ff, // (118, 253, 168)
                lightgreen1                = 0x76ff7bff, // (118, 255, 123)
                blood                      = 0x770001ff, // (119, 0, 1)
                greengrey                  = 0x77926fff, // (119, 146, 111)
                greyblue3                  = 0x77a1b5ff, // (119, 161, 181)
                asparagus                  = 0x77ab56ff, // (119, 171, 86)
                greygreen1                 = 0x789b73ff, // (120, 155, 115)
                seafoamblue                = 0x78d1b6ff, // (120, 209, 182)
                poopbrown                  = 0x7a5901ff, // (122, 89, 1)
                purplishgrey               = 0x7a687fff, // (122, 104, 127)
                greyishbrown               = 0x7a6a4fff, // (122, 106, 79)
                uglygreen                  = 0x7a9703ff, // (122, 151, 3)
                seafoamgreen               = 0x7af9abff, // (122, 249, 171)
                bordeaux                   = 0x7b002cff, // (123, 0, 44)
                winered                    = 0x7b0323ff, // (123, 3, 35)
                shitbrown                  = 0x7b5804ff, // (123, 88, 4)
                fadedgreen                 = 0x7bb274ff, // (123, 178, 116)
                lightblue1                 = 0x7bc8f6ff, // (123, 200, 246)
                tiffanyblue                = 0x7bf2daff, // (123, 242, 218)
                lightaquamarine            = 0x7bfdc7ff, // (123, 253, 199)
                uglybrown                  = 0x7d7103ff, // (125, 113, 3)
                mediumgrey                 = 0x7d7f7cff, // (125, 127, 124)
                purple                     = 0x7e1e9cff, // (126, 30, 156)
                bruise                     = 0x7e4071ff, // (126, 64, 113)
                greenygrey                 = 0x7ea07aff, // (126, 160, 122)
                darklimegreen              = 0x7ebd01ff, // (126, 189, 1)
                lightturquoise             = 0x7ef4ccff, // (126, 244, 204)
                lightbluegreen             = 0x7efbb3ff, // (126, 251, 179)
                reddishbrown               = 0x7f2b0aff, // (127, 43, 10)
                milkchocolate              = 0x7f4e1eff, // (127, 78, 30)
                mediumbrown                = 0x7f5112ff, // (127, 81, 18)
                poop                       = 0x7f5e00ff, // (127, 94, 0)
                shit                       = 0x7f5f00ff, // (127, 95, 0)
                darktaupe                  = 0x7f684eff, // (127, 104, 78)
                greybrown                  = 0x7f7053ff, // (127, 112, 83)
                camo                       = 0x7f8f4eff, // (127, 143, 78)
                wine                       = 0x80013fff, // (128, 1, 63)
                mutedpurple                = 0x805b87ff, // (128, 91, 135)
                seafoam                    = 0x80f9adff, // (128, 249, 173)
                redpurple                  = 0x820747ff, // (130, 7, 71)
                dustypurple                = 0x825f87ff, // (130, 95, 135)
                greypurple                 = 0x826d8cff, // (130, 109, 140)
                drab                       = 0x828344ff, // (130, 131, 68)
                greyishgreen               = 0x82a67dff, // (130, 166, 125)
                sky                        = 0x82cafcff, // (130, 202, 252)
                paleteal                   = 0x82cbb2ff, // (130, 203, 178)
                dirtbrown                  = 0x836539ff, // (131, 101, 57)
                darkred                    = 0x840000ff, // (132, 0, 0)
                dullpurple                 = 0x84597eff, // (132, 89, 126)
                darklime                   = 0x84b701ff, // (132, 183, 1)
                indianred                  = 0x850e04ff, // (133, 14, 4)
                darklavender               = 0x856798ff, // (133, 103, 152)
                bluegrey2                  = 0x85a3b2ff, // (133, 163, 178)
                purplegrey                 = 0x866f85ff, // (134, 111, 133)
                brownishgrey               = 0x86775fff, // (134, 119, 95)
                greygreen2                 = 0x86a17dff, // (134, 161, 125)
                darkmauve                  = 0x874c62ff, // (135, 76, 98)
                purpley                    = 0x8756e4ff, // (135, 86, 228)
                cocoa                      = 0x875f42ff, // (135, 95, 66)
                dullbrown                  = 0x876e4bff, // (135, 110, 75)
                avocadogreen               = 0x87a922ff, // (135, 169, 34)
                sage                       = 0x87ae73ff, // (135, 174, 115)
                brightlime                 = 0x87fd05ff, // (135, 253, 5)
                poobrown                   = 0x885f01ff, // (136, 95, 1)
                muddybrown                 = 0x886806ff, // (136, 104, 6)
                greyishpurple              = 0x887191ff, // (136, 113, 145)
                babyshitgreen              = 0x889717ff, // (136, 151, 23)
                sagegreen                  = 0x88b378ff, // (136, 179, 120)
                lighteggplant              = 0x894585ff, // (137, 69, 133)
                duskypurple                = 0x895b7bff, // (137, 91, 123)
                blueygrey                  = 0x89a0b0ff, // (137, 160, 176)
                vomitgreen                 = 0x89a203ff, // (137, 162, 3)
                limegreen                  = 0x89fe05ff, // (137, 254, 5)
                dirt                       = 0x8a6e45ff, // (138, 110, 69)
                carolinablue               = 0x8ab8feff, // (138, 184, 254)
                robineggblue               = 0x8af1feff, // (138, 241, 254)
                redbrown                   = 0x8b2e16ff, // (139, 46, 22)
                rustbrown                  = 0x8b3103ff, // (139, 49, 3)
                lavenderblue               = 0x8b88f8ff, // (139, 136, 248)
                crimson                    = 0x8c000fff, // (140, 0, 15)
                redwine                    = 0x8c0034ff, // (140, 0, 52)
                eastergreen                = 0x8cfd7eff, // (140, 253, 126)
                babygreen                  = 0x8cff9eff, // (140, 255, 158)
                lightaqua                  = 0x8cffdbff, // (140, 255, 219)
                deeplavender               = 0x8d5eb7ff, // (141, 94, 183)
                browngrey                  = 0x8d8468ff, // (141, 132, 104)
                hazel                      = 0x8e7618ff, // (142, 118, 24)
                periwinkle                 = 0x8e82feff, // (142, 130, 254)
                peagreen                   = 0x8eab12ff, // (142, 171, 18)
                kiwigreen                  = 0x8ee53fff, // (142, 229, 63)
                brickred                   = 0x8f1402ff, // (143, 20, 2)
                poo                        = 0x8f7303ff, // (143, 115, 3)
                perrywinkle                = 0x8f8ce7ff, // (143, 140, 231)
                babypoopgreen              = 0x8f9805ff, // (143, 152, 5)
                periwinkleblue             = 0x8f99fbff, // (143, 153, 251)
                ickygreen                  = 0x8fae22ff, // (143, 174, 34)
                lichen                     = 0x8fb67bff, // (143, 182, 123)
                acidgreen                  = 0x8ffe09ff, // (143, 254, 9)
                mintgreen                  = 0x8fff9fff, // (143, 255, 159)
                avocado                    = 0x90b134ff, // (144, 177, 52)
                lightteal                  = 0x90e4c1ff, // (144, 228, 193)
                foamgreen                  = 0x90fda9ff, // (144, 253, 169)
                reddishpurple              = 0x910951ff, // (145, 9, 81)
                fadedpurple                = 0x916e99ff, // (145, 110, 153)
                mulberry                   = 0x920a4eff, // (146, 10, 78)
                brownred                   = 0x922b05ff, // (146, 43, 5)
                grey                       = 0x929591ff, // (146, 149, 145)
                peasoup                    = 0x929901ff, // (146, 153, 1)
                babypoop                   = 0x937c00ff, // (147, 124, 0)
                purplish                   = 0x94568cff, // (148, 86, 140)
                pukebrown                  = 0x947706ff, // (148, 119, 6)
                purpleygrey                = 0x947e94ff, // (148, 126, 148)
                peasoupgreen               = 0x94a617ff, // (148, 166, 23)
                barfgreen                  = 0x94ac02ff, // (148, 172, 2)
                sicklygreen                = 0x94b21cff, // (148, 178, 28)
                warmpurple                 = 0x952e8fff, // (149, 46, 143)
                coolgrey                   = 0x95a3a6ff, // (149, 163, 166)
                lightblue2                 = 0x95d0fcff, // (149, 208, 252)
                darkmagenta                = 0x960056ff, // (150, 0, 86)
                warmbrown                  = 0x964e02ff, // (150, 78, 2)
                deeplilac                  = 0x966ebdff, // (150, 110, 189)
                greenishgrey               = 0x96ae8dff, // (150, 174, 141)
                boogergreen                = 0x96b403ff, // (150, 180, 3)
                lightgreen2                = 0x96f97bff, // (150, 249, 123)
                warmgrey                   = 0x978a84ff, // (151, 138, 132)
                bloodred                   = 0x980002ff, // (152, 0, 2)
                purply                     = 0x983fb2ff, // (152, 63, 178)
                purpleish                  = 0x98568dff, // (152, 86, 141)
                sepia                      = 0x985e2bff, // (152, 94, 43)
                robinseggblue              = 0x98eff9ff, // (152, 239, 249)
                lightseagreen              = 0x98f6b0ff, // (152, 246, 176)
                vividpurple                = 0x9900faff, // (153, 0, 250)
                purplered                  = 0x990147ff, // (153, 1, 71)
                berry                      = 0x990f4bff, // (153, 15, 75)
                reddishgrey                = 0x997570ff, // (153, 117, 112)
                slimegreen                 = 0x99cc04ff, // (153, 204, 4)
                deepred                    = 0x9a0200ff, // (154, 2, 0)
                violet                     = 0x9a0eeaff, // (154, 14, 234)
                auburn                     = 0x9a3001ff, // (154, 48, 1)
                rawsienna                  = 0x9a6200ff, // (154, 98, 0)
                pukegreen                  = 0x9aae07ff, // (154, 174, 7)
                lightgrassgreen            = 0x9af764ff, // (154, 247, 100)
                amethyst                   = 0x9b5fc0ff, // (155, 95, 192)
                yellowishbrown             = 0x9b7a01ff, // (155, 122, 1)
                darkkhaki                  = 0x9b8f55ff, // (155, 143, 85)
                booger                     = 0x9bb53cff, // (155, 181, 60)
                hospitalgreen              = 0x9be5aaff, // (155, 229, 170)
                brownish                   = 0x9c6d57ff, // (156, 109, 87)
                darklilac                  = 0x9c6da5ff, // (156, 109, 165)
                brightolive                = 0x9cbb04ff, // (156, 187, 4)
                kiwi                       = 0x9cef43ff, // (156, 239, 67)
                carmine                    = 0x9d0216ff, // (157, 2, 22)
                darkfuchsia                = 0x9d0759ff, // (157, 7, 89)
                lightplum                  = 0x9d5783ff, // (157, 87, 131)
                mocha                      = 0x9d7651ff, // (157, 118, 81)
                sickgreen                  = 0x9db92cff, // (157, 185, 44)
                lightgreyblue              = 0x9dbcd4ff, // (157, 188, 212)
                snotgreen                  = 0x9dc100ff, // (157, 193, 0)
                brightyellowgreen          = 0x9dff00ff, // (157, 255, 0)
                cranberry                  = 0x9e003aff, // (158, 0, 58)
                redviolet                  = 0x9e0168ff, // (158, 1, 104)
                brownishred                = 0x9e3623ff, // (158, 54, 35)
                mediumpurple               = 0x9e43a2ff, // (158, 67, 162)
                burntred                   = 0x9f2305ff, // (159, 35, 5)
                diarrhea                   = 0x9f8303ff, // (159, 131, 3)
                mint                       = 0x9ffeb0ff, // (159, 254, 176)
                deepmagenta                = 0xa0025cff, // (160, 2, 92)
                barneypurple               = 0xa00498ff, // (160, 4, 152)
                brick                      = 0xa03623ff, // (160, 54, 35)
                burntumber                 = 0xa0450eff, // (160, 69, 14)
                grossgreen                 = 0xa0bf16ff, // (160, 191, 22)
                lightseafoam               = 0xa0febfff, // (160, 254, 191)
                russet                     = 0xa13905ff, // (161, 57, 5)
                lightmaroon                = 0xa24857ff, // (162, 72, 87)
                earth                      = 0xa2653eff, // (162, 101, 62)
                vomit                      = 0xa2a415ff, // (162, 164, 21)
                pastelblue                 = 0xa2bffeff, // (162, 191, 254)
                babyblue                   = 0xa2cffeff, // (162, 207, 254)
                uglypurple                 = 0xa442a0ff, // (164, 66, 160)
                heather                    = 0xa484acff, // (164, 132, 172)
                lightolivegreen            = 0xa4be5cff, // (164, 190, 92)
                pea                        = 0xa4bf20ff, // (164, 191, 32)
                violetred                  = 0xa50055ff, // (165, 0, 85)
                lightishpurple             = 0xa552e6ff, // (165, 82, 230)
                lighterpurple              = 0xa55af4ff, // (165, 90, 244)
                puce                       = 0xa57e52ff, // (165, 126, 82)
                cement                     = 0xa5a391ff, // (165, 163, 145)
                puke                       = 0xa5a502ff, // (165, 165, 2)
                paleturquoise              = 0xa5fbd5ff, // (165, 251, 213)
                softpurple                 = 0xa66fb5ff, // (166, 111, 181)
                coffee                     = 0xa6814cff, // (166, 129, 76)
                lightmossgreen             = 0xa6c875ff, // (166, 200, 117)
                lightmintgreen             = 0xa6fbb2ff, // (166, 251, 178)
                rawumber                   = 0xa75e09ff, // (167, 94, 9)
                lightseafoamgreen          = 0xa7ffb5ff, // (167, 255, 181)
                rust                       = 0xa83c09ff, // (168, 60, 9)
                lightburgundy              = 0xa8415bff, // (168, 65, 91)
                bronze                     = 0xa87900ff, // (168, 121, 0)
                wisteria                   = 0xa87dc2ff, // (168, 125, 194)
                darkmustard                = 0xa88905ff, // (168, 137, 5)
                darksand                   = 0xa88f59ff, // (168, 143, 89)
                greyish                    = 0xa8a495ff, // (168, 164, 149)
                mustardgreen               = 0xa8b504ff, // (168, 181, 4)
                electriclime               = 0xa8ff04ff, // (168, 255, 4)
                darkishred                 = 0xa90308ff, // (169, 3, 8)
                sienna                     = 0xa9561eff, // (169, 86, 30)
                tangreen                   = 0xa9be70ff, // (169, 190, 112)
                springgreen                = 0xa9f971ff, // (169, 249, 113)
                electricpurple             = 0xaa23ffff, // (170, 35, 255)
                rustred                    = 0xaa2704ff, // (170, 39, 4)
                khaki                      = 0xaaa662ff, // (170, 166, 98)
                lime                       = 0xaaff32ff, // (170, 255, 50)
                rouge                      = 0xab1239ff, // (171, 18, 57)
                tanbrown                   = 0xab7e4cff, // (171, 126, 76)
                babypoo                    = 0xab9004ff, // (171, 144, 4)
                barney                     = 0xac1db8ff, // (172, 29, 184)
                cinnamon                   = 0xac4f06ff, // (172, 79, 6)
                leather                    = 0xac7434ff, // (172, 116, 52)
                mustardbrown               = 0xac7e04ff, // (172, 126, 4)
                dustylavender              = 0xac86a8ff, // (172, 134, 168)
                darkbeige                  = 0xac9362ff, // (172, 147, 98)
                snot                       = 0xacbb0dff, // (172, 187, 13)
                lightolive                 = 0xacbf69ff, // (172, 191, 105)
                cloudyblue                 = 0xacc2d9ff, // (172, 194, 217)
                lightcyan                  = 0xacfffcff, // (172, 255, 252)
                vibrantpurple              = 0xad03deff, // (173, 3, 222)
                brightviolet               = 0xad0afdff, // (173, 10, 253)
                lightbrown                 = 0xad8150ff, // (173, 129, 80)
                babyshitbrown              = 0xad900dff, // (173, 144, 13)
                stone                      = 0xada587ff, // (173, 165, 135)
                lemongreen                 = 0xadf802ff, // (173, 248, 2)
                mauve                      = 0xae7181ff, // (174, 113, 129)
                yellowybrown               = 0xae8b0cff, // (174, 139, 12)
                lightlime                  = 0xaefd6cff, // (174, 253, 108)
                keylime                    = 0xaeff6eff, // (174, 255, 110)
                rustyred                   = 0xaf2f0dff, // (175, 47, 13)
                caramel                    = 0xaf6f09ff, // (175, 111, 9)
                darktan                    = 0xaf884aff, // (175, 136, 74)
                bland                      = 0xafa88bff, // (175, 168, 139)
                raspberry                  = 0xb00149ff, // (176, 1, 73)
                purplishred                = 0xb0054bff, // (176, 5, 75)
                burntsienna                = 0xb04e0fff, // (176, 78, 15)
                yellowishgreen             = 0xb0dd16ff, // (176, 221, 22)
                pastelgreen                = 0xb0ff9dff, // (176, 255, 157)
                orangeybrown               = 0xb16002ff, // (177, 96, 2)
                pinkishbrown               = 0xb17261ff, // (177, 114, 97)
                palebrown                  = 0xb1916eff, // (177, 145, 110)
                powderblue                 = 0xb1d1fcff, // (177, 209, 252)
                paleolivegreen             = 0xb1d27bff, // (177, 210, 123)
                palelightgreen             = 0xb1fc99ff, // (177, 252, 153)
                palelimegreen              = 0xb1ff65ff, // (177, 255, 101)
                orangishbrown              = 0xb25f03ff, // (178, 95, 3)
                umber                      = 0xb26400ff, // (178, 100, 0)
                claybrown                  = 0xb2713dff, // (178, 113, 61)
                goldenbrown                = 0xb27a01ff, // (178, 122, 1)
                brownyellow                = 0xb29705ff, // (178, 151, 5)
                dust                       = 0xb2996eff, // (178, 153, 110)
                lightpastelgreen           = 0xb2fba5ff, // (178, 251, 165)
                lighturple                 = 0xb36ff6ff, // (179, 111, 246)
                darkrose                   = 0xb5485dff, // (181, 72, 93)
                darkgold                   = 0xb59410ff, // (181, 148, 16)
                bile                       = 0xb5c306ff, // (181, 195, 6)
                greenyellow1               = 0xb5ce08ff, // (181, 206, 8)
                copper                     = 0xb66325ff, // (182, 99, 37)
                clay                       = 0xb66a50ff, // (182, 106, 80)
                babypukegreen              = 0xb6c406ff, // (182, 196, 6)
                lightmint                  = 0xb6ffbbff, // (182, 255, 187)
                burntsiena                 = 0xb75203ff, // (183, 82, 3)
                palepurple                 = 0xb790d4ff, // (183, 144, 212)
                yellowbrown                = 0xb79400ff, // (183, 148, 0)
                lightbluegrey              = 0xb7c9e2ff, // (183, 201, 226)
                lightgreygreen             = 0xb7e1a1ff, // (183, 225, 161)
                palecyan                   = 0xb7fffaff, // (183, 255, 250)
                paleaqua                   = 0xb8ffebff, // (184, 255, 235)
                dustyred                   = 0xb9484eff, // (185, 72, 78)
                brownorange                = 0xb96902ff, // (185, 105, 2)
                taupe                      = 0xb9a281ff, // (185, 162, 129)
                paleolive                  = 0xb9cc81ff, // (185, 204, 129)
                lightlimegreen             = 0xb9ff66ff, // (185, 255, 102)
                duskyrose                  = 0xba6873ff, // (186, 104, 115)
                mushroom                   = 0xba9e88ff, // (186, 158, 136)
                dullred                    = 0xbb3f3fff, // (187, 63, 63)
                yellowgreen1               = 0xbbf90fff, // (187, 249, 15)
                neonpurple                 = 0xbc13feff, // (188, 19, 254)
                greenishtan                = 0xbccb7aff, // (188, 203, 122)
                lightsage                  = 0xbcecacff, // (188, 236, 172)
                washedoutgreen             = 0xbcf5a6ff, // (188, 245, 166)
                adobe                      = 0xbd6c48ff, // (189, 108, 72)
                paleskyblue                = 0xbdf6feff, // (189, 246, 254)
                teagreen                   = 0xbdf8a3ff, // (189, 248, 163)
                scarlet                    = 0xbe0119ff, // (190, 1, 25)
                rosered                    = 0xbe013cff, // (190, 1, 60)
                brightpurple               = 0xbe03fdff, // (190, 3, 253)
                orangebrown                = 0xbe6400ff, // (190, 100, 0)
                putty                      = 0xbeae8aff, // (190, 174, 138)
                palelime                   = 0xbefd73ff, // (190, 253, 115)
                celadon                    = 0xbefdb7ff, // (190, 253, 183)
                lightpurple                = 0xbf77f6ff, // (191, 119, 246)
                ochre                      = 0xbf9005ff, // (191, 144, 5)
                ocher                      = 0xbf9b0cff, // (191, 155, 12)
                muddyyellow                = 0xbfac05ff, // (191, 172, 5)
                yellowygreen               = 0xbff128ff, // (191, 241, 40)
                lemonlime                  = 0xbffe28ff, // (191, 254, 40)
                lipstickred                = 0xc0022fff, // (192, 2, 47)
                burntorange                = 0xc04e01ff, // (192, 78, 1)
                easterpurple               = 0xc071feff, // (192, 113, 254)
                dustyrose                  = 0xc0737aff, // (192, 115, 122)
                pistachio                  = 0xc0fa8bff, // (192, 250, 139)
                yellowgreen2               = 0xc0fb2dff, // (192, 251, 45)
                brickorange                = 0xc14a09ff, // (193, 74, 9)
                lightperiwinkle            = 0xc1c6fcff, // (193, 198, 252)
                chartreuse                 = 0xc1f80aff, // (193, 248, 10)
                celery                     = 0xc1fd95ff, // (193, 253, 149)
                magenta                    = 0xc20078ff, // (194, 0, 120)
                brownishpink               = 0xc27e79ff, // (194, 126, 121)
                lightmauve                 = 0xc292a1ff, // (194, 146, 161)
                oliveyellow                = 0xc2b709ff, // (194, 183, 9)
                pukeyellow                 = 0xc2be0eff, // (194, 190, 14)
                lightyellowishgreen        = 0xc2ff89ff, // (194, 255, 137)
                greypink                   = 0xc3909bff, // (195, 144, 155)
                duckeggblue                = 0xc3fbf4ff, // (195, 251, 244)
                reddish                    = 0xc44240ff, // (196, 66, 64)
                rustorange                 = 0xc45508ff, // (196, 85, 8)
                liliac                     = 0xc48efdff, // (196, 142, 253)
                sandybrown                 = 0xc4a661ff, // (196, 166, 97)
                lightpeagreen              = 0xc4fe82ff, // (196, 254, 130)
                eggshellblue               = 0xc4fff7ff, // (196, 255, 247)
                silver                     = 0xc5c9c7ff, // (197, 201, 199)
                darkorange                 = 0xc65102ff, // (198, 81, 2)
                ocre                       = 0xc69c04ff, // (198, 156, 4)
                camel                      = 0xc69f59ff, // (198, 159, 89)
                greenyyellow               = 0xc6f808ff, // (198, 248, 8)
                lightskyblue               = 0xc6fcffff, // (198, 252, 255)
                deeprose                   = 0xc74767ff, // (199, 71, 103)
                brightlavender             = 0xc760ffff, // (199, 96, 255)
                oldpink                    = 0xc77986ff, // (199, 121, 134)
                lavender                   = 0xc79fefff, // (199, 159, 239)
                toupe                      = 0xc7ac7dff, // (199, 172, 125)
                vomityellow                = 0xc7c10cff, // (199, 193, 12)
                palegreen                  = 0xc7fdb5ff, // (199, 253, 181)
                purpleypink                = 0xc83cb9ff, // (200, 60, 185)
                darksalmon                 = 0xc85a53ff, // (200, 90, 83)
                orchid                     = 0xc875c4ff, // (200, 117, 196)
                dirtyorange                = 0xc87606ff, // (200, 118, 6)
                oldrose                    = 0xc87f89ff, // (200, 127, 137)
                greyishpink                = 0xc88d94ff, // (200, 141, 148)
                pinkishgrey                = 0xc8aca9ff, // (200, 172, 169)
                yellowgreen3               = 0xc8fd3dff, // (200, 253, 61)
                lightlightgreen            = 0xc8ffb0ff, // (200, 255, 176)
                pinkypurple                = 0xc94cbeff, // (201, 76, 190)
                brightlilac                = 0xc95efbff, // (201, 94, 251)
                terracotta1                = 0xc9643bff, // (201, 100, 59)
                sandstone                  = 0xc9ae74ff, // (201, 174, 116)
                brownishyellow             = 0xc9b003ff, // (201, 176, 3)
                greenishbeige              = 0xc9d179ff, // (201, 209, 121)
                greenyellow2               = 0xc9ff27ff, // (201, 255, 39)
                ruby                       = 0xca0147ff, // (202, 1, 71)
                terracotta2                = 0xca6641ff, // (202, 102, 65)
                brownyorange               = 0xca6b02ff, // (202, 107, 2)
                dirtypink                  = 0xca7b80ff, // (202, 123, 128)
                babypurple                 = 0xca9bf7ff, // (202, 155, 247)
                pastelpurple               = 0xcaa0ffff, // (202, 160, 255)
                lightlightblue             = 0xcafffbff, // (202, 255, 251)
                hotpurple                  = 0xcb00f5ff, // (203, 0, 245)
                deeppink                   = 0xcb0162ff, // (203, 1, 98)
                darkpink                   = 0xcb416bff, // (203, 65, 107)
                terracota                  = 0xcb6843ff, // (203, 104, 67)
                brownishorange             = 0xcb7723ff, // (203, 119, 35)
                yellowochre                = 0xcb9d06ff, // (203, 157, 6)
                sandbrown                  = 0xcba560ff, // (203, 165, 96)
                pear                       = 0xcbf85fff, // (203, 248, 95)
                duskypink                  = 0xcc7a8bff, // (204, 122, 139)
                desert                     = 0xccad60ff, // (204, 173, 96)
                lightyellowgreen           = 0xccfd7fff, // (204, 253, 127)
                rustyorange                = 0xcd5909ff, // (205, 89, 9)
                uglypink                   = 0xcd7584ff, // (205, 117, 132)
                dirtyyellow                = 0xcdc50aff, // (205, 197, 10)
                greenishyellow             = 0xcdfd02ff, // (205, 253, 2)
                purplishpink               = 0xce5daeff, // (206, 93, 174)
                lilac                      = 0xcea2fdff, // (206, 162, 253)
                paleviolet                 = 0xceaefaff, // (206, 174, 250)
                mustard                    = 0xceb301ff, // (206, 179, 1)
                cherry                     = 0xcf0234ff, // (207, 2, 52)
                darkcoral                  = 0xcf524eff, // (207, 82, 78)
                rose                       = 0xcf6275ff, // (207, 98, 117)
                fawn                       = 0xcfaf7bff, // (207, 175, 123)
                verypalegreen              = 0xcffdbcff, // (207, 253, 188)
                neonyellow                 = 0xcfff04ff, // (207, 255, 4)
                uglyyellow                 = 0xd0c101ff, // (208, 193, 1)
                sicklyyellow               = 0xd0e429ff, // (208, 228, 41)
                limeyellow                 = 0xd0fe1dff, // (208, 254, 29)
                paleblue                   = 0xd0fefeff, // (208, 254, 254)
                mutedpink                  = 0xd1768fff, // (209, 118, 143)
                tan                        = 0xd1b26fff, // (209, 178, 111)
                verylightgreen             = 0xd1ffbdff, // (209, 255, 189)
                mustardyellow              = 0xd2bd0aff, // (210, 189, 10)
                fadedred                   = 0xd3494eff, // (211, 73, 78)
                verylightbrown             = 0xd3b683ff, // (211, 182, 131)
                pinkish                    = 0xd46a7eff, // (212, 106, 126)
                reallylightblue            = 0xd4ffffff, // (212, 255, 255)
                lipstick                   = 0xd5174eff, // (213, 23, 78)
                dullpink                   = 0xd5869dff, // (213, 134, 157)
                dustypink                  = 0xd58a94ff, // (213, 138, 148)
                burntyellow                = 0xd5ab09ff, // (213, 171, 9)
                darkyellow                 = 0xd5b60aff, // (213, 182, 10)
                verylightblue              = 0xd5ffffff, // (213, 255, 255)
                pinkishpurple              = 0xd648d7ff, // (214, 72, 215)
                lightviolet                = 0xd6b4fcff, // (214, 180, 252)
                ice                        = 0xd6fffaff, // (214, 255, 250)
                verypaleblue               = 0xd6fffeff, // (214, 255, 254)
                purplepink1                = 0xd725deff, // (215, 37, 222)
                palemagenta                = 0xd767adff, // (215, 103, 173)
                iceblue                    = 0xd7fffeff, // (215, 255, 254)
                dullorange                 = 0xd8863bff, // (216, 134, 59)
                lightgrey                  = 0xd8dcd6ff, // (216, 220, 214)
                darkhotpink                = 0xd90166ff, // (217, 1, 102)
                heliotrope                 = 0xd94ff5ff, // (217, 79, 245)
                palered                    = 0xd9544dff, // (217, 84, 77)
                pinkishtan                 = 0xd99b82ff, // (217, 155, 130)
                darkishpink                = 0xda467dff, // (218, 70, 125)
                pinkpurple1                = 0xdb4bdaff, // (219, 75, 218)
                pastelred                  = 0xdb5856ff, // (219, 88, 86)
                gold                       = 0xdbb40cff, // (219, 180, 12)
                deeporange                 = 0xdc4d01ff, // (220, 77, 1)
                lavenderpink               = 0xdd85d7ff, // (221, 133, 215)
                pissyellow                 = 0xddd618ff, // (221, 214, 24)
                cerise                     = 0xde0c62ff, // (222, 12, 98)
                darkpeach                  = 0xde7e5dff, // (222, 126, 93)
                fadedpink                  = 0xde9dacff, // (222, 157, 172)
                purpleishpink              = 0xdf4ec8ff, // (223, 78, 200)
                lightlavender              = 0xdfc5feff, // (223, 197, 254)
                purplepink2                = 0xe03fd8ff, // (224, 63, 216)
                pumpkin                    = 0xe17701ff, // (225, 119, 1)
                sand                       = 0xe2ca76ff, // (226, 202, 118)
                palelilac                  = 0xe4cbffff, // (228, 203, 255)
                red                        = 0xe50000ff, // (229, 0, 0)
                beige                      = 0xe6daa6ff, // (230, 218, 166)
                lightkhaki                 = 0xe6f2a2ff, // (230, 242, 162)
                pigpink                    = 0xe78ea5ff, // (231, 142, 165)
                tomatored                  = 0xec2d01ff, // (236, 45, 1)
                fuchsia                    = 0xed0dd9ff, // (237, 13, 217)
                lightlilac                 = 0xedc8ffff, // (237, 200, 255)
                palelavender               = 0xeecffeff, // (238, 207, 254)
                dullyellow                 = 0xeedc5bff, // (238, 220, 91)
                pinkpurple2                = 0xef1de7ff, // (239, 29, 231)
                tomato                     = 0xef4026ff, // (239, 64, 38)
                macaroniandcheese          = 0xefb435ff, // (239, 180, 53)
                lightlavendar              = 0xefc0feff, // (239, 192, 254)
                purplypink                 = 0xf075e6ff, // (240, 117, 230)
                dustyorange                = 0xf0833aff, // (240, 131, 58)
                fadedorange                = 0xf0944dff, // (240, 148, 77)
                pinkishred                 = 0xf10c45ff, // (241, 12, 69)
                sandy                      = 0xf1da7aff, // (241, 218, 122)
                offyellow                  = 0xf1f33fff, // (241, 243, 63)
                blush                      = 0xf29e8eff, // (242, 158, 142)
                squash                     = 0xf2ab15ff, // (242, 171, 21)
                mediumpink                 = 0xf36196ff, // (243, 97, 150)
                vermillion                 = 0xf4320cff, // (244, 50, 12)
                orangishred                = 0xf43605ff, // (244, 54, 5)
                maize                      = 0xf4d054ff, // (244, 208, 84)
                hotmagenta                 = 0xf504c9ff, // (245, 4, 201)
                pinkred                    = 0xf5054fff, // (245, 5, 79)
                golden                     = 0xf5bf03ff, // (245, 191, 3)
                rosypink                   = 0xf6688eff, // (246, 104, 142)
                verylightpurple            = 0xf6cefcff, // (246, 206, 252)
                cherryred                  = 0xf7022aff, // (247, 2, 42)
                rosepink                   = 0xf7879aff, // (247, 135, 154)
                lightmustard               = 0xf7d560ff, // (247, 213, 96)
                reddishorange              = 0xf8481cff, // (248, 72, 28)
                orange                     = 0xf97306ff, // (249, 115, 6)
                goldenrod1                 = 0xf9bc08ff, // (249, 188, 8)
                redpink                    = 0xfa2a55ff, // (250, 42, 85)
                orangeyred                 = 0xfa4224ff, // (250, 66, 36)
                lightmagenta               = 0xfa5ff7ff, // (250, 95, 247)
                goldenrod2                 = 0xfac205ff, // (250, 194, 5)
                yellowish                  = 0xfaee66ff, // (250, 238, 102)
                bananayellow               = 0xfafe4bff, // (250, 254, 75)
                strawberry                 = 0xfb2943ff, // (251, 41, 67)
                warmpink                   = 0xfb5581ff, // (251, 85, 129)
                violetpink                 = 0xfb5ffcff, // (251, 95, 252)
                pumpkinorange              = 0xfb7d07ff, // (251, 125, 7)
                wheat                      = 0xfbdd7eff, // (251, 221, 126)
                lighttan                   = 0xfbeeacff, // (251, 238, 172)
                pinkyred                   = 0xfc2647ff, // (252, 38, 71)
                coral                      = 0xfc5a50ff, // (252, 90, 80)
                orangish                   = 0xfc824aff, // (252, 130, 74)
                pinky                      = 0xfc86aaff, // (252, 134, 170)
                yelloworange               = 0xfcb001ff, // (252, 176, 1)
                marigold                   = 0xfcc006ff, // (252, 192, 6)
                sandyellow                 = 0xfce166ff, // (252, 225, 102)
                straw                      = 0xfcf679ff, // (252, 246, 121)
                yellowishtan               = 0xfcfc81ff, // (252, 252, 129)
                redorange                  = 0xfd3c06ff, // (253, 60, 6)
                orangered1                 = 0xfd411eff, // (253, 65, 30)
                watermelon                 = 0xfd4659ff, // (253, 70, 89)
                grapefruit                 = 0xfd5956ff, // (253, 89, 86)
                carnation                  = 0xfd798fff, // (253, 121, 143)
                orangeish                  = 0xfd8d49ff, // (253, 141, 73)
                lightorange                = 0xfdaa48ff, // (253, 170, 72)
                softpink                   = 0xfdb0c0ff, // (253, 176, 192)
                butterscotch               = 0xfdb147ff, // (253, 177, 71)
                orangeyyellow              = 0xfdb915ff, // (253, 185, 21)
                palerose                   = 0xfdc1c5ff, // (253, 193, 197)
                lightgold                  = 0xfddc5cff, // (253, 220, 92)
                palegold                   = 0xfdde6cff, // (253, 222, 108)
                sandyyellow                = 0xfdee73ff, // (253, 238, 115)
                palegrey                   = 0xfdfdfeff, // (253, 253, 254)
                lemonyellow                = 0xfdff38ff, // (253, 255, 56)
                lemon                      = 0xfdff52ff, // (253, 255, 82)
                canary                     = 0xfdff63ff, // (253, 255, 99)
                fireenginered              = 0xfe0002ff, // (254, 0, 2)
                neonpink                   = 0xfe019aff, // (254, 1, 154)
                brightpink                 = 0xfe01b1ff, // (254, 1, 177)
                shockingpink               = 0xfe02a2ff, // (254, 2, 162)
                reddishpink                = 0xfe2c54ff, // (254, 44, 84)
                lightishred                = 0xfe2f4aff, // (254, 47, 74)
                orangered2                 = 0xfe420fff, // (254, 66, 15)
                barbiepink                 = 0xfe46a5ff, // (254, 70, 165)
                bloodorange                = 0xfe4b03ff, // (254, 75, 3)
                salmonpink                 = 0xfe7b7cff, // (254, 123, 124)
                blushpink                  = 0xfe828cff, // (254, 130, 140)
                bubblegumpink1             = 0xfe83ccff, // (254, 131, 204)
                rosa                       = 0xfe86a4ff, // (254, 134, 164)
                lightsalmon                = 0xfea993ff, // (254, 169, 147)
                saffron                    = 0xfeb209ff, // (254, 178, 9)
                amber                      = 0xfeb308ff, // (254, 179, 8)
                goldenyellow               = 0xfec615ff, // (254, 198, 21)
                palemauve                  = 0xfed0fcff, // (254, 208, 252)
                dandelion                  = 0xfedf08ff, // (254, 223, 8)
                buff                       = 0xfef69eff, // (254, 246, 158)
                parchment                  = 0xfefcafff, // (254, 252, 175)
                fadedyellow                = 0xfeff7fff, // (254, 255, 127)
                ecru                       = 0xfeffcaff, // (254, 255, 202)
                brightred                  = 0xff000dff, // (255, 0, 13)
                hotpink                    = 0xff028dff, // (255, 2, 141)
                electricpink               = 0xff0490ff, // (255, 4, 144)
                neonred                    = 0xff073aff, // (255, 7, 58)
                strongpink                 = 0xff0789ff, // (255, 7, 137)
                brightmagenta              = 0xff08e8ff, // (255, 8, 232)
                lightred                   = 0xff474cff, // (255, 71, 76)
                brightorange               = 0xff5b00ff, // (255, 91, 0)
                coralpink                  = 0xff6163ff, // (255, 97, 99)
                candypink                  = 0xff63e9ff, // (255, 99, 233)
                bubblegumpink2             = 0xff69afff, // (255, 105, 175)
                bubblegum                  = 0xff6cb5ff, // (255, 108, 181)
                orangepink                 = 0xff6f52ff, // (255, 111, 82)
                pinkishorange              = 0xff724cff, // (255, 114, 76)
                melon                      = 0xff7855ff, // (255, 120, 85)
                salmon                     = 0xff796cff, // (255, 121, 108)
                carnationpink              = 0xff7fa7ff, // (255, 127, 167)
                pink                       = 0xff81c0ff, // (255, 129, 192)
                tangerine                  = 0xff9408ff, // (255, 148, 8)
                pastelorange               = 0xff964fff, // (255, 150, 79)
                peachypink                 = 0xff9a8aff, // (255, 154, 138)
                mango                      = 0xffa62bff, // (255, 166, 43)
                paleorange                 = 0xffa756ff, // (255, 167, 86)
                yellowishorange            = 0xffab0fff, // (255, 171, 15)
                orangeyellow               = 0xffad01ff, // (255, 173, 1)
                peach                      = 0xffb07cff, // (255, 176, 124)
                apricot                    = 0xffb16dff, // (255, 177, 109)
                palesalmon                 = 0xffb19aff, // (255, 177, 154)
                powderpink                 = 0xffb2d0ff, // (255, 178, 208)
                babypink                   = 0xffb7ceff, // (255, 183, 206)
                pastelpink                 = 0xffbacdff, // (255, 186, 205)
                sunflower                  = 0xffc512ff, // (255, 197, 18)
                lightrose                  = 0xffc5cbff, // (255, 197, 203)
                palepink                   = 0xffcfdcff, // (255, 207, 220)
                lightpink                  = 0xffd1dfff, // (255, 209, 223)
                lightpeach                 = 0xffd8b1ff, // (255, 216, 177)
                sunfloweryellow            = 0xffda03ff, // (255, 218, 3)
                sunyellow                  = 0xffdf22ff, // (255, 223, 34)
                yellowtan                  = 0xffe36eff, // (255, 227, 110)
                palepeach                  = 0xffe5adff, // (255, 229, 173)
                darkcream                  = 0xfff39aff, // (255, 243, 154)
                verylightpink              = 0xfff4f2ff, // (255, 244, 242)
                sunnyyellow                = 0xfff917ff, // (255, 249, 23)
                pale                       = 0xfff9d0ff, // (255, 249, 208)
                manilla                    = 0xfffa86ff, // (255, 250, 134)
                eggshell1                  = 0xfffcc4ff, // (255, 252, 196)
                brightyellow               = 0xfffd01ff, // (255, 253, 1)
                sunshineyellow             = 0xfffd37ff, // (255, 253, 55)
                butteryellow               = 0xfffd74ff, // (255, 253, 116)
                custard                    = 0xfffd78ff, // (255, 253, 120)
                canaryyellow               = 0xfffe40ff, // (255, 254, 64)
                pastelyellow               = 0xfffe71ff, // (255, 254, 113)
                lightyellow                = 0xfffe7aff, // (255, 254, 122)
                lightbeige                 = 0xfffeb6ff, // (255, 254, 182)
                yellow                     = 0xffff14ff, // (255, 255, 20)
                banana                     = 0xffff7eff, // (255, 255, 126)
                butter                     = 0xffff81ff, // (255, 255, 129)
                paleyellow                 = 0xffff84ff, // (255, 255, 132)
                creme                      = 0xffffb6ff, // (255, 255, 182)
                cream                      = 0xffffc2ff, // (255, 255, 194)
                ivory                      = 0xffffcbff, // (255, 255, 203)
                eggshell2                  = 0xffffd4ff, // (255, 255, 212)
                offwhite                   = 0xffffe4ff, // (255, 255, 228)
                white                      = 0xffffffff  // (255, 255, 255)
            };

            const std::array<sf::Color, 949> Array {
                sf::Color(black),
                sf::Color(verydarkblue),
                sf::Color(darknavyblue),
                sf::Color(darkblue1),
                sf::Color(darknavy),
                sf::Color(navyblue),
                sf::Color(darkforestgreen),
                sf::Color(prussianblue),
                sf::Color(darkbluegreen),
                sf::Color(deepteal),
                sf::Color(petrol),
                sf::Color(kelleygreen),
                sf::Color(greenishturquoise),
                sf::Color(cyan),
                sf::Color(trueblue),
                sf::Color(navy),
                sf::Color(marineblue),
                sf::Color(darkishblue),
                sf::Color(racinggreen),
                sf::Color(darkteal),
                sf::Color(deepseablue),
                sf::Color(brightblue),
                sf::Color(peacockblue),
                sf::Color(darkaquamarine),
                sf::Color(deepturquoise),
                sf::Color(bluegreen1),
                sf::Color(ocean),
                sf::Color(tealblue),
                sf::Color(irishgreen),
                sf::Color(emerald),
                sf::Color(shamrock),
                sf::Color(greenblue1),
                sf::Color(brightteal),
                sf::Color(brightgreen),
                sf::Color(midnightblue),
                sf::Color(pureblue),
                sf::Color(darkroyalblue),
                sf::Color(richblue),
                sf::Color(deepgreen),
                sf::Color(emeraldgreen),
                sf::Color(teal),
                sf::Color(kellygreen),
                sf::Color(shamrockgreen),
                sf::Color(brightskyblue),
                sf::Color(aquablue),
                sf::Color(midnight),
                sf::Color(darkblue2),
                sf::Color(cobaltblue),
                sf::Color(blue),
                sf::Color(darkgreen1),
                sf::Color(vibrantblue),
                sf::Color(oceanblue),
                sf::Color(deepblue),
                sf::Color(nightblue),
                sf::Color(marine),
                sf::Color(bottlegreen),
                sf::Color(darkturquoise),
                sf::Color(seablue),
                sf::Color(junglegreen),
                sf::Color(cerulean),
                sf::Color(aquamarine1),
                sf::Color(neonblue),
                sf::Color(turquoisegreen),
                sf::Color(royalblue),
                sf::Color(evergreen),
                sf::Color(britishracinggreen),
                sf::Color(darkgreen2),
                sf::Color(darkaqua),
                sf::Color(ceruleanblue),
                sf::Color(brightseagreen),
                sf::Color(verydarkgreen),
                sf::Color(forestgreen),
                sf::Color(electricblue),
                sf::Color(azure),
                sf::Color(turquoiseblue),
                sf::Color(greenblue2),
                sf::Color(turquoise),
                sf::Color(almostblack),
                sf::Color(primaryblue),
                sf::Color(deepaqua),
                sf::Color(truegreen),
                sf::Color(fluorescentgreen),
                sf::Color(twilightblue),
                sf::Color(pinegreen),
                sf::Color(spruce),
                sf::Color(darkcyan),
                sf::Color(vibrantgreen),
                sf::Color(flurogreen),
                sf::Color(huntergreen),
                sf::Color(forest),
                sf::Color(greenishblue),
                sf::Color(mintygreen),
                sf::Color(brightaqua),
                sf::Color(strongblue),
                sf::Color(royal),
                sf::Color(greenteal),
                sf::Color(tealishgreen),
                sf::Color(neongreen),
                sf::Color(deepskyblue),
                sf::Color(waterblue),
                sf::Color(bluegreen2),
                sf::Color(brightturquoise),
                sf::Color(niceblue),
                sf::Color(bluishgreen),
                sf::Color(darkseagreen),
                sf::Color(aquagreen),
                sf::Color(bluegreen3),
                sf::Color(topaz),
                sf::Color(aqua),
                sf::Color(vividblue),
                sf::Color(forrestgreen),
                sf::Color(lightnavy),
                sf::Color(green),
                sf::Color(ultramarineblue),
                sf::Color(seaweed),
                sf::Color(dark),
                sf::Color(highlightergreen),
                sf::Color(verydarkbrown),
                sf::Color(azul),
                sf::Color(cobalt),
                sf::Color(viridian),
                sf::Color(spearmint),
                sf::Color(darkindigo),
                sf::Color(darkbluegrey),
                sf::Color(darkgreenblue),
                sf::Color(jade),
                sf::Color(darkseafoam),
                sf::Color(ultramarine),
                sf::Color(darkmintgreen),
                sf::Color(wintergreen),
                sf::Color(sapphire),
                sf::Color(darkslateblue),
                sf::Color(algaegreen),
                sf::Color(electricgreen),
                sf::Color(blueblue),
                sf::Color(greenblue3),
                sf::Color(clearblue),
                sf::Color(tealish),
                sf::Color(tealgreen),
                sf::Color(hotgreen),
                sf::Color(duskblue),
                sf::Color(brightlightblue),
                sf::Color(midblue),
                sf::Color(midnightpurple),
                sf::Color(darkishgreen),
                sf::Color(darkgreyblue),
                sf::Color(bluish),
                sf::Color(verydarkpurple),
                sf::Color(treegreen),
                sf::Color(greenishcyan),
                sf::Color(pine),
                sf::Color(jadegreen),
                sf::Color(blueygreen),
                sf::Color(mediumblue),
                sf::Color(radioactivegreen),
                sf::Color(brightlightgreen),
                sf::Color(lightnavyblue),
                sf::Color(aquamarine2),
                sf::Color(vividgreen),
                sf::Color(uglyblue),
                sf::Color(greenishteal),
                sf::Color(coolgreen),
                sf::Color(darkviolet),
                sf::Color(darkbrown),
                sf::Color(charcoal),
                sf::Color(darkpurple),
                sf::Color(navygreen),
                sf::Color(seaweedgreen),
                sf::Color(deeppurple),
                sf::Color(darkgrey),
                sf::Color(darkolive),
                sf::Color(windowsblue),
                sf::Color(indigo),
                sf::Color(eggplant),
                sf::Color(darkgrassgreen),
                sf::Color(mediumgreen),
                sf::Color(indigoblue),
                sf::Color(lightroyalblue),
                sf::Color(weirdgreen),
                sf::Color(denimblue),
                sf::Color(denim),
                sf::Color(mutedblue),
                sf::Color(darkmaroon),
                sf::Color(charcoalgrey),
                sf::Color(darkolivegreen),
                sf::Color(flatblue),
                sf::Color(sea),
                sf::Color(aubergine),
                sf::Color(chocolate),
                sf::Color(lightishblue),
                sf::Color(oceangreen),
                sf::Color(dodgerblue),
                sf::Color(darkseafoamgreen),
                sf::Color(darkplum),
                sf::Color(dirtyblue),
                sf::Color(grassgreen),
                sf::Color(greenish),
                sf::Color(poisongreen),
                sf::Color(deepbrown),
                sf::Color(chocolatebrown),
                sf::Color(grassygreen),
                sf::Color(brightcyan),
                sf::Color(greenyblue),
                sf::Color(eggplantpurple),
                sf::Color(frenchblue),
                sf::Color(darkskyblue),
                sf::Color(blueberry),
                sf::Color(duskyblue),
                sf::Color(darkmint),
                sf::Color(deepviolet),
                sf::Color(dullblue),
                sf::Color(coolblue),
                sf::Color(mahogany),
                sf::Color(royalpurple),
                sf::Color(driedblood),
                sf::Color(warmblue),
                sf::Color(armygreen),
                sf::Color(camouflagegreen),
                sf::Color(dustyteal),
                sf::Color(lawngreen),
                sf::Color(plumpurple),
                sf::Color(twilight),
                sf::Color(dusk),
                sf::Color(cadetblue),
                sf::Color(lightneongreen),
                sf::Color(metallicblue),
                sf::Color(lightforestgreen),
                sf::Color(stormyblue),
                sf::Color(midgreen),
                sf::Color(violetblue),
                sf::Color(slate),
                sf::Color(cornflowerblue),
                sf::Color(leafygreen),
                sf::Color(camogreen),
                sf::Color(bluewithahintofpurple),
                sf::Color(gunmetal),
                sf::Color(seagreen),
                sf::Color(lightbrightgreen),
                sf::Color(greenbrown),
                sf::Color(ferngreen),
                sf::Color(algae),
                sf::Color(blurple),
                sf::Color(offblue),
                sf::Color(darkpastelgreen),
                sf::Color(lightgreenblue),
                sf::Color(bluepurple1),
                sf::Color(plum),
                sf::Color(froggreen),
                sf::Color(slategrey),
                sf::Color(darksage),
                sf::Color(bluepurple2),
                sf::Color(steelblue),
                sf::Color(dustyblue),
                sf::Color(slateblue),
                sf::Color(sapgreen),
                sf::Color(leafgreen),
                sf::Color(grass),
                sf::Color(kermitgreen),
                sf::Color(blueviolet),
                sf::Color(grapepurple),
                sf::Color(purpleblue1),
                sf::Color(greyishblue),
                sf::Color(greyteal),
                sf::Color(greenapple),
                sf::Color(purpleyblue),
                sf::Color(dullteal),
                sf::Color(mutedgreen),
                sf::Color(purplishblue),
                sf::Color(mudbrown),
                sf::Color(mudgreen),
                sf::Color(bluegrey4),
                sf::Color(burgundy),
                sf::Color(purpleishblue),
                sf::Color(toxicgreen),
                sf::Color(lightishgreen),
                sf::Color(blueypurple),
                sf::Color(iris),
                sf::Color(purpleblue2),
                sf::Color(mossygreen),
                sf::Color(fern),
                sf::Color(boringgreen),
                sf::Color(lightgreenishblue),
                sf::Color(olivebrown),
                sf::Color(greyblue1),
                sf::Color(softblue),
                sf::Color(maroon),
                sf::Color(brown),
                sf::Color(muddygreen),
                sf::Color(mossgreen),
                sf::Color(fadedblue),
                sf::Color(slategreen),
                sf::Color(tea),
                sf::Color(brightlimegreen),
                sf::Color(purplyblue),
                sf::Color(darkperiwinkle),
                sf::Color(militarygreen),
                sf::Color(dirtygreen),
                sf::Color(purplebrown),
                sf::Color(olivegreen),
                sf::Color(claret),
                sf::Color(burple),
                sf::Color(greenybrown),
                sf::Color(greenishbrown),
                sf::Color(swamp),
                sf::Color(flatgreen),
                sf::Color(freshgreen),
                sf::Color(brownishgreen),
                sf::Color(cornflower),
                sf::Color(purplishbrown),
                sf::Color(battleshipgrey),
                sf::Color(greyblue2),
                sf::Color(offgreen),
                sf::Color(grape),
                sf::Color(murkygreen),
                sf::Color(lightindigo),
                sf::Color(robinsegg),
                sf::Color(reddybrown),
                sf::Color(olive),
                sf::Color(apple),
                sf::Color(brownygreen),
                sf::Color(olivedrab),
                sf::Color(poopgreen),
                sf::Color(steelgrey),
                sf::Color(softgreen),
                sf::Color(bluishpurple),
                sf::Color(browngreen),
                sf::Color(nastygreen),
                sf::Color(greyishteal),
                sf::Color(leaf),
                sf::Color(richpurple),
                sf::Color(khakigreen),
                sf::Color(darkyellowgreen),
                sf::Color(merlot),
                sf::Color(dirtypurple),
                sf::Color(mud),
                sf::Color(steel),
                sf::Color(chestnut),
                sf::Color(swampgreen),
                sf::Color(bluishgrey),
                sf::Color(drabgreen),
                sf::Color(dullgreen),
                sf::Color(velvet),
                sf::Color(darkishpurple),
                sf::Color(shitgreen),
                sf::Color(bluegrey1),
                sf::Color(turtlegreen),
                sf::Color(skyblue),
                sf::Color(lightergreen),
                sf::Color(brownishpurple),
                sf::Color(moss),
                sf::Color(dustygreen),
                sf::Color(applegreen),
                sf::Color(lightbluishgreen),
                sf::Color(lightgreen1),
                sf::Color(blood),
                sf::Color(greengrey),
                sf::Color(greyblue3),
                sf::Color(asparagus),
                sf::Color(greygreen1),
                sf::Color(seafoamblue),
                sf::Color(poopbrown),
                sf::Color(purplishgrey),
                sf::Color(greyishbrown),
                sf::Color(uglygreen),
                sf::Color(seafoamgreen),
                sf::Color(bordeaux),
                sf::Color(winered),
                sf::Color(shitbrown),
                sf::Color(fadedgreen),
                sf::Color(lightblue1),
                sf::Color(tiffanyblue),
                sf::Color(lightaquamarine),
                sf::Color(uglybrown),
                sf::Color(mediumgrey),
                sf::Color(purple),
                sf::Color(bruise),
                sf::Color(greenygrey),
                sf::Color(darklimegreen),
                sf::Color(lightturquoise),
                sf::Color(lightbluegreen),
                sf::Color(reddishbrown),
                sf::Color(milkchocolate),
                sf::Color(mediumbrown),
                sf::Color(poop),
                sf::Color(shit),
                sf::Color(darktaupe),
                sf::Color(greybrown),
                sf::Color(camo),
                sf::Color(wine),
                sf::Color(mutedpurple),
                sf::Color(seafoam),
                sf::Color(redpurple),
                sf::Color(dustypurple),
                sf::Color(greypurple),
                sf::Color(drab),
                sf::Color(greyishgreen),
                sf::Color(sky),
                sf::Color(paleteal),
                sf::Color(dirtbrown),
                sf::Color(darkred),
                sf::Color(dullpurple),
                sf::Color(darklime),
                sf::Color(indianred),
                sf::Color(darklavender),
                sf::Color(bluegrey2),
                sf::Color(purplegrey),
                sf::Color(brownishgrey),
                sf::Color(greygreen2),
                sf::Color(darkmauve),
                sf::Color(purpley),
                sf::Color(cocoa),
                sf::Color(dullbrown),
                sf::Color(avocadogreen),
                sf::Color(sage),
                sf::Color(brightlime),
                sf::Color(poobrown),
                sf::Color(muddybrown),
                sf::Color(greyishpurple),
                sf::Color(babyshitgreen),
                sf::Color(sagegreen),
                sf::Color(lighteggplant),
                sf::Color(duskypurple),
                sf::Color(blueygrey),
                sf::Color(vomitgreen),
                sf::Color(limegreen),
                sf::Color(dirt),
                sf::Color(carolinablue),
                sf::Color(robineggblue),
                sf::Color(redbrown),
                sf::Color(rustbrown),
                sf::Color(lavenderblue),
                sf::Color(crimson),
                sf::Color(redwine),
                sf::Color(eastergreen),
                sf::Color(babygreen),
                sf::Color(lightaqua),
                sf::Color(deeplavender),
                sf::Color(browngrey),
                sf::Color(hazel),
                sf::Color(periwinkle),
                sf::Color(peagreen),
                sf::Color(kiwigreen),
                sf::Color(brickred),
                sf::Color(poo),
                sf::Color(perrywinkle),
                sf::Color(babypoopgreen),
                sf::Color(periwinkleblue),
                sf::Color(ickygreen),
                sf::Color(lichen),
                sf::Color(acidgreen),
                sf::Color(mintgreen),
                sf::Color(avocado),
                sf::Color(lightteal),
                sf::Color(foamgreen),
                sf::Color(reddishpurple),
                sf::Color(fadedpurple),
                sf::Color(mulberry),
                sf::Color(brownred),
                sf::Color(grey),
                sf::Color(peasoup),
                sf::Color(babypoop),
                sf::Color(purplish),
                sf::Color(pukebrown),
                sf::Color(purpleygrey),
                sf::Color(peasoupgreen),
                sf::Color(barfgreen),
                sf::Color(sicklygreen),
                sf::Color(warmpurple),
                sf::Color(coolgrey),
                sf::Color(lightblue2),
                sf::Color(darkmagenta),
                sf::Color(warmbrown),
                sf::Color(deeplilac),
                sf::Color(greenishgrey),
                sf::Color(boogergreen),
                sf::Color(lightgreen2),
                sf::Color(warmgrey),
                sf::Color(bloodred),
                sf::Color(purply),
                sf::Color(purpleish),
                sf::Color(sepia),
                sf::Color(robinseggblue),
                sf::Color(lightseagreen),
                sf::Color(vividpurple),
                sf::Color(purplered),
                sf::Color(berry),
                sf::Color(reddishgrey),
                sf::Color(slimegreen),
                sf::Color(deepred),
                sf::Color(violet),
                sf::Color(auburn),
                sf::Color(rawsienna),
                sf::Color(pukegreen),
                sf::Color(lightgrassgreen),
                sf::Color(amethyst),
                sf::Color(yellowishbrown),
                sf::Color(darkkhaki),
                sf::Color(booger),
                sf::Color(hospitalgreen),
                sf::Color(brownish),
                sf::Color(darklilac),
                sf::Color(brightolive),
                sf::Color(kiwi),
                sf::Color(carmine),
                sf::Color(darkfuchsia),
                sf::Color(lightplum),
                sf::Color(mocha),
                sf::Color(sickgreen),
                sf::Color(lightgreyblue),
                sf::Color(snotgreen),
                sf::Color(brightyellowgreen),
                sf::Color(cranberry),
                sf::Color(redviolet),
                sf::Color(brownishred),
                sf::Color(mediumpurple),
                sf::Color(burntred),
                sf::Color(diarrhea),
                sf::Color(mint),
                sf::Color(deepmagenta),
                sf::Color(barneypurple),
                sf::Color(brick),
                sf::Color(burntumber),
                sf::Color(grossgreen),
                sf::Color(lightseafoam),
                sf::Color(russet),
                sf::Color(lightmaroon),
                sf::Color(earth),
                sf::Color(vomit),
                sf::Color(pastelblue),
                sf::Color(babyblue),
                sf::Color(uglypurple),
                sf::Color(heather),
                sf::Color(lightolivegreen),
                sf::Color(pea),
                sf::Color(violetred),
                sf::Color(lightishpurple),
                sf::Color(lighterpurple),
                sf::Color(puce),
                sf::Color(cement),
                sf::Color(puke),
                sf::Color(paleturquoise),
                sf::Color(softpurple),
                sf::Color(coffee),
                sf::Color(lightmossgreen),
                sf::Color(lightmintgreen),
                sf::Color(rawumber),
                sf::Color(lightseafoamgreen),
                sf::Color(rust),
                sf::Color(lightburgundy),
                sf::Color(bronze),
                sf::Color(wisteria),
                sf::Color(darkmustard),
                sf::Color(darksand),
                sf::Color(greyish),
                sf::Color(mustardgreen),
                sf::Color(electriclime),
                sf::Color(darkishred),
                sf::Color(sienna),
                sf::Color(tangreen),
                sf::Color(springgreen),
                sf::Color(electricpurple),
                sf::Color(rustred),
                sf::Color(khaki),
                sf::Color(lime),
                sf::Color(rouge),
                sf::Color(tanbrown),
                sf::Color(babypoo),
                sf::Color(barney),
                sf::Color(cinnamon),
                sf::Color(leather),
                sf::Color(mustardbrown),
                sf::Color(dustylavender),
                sf::Color(darkbeige),
                sf::Color(snot),
                sf::Color(lightolive),
                sf::Color(cloudyblue),
                sf::Color(lightcyan),
                sf::Color(vibrantpurple),
                sf::Color(brightviolet),
                sf::Color(lightbrown),
                sf::Color(babyshitbrown),
                sf::Color(stone),
                sf::Color(lemongreen),
                sf::Color(mauve),
                sf::Color(yellowybrown),
                sf::Color(lightlime),
                sf::Color(keylime),
                sf::Color(rustyred),
                sf::Color(caramel),
                sf::Color(darktan),
                sf::Color(bland),
                sf::Color(raspberry),
                sf::Color(purplishred),
                sf::Color(burntsienna),
                sf::Color(yellowishgreen),
                sf::Color(pastelgreen),
                sf::Color(orangeybrown),
                sf::Color(pinkishbrown),
                sf::Color(palebrown),
                sf::Color(powderblue),
                sf::Color(paleolivegreen),
                sf::Color(palelightgreen),
                sf::Color(palelimegreen),
                sf::Color(orangishbrown),
                sf::Color(umber),
                sf::Color(claybrown),
                sf::Color(goldenbrown),
                sf::Color(brownyellow),
                sf::Color(dust),
                sf::Color(lightpastelgreen),
                sf::Color(lighturple),
                sf::Color(darkrose),
                sf::Color(darkgold),
                sf::Color(bile),
                sf::Color(greenyellow1),
                sf::Color(copper),
                sf::Color(clay),
                sf::Color(babypukegreen),
                sf::Color(lightmint),
                sf::Color(burntsiena),
                sf::Color(palepurple),
                sf::Color(yellowbrown),
                sf::Color(lightbluegrey),
                sf::Color(lightgreygreen),
                sf::Color(palecyan),
                sf::Color(paleaqua),
                sf::Color(dustyred),
                sf::Color(brownorange),
                sf::Color(taupe),
                sf::Color(paleolive),
                sf::Color(lightlimegreen),
                sf::Color(duskyrose),
                sf::Color(mushroom),
                sf::Color(dullred),
                sf::Color(yellowgreen1),
                sf::Color(neonpurple),
                sf::Color(greenishtan),
                sf::Color(lightsage),
                sf::Color(washedoutgreen),
                sf::Color(adobe),
                sf::Color(paleskyblue),
                sf::Color(teagreen),
                sf::Color(scarlet),
                sf::Color(rosered),
                sf::Color(brightpurple),
                sf::Color(orangebrown),
                sf::Color(putty),
                sf::Color(palelime),
                sf::Color(celadon),
                sf::Color(lightpurple),
                sf::Color(ochre),
                sf::Color(ocher),
                sf::Color(muddyyellow),
                sf::Color(yellowygreen),
                sf::Color(lemonlime),
                sf::Color(lipstickred),
                sf::Color(burntorange),
                sf::Color(easterpurple),
                sf::Color(dustyrose),
                sf::Color(pistachio),
                sf::Color(yellowgreen2),
                sf::Color(brickorange),
                sf::Color(lightperiwinkle),
                sf::Color(chartreuse),
                sf::Color(celery),
                sf::Color(magenta),
                sf::Color(brownishpink),
                sf::Color(lightmauve),
                sf::Color(oliveyellow),
                sf::Color(pukeyellow),
                sf::Color(lightyellowishgreen),
                sf::Color(greypink),
                sf::Color(duckeggblue),
                sf::Color(reddish),
                sf::Color(rustorange),
                sf::Color(liliac),
                sf::Color(sandybrown),
                sf::Color(lightpeagreen),
                sf::Color(eggshellblue),
                sf::Color(silver),
                sf::Color(darkorange),
                sf::Color(ocre),
                sf::Color(camel),
                sf::Color(greenyyellow),
                sf::Color(lightskyblue),
                sf::Color(deeprose),
                sf::Color(brightlavender),
                sf::Color(oldpink),
                sf::Color(lavender),
                sf::Color(toupe),
                sf::Color(vomityellow),
                sf::Color(palegreen),
                sf::Color(purpleypink),
                sf::Color(darksalmon),
                sf::Color(orchid),
                sf::Color(dirtyorange),
                sf::Color(oldrose),
                sf::Color(greyishpink),
                sf::Color(pinkishgrey),
                sf::Color(yellowgreen3),
                sf::Color(lightlightgreen),
                sf::Color(pinkypurple),
                sf::Color(brightlilac),
                sf::Color(terracotta1),
                sf::Color(sandstone),
                sf::Color(brownishyellow),
                sf::Color(greenishbeige),
                sf::Color(greenyellow2),
                sf::Color(ruby),
                sf::Color(terracotta2),
                sf::Color(brownyorange),
                sf::Color(dirtypink),
                sf::Color(babypurple),
                sf::Color(pastelpurple),
                sf::Color(lightlightblue),
                sf::Color(hotpurple),
                sf::Color(deeppink),
                sf::Color(darkpink),
                sf::Color(terracota),
                sf::Color(brownishorange),
                sf::Color(yellowochre),
                sf::Color(sandbrown),
                sf::Color(pear),
                sf::Color(duskypink),
                sf::Color(desert),
                sf::Color(lightyellowgreen),
                sf::Color(rustyorange),
                sf::Color(uglypink),
                sf::Color(dirtyyellow),
                sf::Color(greenishyellow),
                sf::Color(purplishpink),
                sf::Color(lilac),
                sf::Color(paleviolet),
                sf::Color(mustard),
                sf::Color(cherry),
                sf::Color(darkcoral),
                sf::Color(rose),
                sf::Color(fawn),
                sf::Color(verypalegreen),
                sf::Color(neonyellow),
                sf::Color(uglyyellow),
                sf::Color(sicklyyellow),
                sf::Color(limeyellow),
                sf::Color(paleblue),
                sf::Color(mutedpink),
                sf::Color(tan),
                sf::Color(verylightgreen),
                sf::Color(mustardyellow),
                sf::Color(fadedred),
                sf::Color(verylightbrown),
                sf::Color(pinkish),
                sf::Color(reallylightblue),
                sf::Color(lipstick),
                sf::Color(dullpink),
                sf::Color(dustypink),
                sf::Color(burntyellow),
                sf::Color(darkyellow),
                sf::Color(verylightblue),
                sf::Color(pinkishpurple),
                sf::Color(lightviolet),
                sf::Color(ice),
                sf::Color(verypaleblue),
                sf::Color(purplepink1),
                sf::Color(palemagenta),
                sf::Color(iceblue),
                sf::Color(dullorange),
                sf::Color(lightgrey),
                sf::Color(darkhotpink),
                sf::Color(heliotrope),
                sf::Color(palered),
                sf::Color(pinkishtan),
                sf::Color(darkishpink),
                sf::Color(pinkpurple1),
                sf::Color(pastelred),
                sf::Color(gold),
                sf::Color(deeporange),
                sf::Color(lavenderpink),
                sf::Color(pissyellow),
                sf::Color(cerise),
                sf::Color(darkpeach),
                sf::Color(fadedpink),
                sf::Color(purpleishpink),
                sf::Color(lightlavender),
                sf::Color(purplepink2),
                sf::Color(pumpkin),
                sf::Color(sand),
                sf::Color(palelilac),
                sf::Color(red),
                sf::Color(beige),
                sf::Color(lightkhaki),
                sf::Color(pigpink),
                sf::Color(tomatored),
                sf::Color(fuchsia),
                sf::Color(lightlilac),
                sf::Color(palelavender),
                sf::Color(dullyellow),
                sf::Color(pinkpurple2),
                sf::Color(tomato),
                sf::Color(macaroniandcheese),
                sf::Color(lightlavendar),
                sf::Color(purplypink),
                sf::Color(dustyorange),
                sf::Color(fadedorange),
                sf::Color(pinkishred),
                sf::Color(sandy),
                sf::Color(offyellow),
                sf::Color(blush),
                sf::Color(squash),
                sf::Color(mediumpink),
                sf::Color(vermillion),
                sf::Color(orangishred),
                sf::Color(maize),
                sf::Color(hotmagenta),
                sf::Color(pinkred),
                sf::Color(golden),
                sf::Color(rosypink),
                sf::Color(verylightpurple),
                sf::Color(cherryred),
                sf::Color(rosepink),
                sf::Color(lightmustard),
                sf::Color(reddishorange),
                sf::Color(orange),
                sf::Color(goldenrod1),
                sf::Color(redpink),
                sf::Color(orangeyred),
                sf::Color(lightmagenta),
                sf::Color(goldenrod2),
                sf::Color(yellowish),
                sf::Color(bananayellow),
                sf::Color(strawberry),
                sf::Color(warmpink),
                sf::Color(violetpink),
                sf::Color(pumpkinorange),
                sf::Color(wheat),
                sf::Color(lighttan),
                sf::Color(pinkyred),
                sf::Color(coral),
                sf::Color(orangish),
                sf::Color(pinky),
                sf::Color(yelloworange),
                sf::Color(marigold),
                sf::Color(sandyellow),
                sf::Color(straw),
                sf::Color(yellowishtan),
                sf::Color(redorange),
                sf::Color(orangered1),
                sf::Color(watermelon),
                sf::Color(grapefruit),
                sf::Color(carnation),
                sf::Color(orangeish),
                sf::Color(lightorange),
                sf::Color(softpink),
                sf::Color(butterscotch),
                sf::Color(orangeyyellow),
                sf::Color(palerose),
                sf::Color(lightgold),
                sf::Color(palegold),
                sf::Color(sandyyellow),
                sf::Color(palegrey),
                sf::Color(lemonyellow),
                sf::Color(lemon),
                sf::Color(canary),
                sf::Color(fireenginered),
                sf::Color(neonpink),
                sf::Color(brightpink),
                sf::Color(shockingpink),
                sf::Color(reddishpink),
                sf::Color(lightishred),
                sf::Color(orangered2),
                sf::Color(barbiepink),
                sf::Color(bloodorange),
                sf::Color(salmonpink),
                sf::Color(blushpink),
                sf::Color(bubblegumpink1),
                sf::Color(rosa),
                sf::Color(lightsalmon),
                sf::Color(saffron),
                sf::Color(amber),
                sf::Color(goldenyellow),
                sf::Color(palemauve),
                sf::Color(dandelion),
                sf::Color(buff),
                sf::Color(parchment),
                sf::Color(fadedyellow),
                sf::Color(ecru),
                sf::Color(brightred),
                sf::Color(hotpink),
                sf::Color(electricpink),
                sf::Color(neonred),
                sf::Color(strongpink),
                sf::Color(brightmagenta),
                sf::Color(lightred),
                sf::Color(brightorange),
                sf::Color(coralpink),
                sf::Color(candypink),
                sf::Color(bubblegumpink2),
                sf::Color(bubblegum),
                sf::Color(orangepink),
                sf::Color(pinkishorange),
                sf::Color(melon),
                sf::Color(salmon),
                sf::Color(carnationpink),
                sf::Color(pink),
                sf::Color(tangerine),
                sf::Color(pastelorange),
                sf::Color(peachypink),
                sf::Color(mango),
                sf::Color(paleorange),
                sf::Color(yellowishorange),
                sf::Color(orangeyellow),
                sf::Color(peach),
                sf::Color(apricot),
                sf::Color(palesalmon),
                sf::Color(powderpink),
                sf::Color(babypink),
                sf::Color(pastelpink),
                sf::Color(sunflower),
                sf::Color(lightrose),
                sf::Color(palepink),
                sf::Color(lightpink),
                sf::Color(lightpeach),
                sf::Color(sunfloweryellow),
                sf::Color(sunyellow),
                sf::Color(yellowtan),
                sf::Color(palepeach),
                sf::Color(darkcream),
                sf::Color(verylightpink),
                sf::Color(sunnyyellow),
                sf::Color(pale),
                sf::Color(manilla),
                sf::Color(eggshell1),
                sf::Color(brightyellow),
                sf::Color(sunshineyellow),
                sf::Color(butteryellow),
                sf::Color(custard),
                sf::Color(canaryyellow),
                sf::Color(pastelyellow),
                sf::Color(lightyellow),
                sf::Color(lightbeige),
                sf::Color(yellow),
                sf::Color(banana),
                sf::Color(butter),
                sf::Color(paleyellow),
                sf::Color(creme),
                sf::Color(cream),
                sf::Color(ivory),
                sf::Color(eggshell2),
                sf::Color(offwhite),
                sf::Color(white) };

            constexpr std::size_t Count{ Array.size() };
            // clang-format on

        } // namespace xkcd

    } // namespace pallete

} // namespace color

#endif // SFUTIL_COLOR_NAMES_HPP_INCLUDED
