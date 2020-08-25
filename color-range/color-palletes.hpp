#ifndef SFUTIL_COLOR_NAMES_HPP_INCLUDED
#define SFUTIL_COLOR_NAMES_HPP_INCLUDED
//
// color-palletes.hpp
//
#include <string>
#include <vector>

#include <SFML/Graphics/Color.hpp>

//
// TODO
//  add a self test to see if any pallets have duplicates values or names

namespace color
{
    namespace pallete
    {
        // All of these pallets start with black and end with white.

        // All 8 Primary and Secondary RGB colors including black and white.
        //   (all combinations of 0 and 255 values)
        struct rgb_8
        {
            // clang-format off
            enum Enum : sf::Uint32
            {
                black   = 0x000000ff,
                red     = 0xff0000ff,
                green   = 0x00ff00ff,
                blue    = 0x0000ffff,
                cyan    = 0x00ffffff,
                magenta = 0xff00ffff,
                yellow  = 0xffff00ff,
                white   = 0xffffffff,
                Count = 8
            };

            static inline const std::vector<sf::Color> Colors{
                sf::Color(black),
                sf::Color(red),
                sf::Color(green),
                sf::Color(blue),
                sf::Color(cyan),
                sf::Color(magenta),
                sf::Color(yellow),
                sf::Color(white)
            };
            // clang-format on

            static inline const std::string palleteName { "rgb_8" };
        };

        // A muted pallet used by the classic home computer from 1982.
        struct commodore_16
        {
            // clang-format off
            enum Enum : sf::Uint32
            {
                black      = 0x000000ff,
                red        = 0x880000ff,
                cyan       = 0xaaffeeff,
                purple     = 0xcc44ccff,
                green      = 0x00cc55ff,
                blue       = 0x0000aaff,
                yellow     = 0xeeee77ff,
                orange     = 0xdd8855ff,
                //
                brown      = 0x664400ff,
                lightred   = 0xff7777ff,
                darkgrey   = 0x333333ff,
                grey       = 0x777777ff,
                lightgreen = 0xaaff66ff,
                lightblue  = 0x0088ffff,
                lightgrey  = 0xbbbbbbff,
                white      = 0xffffffff,
                Count = 16
            };

            static inline const std::vector<sf::Color> Colors{
                sf::Color(black),
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
                sf::Color(lightgrey),
                sf::Color(white)
            };
            // clang-format on

            static inline const std::string palleteName { "commodore_16" };
        };

        // The awkward but instantly recognizable pallet of the Apple IIgs from 1997.
        //  Note that some of these colors are out of the RGB model so they look weird on modern
        //  displays.  For example, the brown here is not brown at all on modern RGB displays.
        struct appleiigs_16
        {
            // clang-format off
            enum Enum : sf::Uint32
            {
                black     = 0x000000ff,
                magenta   = 0x7c3f51ff,
                darkblue  = 0x4e4985ff,
                purple    = 0xd96ae9ff,
                darkgreen = 0x246653ff,
                grey1     = 0x929292ff,
                medblue   = 0x37a6eaff,
                lightblue = 0xc8c4f4ff,
                //
                brown     = 0x535b1fff,
                orange    = 0xdd833cff,
                grey2     = 0x929292ff,
                pink      = 0xedbccaff,
                green     = 0x4fc546ff,
                yellow    = 0xccd2a1ff,
                aqua      = 0xa8dacbff,
                white     = 0xffffffff,
                Count = 16
            };

            static inline const std::vector<sf::Color> Colors{
                sf::Color(black),
                sf::Color(magenta),
                sf::Color(darkblue),
                sf::Color(purple),
                sf::Color(darkgreen),
                sf::Color(grey1),
                sf::Color(medblue),
                sf::Color(lightblue),
                sf::Color(brown),
                sf::Color(orange),
                sf::Color(grey2),
                sf::Color(pink),
                sf::Color(green),
                sf::Color(yellow),
                sf::Color(aqua),
                sf::Color(white)
            };
            // clang-format on

            static inline const std::string palleteName { "appleiigs_16" };
        };

        // 16 eye-bleeding colors from good old windows 3.1.
        struct windows_16
        {
            // clang-format off
            enum Enum : sf::Uint32
            {
                black  = 0x000000ff,
                maroon = 0x7e0000ff,
                green  = 0x047e00ff,
                olive  = 0x7e7e00ff,
                navy   = 0x00007eff,
                purple = 0x7e007eff,
                teal   = 0x047e7eff,
                silver = 0xbebebeff,
                //
                grey    = 0x7e7e7eff,
                red     = 0xfe0000ff,
                lime    = 0x06ff04ff,
                yellow  = 0xffff04ff,
                blue    = 0x0000ffff,
                fuchsia = 0xfe00ffff,
                aqua    = 0x06ffffff,
                white   = 0xffffffff,
                Count = 16
            };

           static inline const std::vector<sf::Color> Colors{
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
            // clang-format on

            static inline const std::string palleteName { "windows_16" };
        };

        // The original Color-Graphics-Adapter pallet from 1982.
        // This is NOT the cga pallet that became the standard adopted by EGA/VGA/
        struct cga_orig_16
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
                darkyellow   = 0xaaaa00ff,
                lightgrey    = 0xaaaaaaff,
                darkgrey     = 0x555555ff,
                lightblue    = 0x5555ffff,
                lightgreen   = 0x55ff55ff,
                lightcyan    = 0x55ffffff,
                lightred     = 0xff5555ff,
                lightmagenta = 0xff55ffff,
                yellow       = 0xffff55ff,
                white        = 0xffffffff,
                Count = 16
            };

           static inline const std::vector<sf::Color> Colors{
                sf::Color(black),
                sf::Color(blue),
                sf::Color(green),
                sf::Color(cyan),
                sf::Color(red),
                sf::Color(magenta),
                sf::Color(darkyellow),
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
            // clang-format on

            static inline const std::string palleteName { "cga_orig_16" };
        };

        // The "RGBI with tweaked brown" version of the Color-Graphics-Adapter pallet
        //  The only difference from cga_orig_16 is color #6 darkyellow, which was changed to
        //  "brown" with special circuitry in RGBI monitors (i.e. IBM 5153) to half the analogue
        //  green signal's amplitude.  This version of the CGA pallet became the default used by
        //  the standards that came after, such as EGA/VGA/etc.
        struct cga_16
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
                white        = 0xffffffff,
                Count = 16
            };

           static inline const std::vector<sf::Color> Colors{
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
            // clang-format on

            static inline const std::string palleteName { "cga_16" };
        };

        // Enhanced Graphics Adapter from 1984.
        //  EGA could only use 16 colors at once, but those colors could be selected from a pallet
        //  of 64.  I didn't bother making the default 16 color EGA pallet because it's the same as
        //  the "brown tweaked" CGA pallet above.
        struct ega_64
        {
            // clang-format off
            enum Enum : sf::Uint32
            {
                black = 0x000000ff,
                ega_00 = black,
                ega_01 = 0x0000aaff,
                ega_02 = 0x00aa00ff,
                ega_03 = 0x00aaaaff,
                ega_04 = 0xaa0000ff,
                ega_05 = 0xaa00aaff,
                ega_06 = 0xaaaa00ff,
                ega_07 = 0xaaaaaaff,
                ega_08 = 0x000055ff,
                ega_09 = 0x0000ffff,
                ega_10 = 0x00aa55ff,
                ega_11 = 0x00aaffff,
                ega_12 = 0xaa0055ff,
                ega_13 = 0xaa00ffff,
                ega_14 = 0xaaaa55ff,
                ega_15 = 0xaaaaffff,
                ega_16 = 0x005500ff,
                ega_17 = 0x0055aaff,
                ega_18 = 0x00ff00ff,
                ega_19 = 0x00ffaaff,
                ega_20 = 0xaa5500ff,
                ega_21 = 0xaa55aaff,
                ega_22 = 0xaaff00ff,
                ega_23 = 0xaaffaaff,
                ega_24 = 0x005555ff,
                ega_25 = 0x0055ffff,
                ega_26 = 0x00ff55ff,
                ega_27 = 0x00ffffff,
                ega_28 = 0xaa5555ff,
                ega_29 = 0xaa55ffff,
                ega_30 = 0xaaff55ff,
                ega_31 = 0xaaffffff,
                ega_32 = 0x550000ff,
                ega_33 = 0x5500aaff,
                ega_34 = 0x55aa00ff,
                ega_35 = 0x55aaaaff,
                ega_36 = 0xff0000ff,
                ega_37 = 0xff00aaff,
                ega_38 = 0xffaa00ff,
                ega_39 = 0xffaaaaff,
                ega_40 = 0x550055ff,
                ega_41 = 0x5500ffff,
                ega_42 = 0x55aa55ff,
                ega_43 = 0x55aaffff,
                ega_44 = 0xff0055ff,
                ega_45 = 0xff00ffff,
                ega_46 = 0xffaa55ff,
                ega_47 = 0xffaaffff,
                ega_48 = 0x555500ff,
                ega_49 = 0x5555aaff,
                ega_50 = 0x55ff00ff,
                ega_51 = 0x55ffaaff,
                ega_52 = 0xff5500ff,
                ega_53 = 0xff55aaff,
                ega_54 = 0xffff00ff,
                ega_55 = 0xffffaaff,
                ega_56 = 0x555555ff,
                ega_57 = 0x5555ffff,
                ega_58 = 0x55ff55ff,
                ega_59 = 0x55ffffff,
                ega_60 = 0xff5555ff,
                ega_61 = 0xff55ffff,
                ega_62 = 0xffff55ff,
                ega_63 = 0xffffffff,
                white = ega_63,
                Count = 64
            };

           static inline const std::vector<sf::Color> Colors{
                sf::Color(ega_00),
                sf::Color(ega_01),
                sf::Color(ega_02),
                sf::Color(ega_03),
                sf::Color(ega_04),
                sf::Color(ega_05),
                sf::Color(ega_06),
                sf::Color(ega_07),
                sf::Color(ega_08),
                sf::Color(ega_09),
                sf::Color(ega_10),
                sf::Color(ega_11),
                sf::Color(ega_12),
                sf::Color(ega_13),
                sf::Color(ega_14),
                sf::Color(ega_15),
                sf::Color(ega_16),
                sf::Color(ega_17),
                sf::Color(ega_18),
                sf::Color(ega_19),
                sf::Color(ega_20),
                sf::Color(ega_21),
                sf::Color(ega_22),
                sf::Color(ega_23),
                sf::Color(ega_24),
                sf::Color(ega_25),
                sf::Color(ega_26),
                sf::Color(ega_27),
                sf::Color(ega_28),
                sf::Color(ega_29),
                sf::Color(ega_30),
                sf::Color(ega_31),
                sf::Color(ega_32),
                sf::Color(ega_33),
                sf::Color(ega_34),
                sf::Color(ega_35),
                sf::Color(ega_36),
                sf::Color(ega_37),
                sf::Color(ega_38),
                sf::Color(ega_39),
                sf::Color(ega_40),
                sf::Color(ega_41),
                sf::Color(ega_42),
                sf::Color(ega_43),
                sf::Color(ega_44),
                sf::Color(ega_45),
                sf::Color(ega_46),
                sf::Color(ega_47),
                sf::Color(ega_48),
                sf::Color(ega_49),
                sf::Color(ega_50),
                sf::Color(ega_51),
                sf::Color(ega_52),
                sf::Color(ega_53),
                sf::Color(ega_54),
                sf::Color(ega_55),
                sf::Color(ega_56),
                sf::Color(ega_57),
                sf::Color(ega_58),
                sf::Color(ega_59),
                sf::Color(ega_60),
                sf::Color(ega_61),
                sf::Color(ega_62),
                sf::Color(ega_63)
            };
            // clang-format on

            static inline const std::string palleteName { "ega_64" };
        };

        // The Video Graphics Array from 1987.
        //  VGA could only use 256 colors at once, but those colors could be selected from a pallet
        //  of 262,144.  Again, I didn't bother making the default 16 color EGA pallet because it's
        //  the same as the "brown tweaked" CGA pallet above.  Note that the repeats of black at
        //  the end were intentional.  You will have to dig into the history to understand why...
        //  I changed the last duplicate black into white so that all the pallets in this file start
        //  with black and end with white.
        struct vga_256
        {
            // clang-format off
            enum Enum : sf::Uint32
            {
                black = 0x000000ff,
                vga_000 = black,
                vga_001 = 0x0000aaff,
                vga_002 = 0x00aa00ff,
                vga_003 = 0x00aaaaff,
                vga_004 = 0xaa0000ff,
                vga_005 = 0xaa00aaff,
                vga_006 = 0xaa5500ff,
                vga_007 = 0xaaaaaaff,
                vga_008 = 0x555555ff,
                vga_009 = 0x5555ffff,
                vga_010 = 0x55ff55ff,
                vga_011 = 0x55ffffff,
                vga_012 = 0xff5555ff,
                vga_013 = 0xff55ffff,
                vga_014 = 0xffff55ff,
                vga_015 = 0xffffffff,
                vga_016 = 0x000000ff,
                vga_017 = 0x101010ff,
                vga_018 = 0x202020ff,
                vga_019 = 0x353535ff,
                vga_020 = 0x454545ff,
                vga_021 = 0x555555ff,
                vga_022 = 0x656565ff,
                vga_023 = 0x757575ff,
                vga_024 = 0x8a8a8aff,
                vga_025 = 0x9a9a9aff,
                vga_026 = 0xaaaaaaff,
                vga_027 = 0xbababaff,
                vga_028 = 0xcacacaff,
                vga_029 = 0xdfdfdfff,
                vga_030 = 0xefefefff,
                vga_031 = 0xffffffff,
                vga_032 = 0x0000ffff,
                vga_033 = 0x4100ffff,
                vga_034 = 0x8200ffff,
                vga_035 = 0xbe00ffff,
                vga_036 = 0xff00ffff,
                vga_037 = 0xff00beff,
                vga_038 = 0xff0082ff,
                vga_039 = 0xff0041ff,
                vga_040 = 0xff0000ff,
                vga_041 = 0xff4100ff,
                vga_042 = 0xff8200ff,
                vga_043 = 0xffbe00ff,
                vga_044 = 0xffff00ff,
                vga_045 = 0xbeff00ff,
                vga_046 = 0x82ff00ff,
                vga_047 = 0x41ff00ff,
                vga_048 = 0x00ff00ff,
                vga_049 = 0x00ff41ff,
                vga_050 = 0x00ff82ff,
                vga_051 = 0x00ffbeff,
                vga_052 = 0x00ffffff,
                vga_053 = 0x00beffff,
                vga_054 = 0x0082ffff,
                vga_055 = 0x0041ffff,
                vga_056 = 0x8282ffff,
                vga_057 = 0x9e82ffff,
                vga_058 = 0xbe82ffff,
                vga_059 = 0xdf82ffff,
                vga_060 = 0xff82ffff,
                vga_061 = 0xff82dfff,
                vga_062 = 0xff82beff,
                vga_063 = 0xff829eff,
                vga_064 = 0xff8282ff,
                vga_065 = 0xff9e82ff,
                vga_066 = 0xffbe82ff,
                vga_067 = 0xffdf82ff,
                vga_068 = 0xffff82ff,
                vga_069 = 0xdfff82ff,
                vga_070 = 0xbeff82ff,
                vga_071 = 0x9eff82ff,
                vga_072 = 0x82ff82ff,
                vga_073 = 0x82ff9eff,
                vga_074 = 0x82ffbeff,
                vga_075 = 0x82ffdfff,
                vga_076 = 0x82ffffff,
                vga_077 = 0x82dfffff,
                vga_078 = 0x82beffff,
                vga_079 = 0x829effff,
                vga_080 = 0xbabaffff,
                vga_081 = 0xcabaffff,
                vga_082 = 0xdfbaffff,
                vga_083 = 0xefbaffff,
                vga_084 = 0xffbaffff,
                vga_085 = 0xffbaefff,
                vga_086 = 0xffbadfff,
                vga_087 = 0xffbacaff,
                vga_088 = 0xffbabaff,
                vga_089 = 0xffcabaff,
                vga_090 = 0xffdfbaff,
                vga_091 = 0xffefbaff,
                vga_092 = 0xffffbaff,
                vga_093 = 0xefffbaff,
                vga_094 = 0xdfffbaff,
                vga_095 = 0xcaffbaff,
                vga_096 = 0xbaffbaff,
                vga_097 = 0xbaffcaff,
                vga_098 = 0xbaffdfff,
                vga_099 = 0xbaffefff,
                vga_100 = 0xbaffffff,
                vga_101 = 0xbaefffff,
                vga_102 = 0xbadfffff,
                vga_103 = 0xbacaffff,
                vga_104 = 0x000071ff,
                vga_105 = 0x1c0071ff,
                vga_106 = 0x390071ff,
                vga_107 = 0x550071ff,
                vga_108 = 0x710071ff,
                vga_109 = 0x710055ff,
                vga_110 = 0x710039ff,
                vga_111 = 0x71001cff,
                vga_112 = 0x710000ff,
                vga_113 = 0x711c00ff,
                vga_114 = 0x713900ff,
                vga_115 = 0x715500ff,
                vga_116 = 0x717100ff,
                vga_117 = 0x557100ff,
                vga_118 = 0x397100ff,
                vga_119 = 0x1c7100ff,
                vga_120 = 0x007100ff,
                vga_121 = 0x00711cff,
                vga_122 = 0x007139ff,
                vga_123 = 0x007155ff,
                vga_124 = 0x007171ff,
                vga_125 = 0x005571ff,
                vga_126 = 0x003971ff,
                vga_127 = 0x001c71ff,
                vga_128 = 0x393971ff,
                vga_129 = 0x453971ff,
                vga_130 = 0x553971ff,
                vga_131 = 0x613971ff,
                vga_132 = 0x713971ff,
                vga_133 = 0x713961ff,
                vga_134 = 0x713955ff,
                vga_135 = 0x713945ff,
                vga_136 = 0x713939ff,
                vga_137 = 0x714539ff,
                vga_138 = 0x715539ff,
                vga_139 = 0x716139ff,
                vga_140 = 0x717139ff,
                vga_141 = 0x617139ff,
                vga_142 = 0x557139ff,
                vga_143 = 0x457139ff,
                vga_144 = 0x397139ff,
                vga_145 = 0x397145ff,
                vga_146 = 0x397155ff,
                vga_147 = 0x397161ff,
                vga_148 = 0x397171ff,
                vga_149 = 0x396171ff,
                vga_150 = 0x395571ff,
                vga_151 = 0x394571ff,
                vga_152 = 0x515171ff,
                vga_153 = 0x595171ff,
                vga_154 = 0x615171ff,
                vga_155 = 0x695171ff,
                vga_156 = 0x715171ff,
                vga_157 = 0x715169ff,
                vga_158 = 0x715161ff,
                vga_159 = 0x715159ff,
                vga_160 = 0x715151ff,
                vga_161 = 0x715951ff,
                vga_162 = 0x716151ff,
                vga_163 = 0x716951ff,
                vga_164 = 0x717151ff,
                vga_165 = 0x697151ff,
                vga_166 = 0x617151ff,
                vga_167 = 0x597151ff,
                vga_168 = 0x517151ff,
                vga_169 = 0x517159ff,
                vga_170 = 0x517161ff,
                vga_171 = 0x517169ff,
                vga_172 = 0x517171ff,
                vga_173 = 0x516971ff,
                vga_174 = 0x516171ff,
                vga_175 = 0x515971ff,
                vga_176 = 0x000041ff,
                vga_177 = 0x100041ff,
                vga_178 = 0x200041ff,
                vga_179 = 0x310041ff,
                vga_180 = 0x410041ff,
                vga_181 = 0x410031ff,
                vga_182 = 0x410020ff,
                vga_183 = 0x410010ff,
                vga_184 = 0x410000ff,
                vga_185 = 0x411000ff,
                vga_186 = 0x412000ff,
                vga_187 = 0x413100ff,
                vga_188 = 0x414100ff,
                vga_189 = 0x314100ff,
                vga_190 = 0x204100ff,
                vga_191 = 0x104100ff,
                vga_192 = 0x004100ff,
                vga_193 = 0x004110ff,
                vga_194 = 0x004120ff,
                vga_195 = 0x004131ff,
                vga_196 = 0x004141ff,
                vga_197 = 0x003141ff,
                vga_198 = 0x002041ff,
                vga_199 = 0x001041ff,
                vga_200 = 0x202041ff,
                vga_201 = 0x282041ff,
                vga_202 = 0x312041ff,
                vga_203 = 0x392041ff,
                vga_204 = 0x412041ff,
                vga_205 = 0x412039ff,
                vga_206 = 0x412031ff,
                vga_207 = 0x412028ff,
                vga_208 = 0x412020ff,
                vga_209 = 0x412820ff,
                vga_210 = 0x413120ff,
                vga_211 = 0x413920ff,
                vga_212 = 0x414120ff,
                vga_213 = 0x394120ff,
                vga_214 = 0x314120ff,
                vga_215 = 0x284120ff,
                vga_216 = 0x204120ff,
                vga_217 = 0x204128ff,
                vga_218 = 0x204131ff,
                vga_219 = 0x204139ff,
                vga_220 = 0x204141ff,
                vga_221 = 0x203941ff,
                vga_222 = 0x203141ff,
                vga_223 = 0x202841ff,
                vga_224 = 0x2d2d41ff,
                vga_225 = 0x312d41ff,
                vga_226 = 0x352d41ff,
                vga_227 = 0x3d2d41ff,
                vga_228 = 0x412d41ff,
                vga_229 = 0x412d3dff,
                vga_230 = 0x412d35ff,
                vga_231 = 0x412d31ff,
                vga_232 = 0x412d2dff,
                vga_233 = 0x41312dff,
                vga_234 = 0x41352dff,
                vga_235 = 0x413d2dff,
                vga_236 = 0x41412dff,
                vga_237 = 0x3d412dff,
                vga_238 = 0x35412dff,
                vga_239 = 0x31412dff,
                vga_240 = 0x2d412dff,
                vga_241 = 0x2d4131ff,
                vga_242 = 0x2d4135ff,
                vga_243 = 0x2d413dff,
                vga_244 = 0x2d4141ff,
                vga_245 = 0x2d3d41ff,
                vga_246 = 0x2d3541ff,
                vga_247 = 0x2d3141ff,
                vga_248 = 0x000000ff,
                vga_249 = 0x000000ff,
                vga_250 = 0x000000ff,
                vga_251 = 0x000000ff,
                vga_252 = 0x000000ff,
                vga_253 = 0x000000ff,
                vga_254 = 0x000000ff,
                vga_255 = 0xffffffff,
                white = vga_255,
                Count = 256
            };

           static inline const std::vector<sf::Color> Colors{
                sf::Color(vga_000),
                sf::Color(vga_001),
                sf::Color(vga_002),
                sf::Color(vga_003),
                sf::Color(vga_004),
                sf::Color(vga_005),
                sf::Color(vga_006),
                sf::Color(vga_007),
                sf::Color(vga_008),
                sf::Color(vga_009),
                sf::Color(vga_010),
                sf::Color(vga_011),
                sf::Color(vga_012),
                sf::Color(vga_013),
                sf::Color(vga_014),
                sf::Color(vga_015),
                sf::Color(vga_016),
                sf::Color(vga_017),
                sf::Color(vga_018),
                sf::Color(vga_019),
                sf::Color(vga_020),
                sf::Color(vga_021),
                sf::Color(vga_022),
                sf::Color(vga_023),
                sf::Color(vga_024),
                sf::Color(vga_025),
                sf::Color(vga_026),
                sf::Color(vga_027),
                sf::Color(vga_028),
                sf::Color(vga_029),
                sf::Color(vga_030),
                sf::Color(vga_031),
                sf::Color(vga_032),
                sf::Color(vga_033),
                sf::Color(vga_034),
                sf::Color(vga_035),
                sf::Color(vga_036),
                sf::Color(vga_037),
                sf::Color(vga_038),
                sf::Color(vga_039),
                sf::Color(vga_040),
                sf::Color(vga_041),
                sf::Color(vga_042),
                sf::Color(vga_043),
                sf::Color(vga_044),
                sf::Color(vga_045),
                sf::Color(vga_046),
                sf::Color(vga_047),
                sf::Color(vga_048),
                sf::Color(vga_049),
                sf::Color(vga_050),
                sf::Color(vga_051),
                sf::Color(vga_052),
                sf::Color(vga_053),
                sf::Color(vga_054),
                sf::Color(vga_055),
                sf::Color(vga_056),
                sf::Color(vga_057),
                sf::Color(vga_058),
                sf::Color(vga_059),
                sf::Color(vga_060),
                sf::Color(vga_061),
                sf::Color(vga_062),
                sf::Color(vga_063),
                sf::Color(vga_064),
                sf::Color(vga_065),
                sf::Color(vga_066),
                sf::Color(vga_067),
                sf::Color(vga_068),
                sf::Color(vga_069),
                sf::Color(vga_070),
                sf::Color(vga_071),
                sf::Color(vga_072),
                sf::Color(vga_073),
                sf::Color(vga_074),
                sf::Color(vga_075),
                sf::Color(vga_076),
                sf::Color(vga_077),
                sf::Color(vga_078),
                sf::Color(vga_079),
                sf::Color(vga_080),
                sf::Color(vga_081),
                sf::Color(vga_082),
                sf::Color(vga_083),
                sf::Color(vga_084),
                sf::Color(vga_085),
                sf::Color(vga_086),
                sf::Color(vga_087),
                sf::Color(vga_088),
                sf::Color(vga_089),
                sf::Color(vga_090),
                sf::Color(vga_091),
                sf::Color(vga_092),
                sf::Color(vga_093),
                sf::Color(vga_094),
                sf::Color(vga_095),
                sf::Color(vga_096),
                sf::Color(vga_097),
                sf::Color(vga_098),
                sf::Color(vga_099),
                sf::Color(vga_100),
                sf::Color(vga_101),
                sf::Color(vga_102),
                sf::Color(vga_103),
                sf::Color(vga_104),
                sf::Color(vga_105),
                sf::Color(vga_106),
                sf::Color(vga_107),
                sf::Color(vga_108),
                sf::Color(vga_109),
                sf::Color(vga_110),
                sf::Color(vga_111),
                sf::Color(vga_112),
                sf::Color(vga_113),
                sf::Color(vga_114),
                sf::Color(vga_115),
                sf::Color(vga_116),
                sf::Color(vga_117),
                sf::Color(vga_118),
                sf::Color(vga_119),
                sf::Color(vga_120),
                sf::Color(vga_121),
                sf::Color(vga_122),
                sf::Color(vga_123),
                sf::Color(vga_124),
                sf::Color(vga_125),
                sf::Color(vga_126),
                sf::Color(vga_127),
                sf::Color(vga_128),
                sf::Color(vga_129),
                sf::Color(vga_130),
                sf::Color(vga_131),
                sf::Color(vga_132),
                sf::Color(vga_133),
                sf::Color(vga_134),
                sf::Color(vga_135),
                sf::Color(vga_136),
                sf::Color(vga_137),
                sf::Color(vga_138),
                sf::Color(vga_139),
                sf::Color(vga_140),
                sf::Color(vga_141),
                sf::Color(vga_142),
                sf::Color(vga_143),
                sf::Color(vga_144),
                sf::Color(vga_145),
                sf::Color(vga_146),
                sf::Color(vga_147),
                sf::Color(vga_148),
                sf::Color(vga_149),
                sf::Color(vga_150),
                sf::Color(vga_151),
                sf::Color(vga_152),
                sf::Color(vga_153),
                sf::Color(vga_154),
                sf::Color(vga_155),
                sf::Color(vga_156),
                sf::Color(vga_157),
                sf::Color(vga_158),
                sf::Color(vga_159),
                sf::Color(vga_160),
                sf::Color(vga_161),
                sf::Color(vga_162),
                sf::Color(vga_163),
                sf::Color(vga_164),
                sf::Color(vga_165),
                sf::Color(vga_166),
                sf::Color(vga_167),
                sf::Color(vga_168),
                sf::Color(vga_169),
                sf::Color(vga_170),
                sf::Color(vga_171),
                sf::Color(vga_172),
                sf::Color(vga_173),
                sf::Color(vga_174),
                sf::Color(vga_175),
                sf::Color(vga_176),
                sf::Color(vga_177),
                sf::Color(vga_178),
                sf::Color(vga_179),
                sf::Color(vga_180),
                sf::Color(vga_181),
                sf::Color(vga_182),
                sf::Color(vga_183),
                sf::Color(vga_184),
                sf::Color(vga_185),
                sf::Color(vga_186),
                sf::Color(vga_187),
                sf::Color(vga_188),
                sf::Color(vga_189),
                sf::Color(vga_190),
                sf::Color(vga_191),
                sf::Color(vga_192),
                sf::Color(vga_193),
                sf::Color(vga_194),
                sf::Color(vga_195),
                sf::Color(vga_196),
                sf::Color(vga_197),
                sf::Color(vga_198),
                sf::Color(vga_199),
                sf::Color(vga_200),
                sf::Color(vga_201),
                sf::Color(vga_202),
                sf::Color(vga_203),
                sf::Color(vga_204),
                sf::Color(vga_205),
                sf::Color(vga_206),
                sf::Color(vga_207),
                sf::Color(vga_208),
                sf::Color(vga_209),
                sf::Color(vga_210),
                sf::Color(vga_211),
                sf::Color(vga_212),
                sf::Color(vga_213),
                sf::Color(vga_214),
                sf::Color(vga_215),
                sf::Color(vga_216),
                sf::Color(vga_217),
                sf::Color(vga_218),
                sf::Color(vga_219),
                sf::Color(vga_220),
                sf::Color(vga_221),
                sf::Color(vga_222),
                sf::Color(vga_223),
                sf::Color(vga_224),
                sf::Color(vga_225),
                sf::Color(vga_226),
                sf::Color(vga_227),
                sf::Color(vga_228),
                sf::Color(vga_229),
                sf::Color(vga_230),
                sf::Color(vga_231),
                sf::Color(vga_232),
                sf::Color(vga_233),
                sf::Color(vga_234),
                sf::Color(vga_235),
                sf::Color(vga_236),
                sf::Color(vga_237),
                sf::Color(vga_238),
                sf::Color(vga_239),
                sf::Color(vga_240),
                sf::Color(vga_241),
                sf::Color(vga_242),
                sf::Color(vga_243),
                sf::Color(vga_244),
                sf::Color(vga_245),
                sf::Color(vga_246),
                sf::Color(vga_247),
                sf::Color(vga_248),
                sf::Color(vga_249),
                sf::Color(vga_250),
                sf::Color(vga_251),
                sf::Color(vga_252),
                sf::Color(vga_253),
                sf::Color(vga_254),
                sf::Color(vga_255)
            };
            // clang-format on

            static inline const std::string palleteName { "vga_256" };
        };

        // 16 colors from HTML3 (1986) -the only truly "web-safe" colors.
        struct websafe_16
        {
            // clang-format off
            enum Enum : sf::Uint32
            {
                black    = 0x000000ff,
                maroon   = 0x800000ff,
                green    = 0x008000ff,
                navy     = 0x000080ff,
                olive    = 0x808000ff,
                purple   = 0x800080ff,
                teal     = 0x008080ff,
                gray     = 0x808080ff,
                silver   = 0xc0c0c0ff,
                red      = 0xff0000ff,
                lime     = 0x00ff00ff,
                blue     = 0x0000ffff,
                aqua     = 0x00ffffff,
                yellow   = 0xffff00ff,
                fuchsia  = 0xff00ffff,
                white    = 0xffffffff,
                Count = 16
            };

           static inline const std::vector<sf::Color> Colors{
                sf::Color(black),
                sf::Color(maroon),
                sf::Color(green),
                sf::Color(navy),
                sf::Color(olive),
                sf::Color(purple),
                sf::Color(teal),
                sf::Color(gray),
                sf::Color(silver),
                sf::Color(red),
                sf::Color(lime),
                sf::Color(blue),
                sf::Color(aqua),
                sf::Color(yellow),
                sf::Color(fuchsia),
                sf::Color(white)
            };
            // clang-format on

            static inline const std::string palleteName { "websafe_16" };
        };

        // 216 (unnamed) colors that tried to be "web-safe" in the mid 1990's but failed miserably.
        // (all combinations of the 6x6x6 RGB triplet)
        struct websafe_216
        {
            // clang-format off
            enum Enum : sf::Uint32
            {
                black = 0x000000ff,
                ws_000 = black, // (0,0,0)
                ws_001 = 0x000033ff, // (0,0,51)
                ws_002 = 0x000066ff, // (0,0,102)
                ws_003 = 0x000099ff, // (0,0,153)
                ws_004 = 0x0000ccff, // (0,0,204)
                ws_005 = 0x0000ffff, // (0,0,255)
                ws_006 = 0x003300ff, // (0,51,0)
                ws_007 = 0x003333ff, // (0,51,51)
                ws_008 = 0x003366ff, // (0,51,102)
                ws_009 = 0x003399ff, // (0,51,153)
                ws_010 = 0x0033ccff, // (0,51,204)
                ws_011 = 0x0033ffff, // (0,51,255)
                ws_012 = 0x006600ff, // (0,102,0)
                ws_013 = 0x006633ff, // (0,102,51)
                ws_014 = 0x006666ff, // (0,102,102)
                ws_015 = 0x006699ff, // (0,102,153)
                ws_016 = 0x0066ccff, // (0,102,204)
                ws_017 = 0x0066ffff, // (0,102,255)
                ws_018 = 0x009900ff, // (0,153,0)
                ws_019 = 0x009933ff, // (0,153,51)
                ws_020 = 0x009966ff, // (0,153,102)
                ws_021 = 0x009999ff, // (0,153,153)
                ws_022 = 0x0099ccff, // (0,153,204)
                ws_023 = 0x0099ffff, // (0,153,255)
                ws_024 = 0x00cc00ff, // (0,204,0)
                ws_025 = 0x00cc33ff, // (0,204,51)
                ws_026 = 0x00cc66ff, // (0,204,102)
                ws_027 = 0x00cc99ff, // (0,204,153)
                ws_028 = 0x00ccccff, // (0,204,204)
                ws_029 = 0x00ccffff, // (0,204,255)
                ws_030 = 0x00ff00ff, // (0,255,0)
                ws_031 = 0x00ff33ff, // (0,255,51)
                ws_032 = 0x00ff66ff, // (0,255,102)
                ws_033 = 0x00ff99ff, // (0,255,153)
                ws_034 = 0x00ffccff, // (0,255,204)
                ws_035 = 0x00ffffff, // (0,255,255)
                ws_036 = 0x330000ff, // (51,0,0)
                ws_037 = 0x330033ff, // (51,0,51)
                ws_038 = 0x330066ff, // (51,0,102)
                ws_039 = 0x330099ff, // (51,0,153)
                ws_040 = 0x3300ccff, // (51,0,204)
                ws_041 = 0x3300ffff, // (51,0,255)
                ws_042 = 0x333300ff, // (51,51,0)
                ws_043 = 0x333333ff, // (51,51,51)
                ws_044 = 0x333366ff, // (51,51,102)
                ws_045 = 0x333399ff, // (51,51,153)
                ws_046 = 0x3333ccff, // (51,51,204)
                ws_047 = 0x3333ffff, // (51,51,255)
                ws_048 = 0x336600ff, // (51,102,0)
                ws_049 = 0x336633ff, // (51,102,51)
                ws_050 = 0x336666ff, // (51,102,102)
                ws_051 = 0x336699ff, // (51,102,153)
                ws_052 = 0x3366ccff, // (51,102,204)
                ws_053 = 0x3366ffff, // (51,102,255)
                ws_054 = 0x339900ff, // (51,153,0)
                ws_055 = 0x339933ff, // (51,153,51)
                ws_056 = 0x339966ff, // (51,153,102)
                ws_057 = 0x339999ff, // (51,153,153)
                ws_058 = 0x3399ccff, // (51,153,204)
                ws_059 = 0x3399ffff, // (51,153,255)
                ws_060 = 0x33cc00ff, // (51,204,0)
                ws_061 = 0x33cc33ff, // (51,204,51)
                ws_062 = 0x33cc66ff, // (51,204,102)
                ws_063 = 0x33cc99ff, // (51,204,153)
                ws_064 = 0x33ccccff, // (51,204,204)
                ws_065 = 0x33ccffff, // (51,204,255)
                ws_066 = 0x33ff00ff, // (51,255,0)
                ws_067 = 0x33ff33ff, // (51,255,51)
                ws_068 = 0x33ff66ff, // (51,255,102)
                ws_069 = 0x33ff99ff, // (51,255,153)
                ws_070 = 0x33ffccff, // (51,255,204)
                ws_071 = 0x33ffffff, // (51,255,255)
                ws_072 = 0x660000ff, // (102,0,0)
                ws_073 = 0x660033ff, // (102,0,51)
                ws_074 = 0x660066ff, // (102,0,102)
                ws_075 = 0x660099ff, // (102,0,153)
                ws_076 = 0x6600ccff, // (102,0,204)
                ws_077 = 0x6600ffff, // (102,0,255)
                ws_078 = 0x663300ff, // (102,51,0)
                ws_079 = 0x663333ff, // (102,51,51)
                ws_080 = 0x663366ff, // (102,51,102)
                ws_081 = 0x663399ff, // (102,51,153)
                ws_082 = 0x6633ccff, // (102,51,204)
                ws_083 = 0x6633ffff, // (102,51,255)
                ws_084 = 0x666600ff, // (102,102,0)
                ws_085 = 0x666633ff, // (102,102,51)
                ws_086 = 0x666666ff, // (102,102,102)
                ws_087 = 0x666699ff, // (102,102,153)
                ws_088 = 0x6666ccff, // (102,102,204)
                ws_089 = 0x6666ffff, // (102,102,255)
                ws_090 = 0x669900ff, // (102,153,0)
                ws_091 = 0x669933ff, // (102,153,51)
                ws_092 = 0x669966ff, // (102,153,102)
                ws_093 = 0x669999ff, // (102,153,153)
                ws_094 = 0x6699ccff, // (102,153,204)
                ws_095 = 0x6699ffff, // (102,153,255)
                ws_096 = 0x66cc00ff, // (102,204,0)
                ws_097 = 0x66cc33ff, // (102,204,51)
                ws_098 = 0x66cc66ff, // (102,204,102)
                ws_099 = 0x66cc99ff, // (102,204,153)
                ws_100 = 0x66ccccff, // (102,204,204)
                ws_101 = 0x66ccffff, // (102,204,255)
                ws_102 = 0x66ff00ff, // (102,255,0)
                ws_103 = 0x66ff33ff, // (102,255,51)
                ws_104 = 0x66ff66ff, // (102,255,102)
                ws_105 = 0x66ff99ff, // (102,255,153)
                ws_106 = 0x66ffccff, // (102,255,204)
                ws_107 = 0x66ffffff, // (102,255,255)
                ws_108 = 0x990000ff, // (153,0,0)
                ws_109 = 0x990033ff, // (153,0,51)
                ws_110 = 0x990066ff, // (153,0,102)
                ws_111 = 0x990099ff, // (153,0,153)
                ws_112 = 0x9900ccff, // (153,0,204)
                ws_113 = 0x9900ffff, // (153,0,255)
                ws_114 = 0x993300ff, // (153,51,0)
                ws_115 = 0x993333ff, // (153,51,51)
                ws_116 = 0x993366ff, // (153,51,102)
                ws_117 = 0x993399ff, // (153,51,153)
                ws_118 = 0x9933ccff, // (153,51,204)
                ws_119 = 0x9933ffff, // (153,51,255)
                ws_120 = 0x996600ff, // (153,102,0)
                ws_121 = 0x996633ff, // (153,102,51)
                ws_122 = 0x996666ff, // (153,102,102)
                ws_123 = 0x996699ff, // (153,102,153)
                ws_124 = 0x9966ccff, // (153,102,204)
                ws_125 = 0x9966ffff, // (153,102,255)
                ws_126 = 0x999900ff, // (153,153,0)
                ws_127 = 0x999933ff, // (153,153,51)
                ws_128 = 0x999966ff, // (153,153,102)
                ws_129 = 0x999999ff, // (153,153,153)
                ws_130 = 0x9999ccff, // (153,153,204)
                ws_131 = 0x9999ffff, // (153,153,255)
                ws_132 = 0x99cc00ff, // (153,204,0)
                ws_133 = 0x99cc33ff, // (153,204,51)
                ws_134 = 0x99cc66ff, // (153,204,102)
                ws_135 = 0x99cc99ff, // (153,204,153)
                ws_136 = 0x99ccccff, // (153,204,204)
                ws_137 = 0x99ccffff, // (153,204,255)
                ws_138 = 0x99ff00ff, // (153,255,0)
                ws_139 = 0x99ff33ff, // (153,255,51)
                ws_140 = 0x99ff66ff, // (153,255,102)
                ws_141 = 0x99ff99ff, // (153,255,153)
                ws_142 = 0x99ffccff, // (153,255,204)
                ws_143 = 0x99ffffff, // (153,255,255)
                ws_144 = 0xcc0000ff, // (204,0,0)
                ws_145 = 0xcc0033ff, // (204,0,51)
                ws_146 = 0xcc0066ff, // (204,0,102)
                ws_147 = 0xcc0099ff, // (204,0,153)
                ws_148 = 0xcc00ccff, // (204,0,204)
                ws_149 = 0xcc00ffff, // (204,0,255)
                ws_150 = 0xcc3300ff, // (204,51,0)
                ws_151 = 0xcc3333ff, // (204,51,51)
                ws_152 = 0xcc3366ff, // (204,51,102)
                ws_153 = 0xcc3399ff, // (204,51,153)
                ws_154 = 0xcc33ccff, // (204,51,204)
                ws_155 = 0xcc33ffff, // (204,51,255)
                ws_156 = 0xcc6600ff, // (204,102,0)
                ws_157 = 0xcc6633ff, // (204,102,51)
                ws_158 = 0xcc6666ff, // (204,102,102)
                ws_159 = 0xcc6699ff, // (204,102,153)
                ws_160 = 0xcc66ccff, // (204,102,204)
                ws_161 = 0xcc66ffff, // (204,102,255)
                ws_162 = 0xcc9900ff, // (204,153,0)
                ws_163 = 0xcc9933ff, // (204,153,51)
                ws_164 = 0xcc9966ff, // (204,153,102)
                ws_165 = 0xcc9999ff, // (204,153,153)
                ws_166 = 0xcc99ccff, // (204,153,204)
                ws_167 = 0xcc99ffff, // (204,153,255)
                ws_168 = 0xcccc00ff, // (204,204,0)
                ws_169 = 0xcccc33ff, // (204,204,51)
                ws_170 = 0xcccc66ff, // (204,204,102)
                ws_171 = 0xcccc99ff, // (204,204,153)
                ws_172 = 0xccccccff, // (204,204,204)
                ws_173 = 0xccccffff, // (204,204,255)
                ws_174 = 0xccff00ff, // (204,255,0)
                ws_175 = 0xccff33ff, // (204,255,51)
                ws_176 = 0xccff66ff, // (204,255,102)
                ws_177 = 0xccff99ff, // (204,255,153)
                ws_178 = 0xccffccff, // (204,255,204)
                ws_179 = 0xccffffff, // (204,255,255)
                ws_180 = 0xff0000ff, // (255,0,0)
                ws_181 = 0xff0033ff, // (255,0,51)
                ws_182 = 0xff0066ff, // (255,0,102)
                ws_183 = 0xff0099ff, // (255,0,153)
                ws_184 = 0xff00ccff, // (255,0,204)
                ws_185 = 0xff00ffff, // (255,0,255)
                ws_186 = 0xff3300ff, // (255,51,0)
                ws_187 = 0xff3333ff, // (255,51,51)
                ws_188 = 0xff3366ff, // (255,51,102)
                ws_189 = 0xff3399ff, // (255,51,153)
                ws_190 = 0xff33ccff, // (255,51,204)
                ws_191 = 0xff33ffff, // (255,51,255)
                ws_192 = 0xff6600ff, // (255,102,0)
                ws_193 = 0xff6633ff, // (255,102,51)
                ws_194 = 0xff6666ff, // (255,102,102)
                ws_195 = 0xff6699ff, // (255,102,153)
                ws_196 = 0xff66ccff, // (255,102,204)
                ws_197 = 0xff66ffff, // (255,102,255)
                ws_198 = 0xff9900ff, // (255,153,0)
                ws_199 = 0xff9933ff, // (255,153,51)
                ws_200 = 0xff9966ff, // (255,153,102)
                ws_201 = 0xff9999ff, // (255,153,153)
                ws_202 = 0xff99ccff, // (255,153,204)
                ws_203 = 0xff99ffff, // (255,153,255)
                ws_204 = 0xffcc00ff, // (255,204,0)
                ws_205 = 0xffcc33ff, // (255,204,51)
                ws_206 = 0xffcc66ff, // (255,204,102)
                ws_207 = 0xffcc99ff, // (255,204,153)
                ws_208 = 0xffccccff, // (255,204,204)
                ws_209 = 0xffccffff, // (255,204,255)
                ws_210 = 0xffff00ff, // (255,255,0)
                ws_211 = 0xffff33ff, // (255,255,51)
                ws_212 = 0xffff66ff, // (255,255,102)
                ws_213 = 0xffff99ff, // (255,255,153)
                ws_214 = 0xffffccff, // (255,255,204)
                ws_215 = 0xffffffff, // (255,255,255)
                white = ws_215,
                Count = 216
            };

           static inline const std::vector<sf::Color> Colors{
                sf::Color(ws_000),
                sf::Color(ws_001),
                sf::Color(ws_002),
                sf::Color(ws_003),
                sf::Color(ws_004),
                sf::Color(ws_005),
                sf::Color(ws_006),
                sf::Color(ws_007),
                sf::Color(ws_008),
                sf::Color(ws_009),
                sf::Color(ws_010),
                sf::Color(ws_011),
                sf::Color(ws_012),
                sf::Color(ws_013),
                sf::Color(ws_014),
                sf::Color(ws_015),
                sf::Color(ws_016),
                sf::Color(ws_017),
                sf::Color(ws_018),
                sf::Color(ws_019),
                sf::Color(ws_020),
                sf::Color(ws_021),
                sf::Color(ws_022),
                sf::Color(ws_023),
                sf::Color(ws_024),
                sf::Color(ws_025),
                sf::Color(ws_026),
                sf::Color(ws_027),
                sf::Color(ws_028),
                sf::Color(ws_029),
                sf::Color(ws_030),
                sf::Color(ws_031),
                sf::Color(ws_032),
                sf::Color(ws_033),
                sf::Color(ws_034),
                sf::Color(ws_035),
                sf::Color(ws_036),
                sf::Color(ws_037),
                sf::Color(ws_038),
                sf::Color(ws_039),
                sf::Color(ws_040),
                sf::Color(ws_041),
                sf::Color(ws_042),
                sf::Color(ws_043),
                sf::Color(ws_044),
                sf::Color(ws_045),
                sf::Color(ws_046),
                sf::Color(ws_047),
                sf::Color(ws_048),
                sf::Color(ws_049),
                sf::Color(ws_050),
                sf::Color(ws_051),
                sf::Color(ws_052),
                sf::Color(ws_053),
                sf::Color(ws_054),
                sf::Color(ws_055),
                sf::Color(ws_056),
                sf::Color(ws_057),
                sf::Color(ws_058),
                sf::Color(ws_059),
                sf::Color(ws_060),
                sf::Color(ws_061),
                sf::Color(ws_062),
                sf::Color(ws_063),
                sf::Color(ws_064),
                sf::Color(ws_065),
                sf::Color(ws_066),
                sf::Color(ws_067),
                sf::Color(ws_068),
                sf::Color(ws_069),
                sf::Color(ws_070),
                sf::Color(ws_071),
                sf::Color(ws_072),
                sf::Color(ws_073),
                sf::Color(ws_074),
                sf::Color(ws_075),
                sf::Color(ws_076),
                sf::Color(ws_077),
                sf::Color(ws_078),
                sf::Color(ws_079),
                sf::Color(ws_080),
                sf::Color(ws_081),
                sf::Color(ws_082),
                sf::Color(ws_083),
                sf::Color(ws_084),
                sf::Color(ws_085),
                sf::Color(ws_086),
                sf::Color(ws_087),
                sf::Color(ws_088),
                sf::Color(ws_089),
                sf::Color(ws_090),
                sf::Color(ws_091),
                sf::Color(ws_092),
                sf::Color(ws_093),
                sf::Color(ws_094),
                sf::Color(ws_095),
                sf::Color(ws_096),
                sf::Color(ws_097),
                sf::Color(ws_098),
                sf::Color(ws_099),
                sf::Color(ws_100),
                sf::Color(ws_101),
                sf::Color(ws_102),
                sf::Color(ws_103),
                sf::Color(ws_104),
                sf::Color(ws_105),
                sf::Color(ws_106),
                sf::Color(ws_107),
                sf::Color(ws_108),
                sf::Color(ws_109),
                sf::Color(ws_110),
                sf::Color(ws_111),
                sf::Color(ws_112),
                sf::Color(ws_113),
                sf::Color(ws_114),
                sf::Color(ws_115),
                sf::Color(ws_116),
                sf::Color(ws_117),
                sf::Color(ws_118),
                sf::Color(ws_119),
                sf::Color(ws_120),
                sf::Color(ws_121),
                sf::Color(ws_122),
                sf::Color(ws_123),
                sf::Color(ws_124),
                sf::Color(ws_125),
                sf::Color(ws_126),
                sf::Color(ws_127),
                sf::Color(ws_128),
                sf::Color(ws_129),
                sf::Color(ws_130),
                sf::Color(ws_131),
                sf::Color(ws_132),
                sf::Color(ws_133),
                sf::Color(ws_134),
                sf::Color(ws_135),
                sf::Color(ws_136),
                sf::Color(ws_137),
                sf::Color(ws_138),
                sf::Color(ws_139),
                sf::Color(ws_140),
                sf::Color(ws_141),
                sf::Color(ws_142),
                sf::Color(ws_143),
                sf::Color(ws_144),
                sf::Color(ws_145),
                sf::Color(ws_146),
                sf::Color(ws_147),
                sf::Color(ws_148),
                sf::Color(ws_149),
                sf::Color(ws_150),
                sf::Color(ws_151),
                sf::Color(ws_152),
                sf::Color(ws_153),
                sf::Color(ws_154),
                sf::Color(ws_155),
                sf::Color(ws_156),
                sf::Color(ws_157),
                sf::Color(ws_158),
                sf::Color(ws_159),
                sf::Color(ws_160),
                sf::Color(ws_161),
                sf::Color(ws_162),
                sf::Color(ws_163),
                sf::Color(ws_164),
                sf::Color(ws_165),
                sf::Color(ws_166),
                sf::Color(ws_167),
                sf::Color(ws_168),
                sf::Color(ws_169),
                sf::Color(ws_170),
                sf::Color(ws_171),
                sf::Color(ws_172),
                sf::Color(ws_173),
                sf::Color(ws_174),
                sf::Color(ws_175),
                sf::Color(ws_176),
                sf::Color(ws_177),
                sf::Color(ws_178),
                sf::Color(ws_179),
                sf::Color(ws_180),
                sf::Color(ws_181),
                sf::Color(ws_182),
                sf::Color(ws_183),
                sf::Color(ws_184),
                sf::Color(ws_185),
                sf::Color(ws_186),
                sf::Color(ws_187),
                sf::Color(ws_188),
                sf::Color(ws_189),
                sf::Color(ws_190),
                sf::Color(ws_191),
                sf::Color(ws_192),
                sf::Color(ws_193),
                sf::Color(ws_194),
                sf::Color(ws_195),
                sf::Color(ws_196),
                sf::Color(ws_197),
                sf::Color(ws_198),
                sf::Color(ws_199),
                sf::Color(ws_200),
                sf::Color(ws_201),
                sf::Color(ws_202),
                sf::Color(ws_203),
                sf::Color(ws_204),
                sf::Color(ws_205),
                sf::Color(ws_206),
                sf::Color(ws_207),
                sf::Color(ws_208),
                sf::Color(ws_209),
                sf::Color(ws_210),
                sf::Color(ws_211),
                sf::Color(ws_212),
                sf::Color(ws_213),
                sf::Color(ws_214),
                sf::Color(ws_215)
            };
            // clang-format on

            static inline const std::string palleteName { "websafe_216" };
        };

        // The W3C "web-safe" colors (flawed and abandoned, but the names stuck...)
        // This is the most modern of the websafe pallets.
        struct websafe_139
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
                Count = 139
            };

           static inline const std::vector<sf::Color> Colors{
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
            // clang-format on

            static inline const std::string palleteName { "websafe_139" };
        };

        // The X11 colors are a constant changing mess, but you can find the current set here:
        //      gitlab.freedesktop.org/xorg/app/rgb/raw/master/rgb.txt
        //  Instead of using the list of the day, or some custom minimal list, I opted to create
        //  a set that included every color from every version I could find.
        struct x11_543
        {
            // clang-format off
            enum Enum : sf::Uint32
            {
                black                 = 0x000000ff, // (  0,  0,  0)
                grey0                 = 0x000000ff, // (  0,  0,  0)
                navy                  = 0x000080ff, // (  0,  0,128)
                blue4                 = 0x00008bff, // (  0,  0,139)
                darkblue              = 0x00008bff, // (  0,  0,139)
                blue3                 = 0x0000cdff, // (  0,  0,205)
                mediumblue            = 0x0000cdff, // (  0,  0,205)
                blue2                 = 0x0000eeff, // (  0,  0,238)
                blue                  = 0x0000ffff, // (  0,  0,255)
                darkgreen             = 0x005300ff, // (  0, 83,  0)
                darkgreen1            = 0x006400ff, // (  0,100,  0)
                deepskyblue4          = 0x00688bff, // (  0,104,139)
                turquoise4            = 0x00868bff, // (  0,134,139)
                green4                = 0x008b00ff, // (  0,139,  0)
                springgreen4          = 0x008b45ff, // (  0,139, 69)
                cyan4                 = 0x008b8bff, // (  0,139,139)
                darkcyan              = 0x008b8bff, // (  0,139,139)
                deepskyblue3          = 0x009acdff, // (  0,154,205)
                deepskyblue2          = 0x00b2eeff, // (  0,178,238)
                deepskyblue           = 0x00bfffff, // (  0,191,255)
                darkturquoise1        = 0x00c3cdff, // (  0,195,205)
                turquoise3            = 0x00c5cdff, // (  0,197,205)
                green3                = 0x00cd00ff, // (  0,205,  0)
                springgreen3          = 0x00cd66ff, // (  0,205,102)
                cyan3                 = 0x00cdcdff, // (  0,205,205)
                darkturquoise         = 0x00ced1ff, // (  0,206,209)
                mediumturquoise2      = 0x00e3eeff, // (  0,227,238)
                turquoise2            = 0x00e5eeff, // (  0,229,238)
                green2                = 0x00ee00ff, // (  0,238,  0)
                springgreen2          = 0x00ee76ff, // (  0,238,118)
                cyan2                 = 0x00eeeeff, // (  0,238,238)
                turquoise1            = 0x00f5ffff, // (  0,245,255)
                mediumspringgreen     = 0x00fa9aff, // (  0,250,154)
                green                 = 0x00ff00ff, // (  0,255,  0)
                springgreen           = 0x00ff7fff, // (  0,255,127)
                cyan                  = 0x00ffffff, // (  0,255,255)
                grey1                 = 0x030303ff, // (  3,  3,  3)
                grey2                 = 0x050505ff, // (  5,  5,  5)
                grey3                 = 0x080808ff, // (  8,  8,  8)
                grey4                 = 0x0a0a0aff, // ( 10, 10, 10)
                grey5                 = 0x0d0d0dff, // ( 13, 13, 13)
                grey6                 = 0x0f0f0fff, // ( 15, 15, 15)
                dodgerblue4           = 0x104e8bff, // ( 16, 78,139)
                grey7                 = 0x121212ff, // ( 18, 18, 18)
                grey8                 = 0x141414ff, // ( 20, 20, 20)
                grey9                 = 0x171717ff, // ( 23, 23, 23)
                dodgerblue3           = 0x1874cdff, // ( 24,116,205)
                midnightblue1         = 0x191964ff, // ( 25, 25,100)
                midnightblue          = 0x191970ff, // ( 25, 25,112)
                grey10                = 0x1a1a1aff, // ( 26, 26, 26)
                grey11                = 0x1c1c1cff, // ( 28, 28, 28)
                dodgerblue2           = 0x1c86eeff, // ( 28,134,238)
                dodgerblue            = 0x1e90ffff, // ( 30,144,255)
                grey12                = 0x1f1f1fff, // ( 31, 31, 31)
                lightseagreen         = 0x20b2aaff, // ( 32,178,170)
                grey13                = 0x212121ff, // ( 33, 33, 33)
                cornflowerblue        = 0x222298ff, // ( 34, 34,152)
                forestgreen           = 0x228b22ff, // ( 34,139, 34)
                grey14                = 0x242424ff, // ( 36, 36, 36)
                grey15                = 0x262626ff, // ( 38, 38, 38)
                royalblue4            = 0x27408bff, // ( 39, 64,139)
                grey16                = 0x292929ff, // ( 41, 41, 41)
                grey17                = 0x2b2b2bff, // ( 43, 43, 43)
                grey18                = 0x2e2e2eff, // ( 46, 46, 46)
                seagreen              = 0x2e8b57ff, // ( 46,139, 87)
                darkslategrey         = 0x2f4f4fff, // ( 47, 79, 79)
                grey19                = 0x303030ff, // ( 48, 48, 48)
                limegreen             = 0x32cd32ff, // ( 50,205, 50)
                grey20                = 0x333333ff, // ( 51, 51, 51)
                grey21                = 0x363636ff, // ( 54, 54, 54)
                steelblue4            = 0x36648bff, // ( 54,100,139)
                grey22                = 0x383838ff, // ( 56, 56, 56)
                royalblue3            = 0x3a5fcdff, // ( 58, 95,205)
                grey23                = 0x3b3b3bff, // ( 59, 59, 59)
                mediumseagreen        = 0x3cb371ff, // ( 60,179,113)
                grey24                = 0x3d3d3dff, // ( 61, 61, 61)
                grey25                = 0x404040ff, // ( 64, 64, 64)
                turquoise             = 0x40e0d0ff, // ( 64,224,208)
                royalblue             = 0x4169e1ff, // ( 65,105,225)
                grey26                = 0x424242ff, // ( 66, 66, 66)
                royalblue2            = 0x436eeeff, // ( 67,110,238)
                seagreen3             = 0x43cd80ff, // ( 67,205,128)
                grey27                = 0x454545ff, // ( 69, 69, 69)
                chartreuse4           = 0x458b00ff, // ( 69,139,  0)
                aquamarine4           = 0x458b74ff, // ( 69,139,116)
                steelblue             = 0x4682b4ff, // ( 70,130,180)
                slateblue4            = 0x473c8bff, // ( 71, 60,139)
                grey28                = 0x474747ff, // ( 71, 71, 71)
                darkslateblue         = 0x483d8bff, // ( 72, 61,139)
                royalblue1            = 0x4876ffff, // ( 72,118,255)
                mediumturquoise       = 0x48d1ccff, // ( 72,209,204)
                grey29                = 0x4a4a4aff, // ( 74, 74, 74)
                skyblue4              = 0x4a708bff, // ( 74,112,139)
                grey30                = 0x4d4d4dff, // ( 77, 77, 77)
                seagreen2             = 0x4eee94ff, // ( 78,238,148)
                grey31                = 0x4f4f4fff, // ( 79, 79, 79)
                steelblue3            = 0x4f94cdff, // ( 79,148,205)
                grey32                = 0x525252ff, // ( 82, 82, 82)
                darkslategrey4        = 0x528b8bff, // ( 82,139,139)
                cadetblue4            = 0x53868bff, // ( 83,134,139)
                grey33                = 0x545454ff, // ( 84, 84, 84)
                palegreen4            = 0x548b54ff, // ( 84,139, 84)
                seagreen1             = 0x54ff9fff, // ( 84,255,159)
                purple4               = 0x551a8bff, // ( 85, 26,139)
                darkolivegreen        = 0x556b2fff, // ( 85,107, 47)
                grey34                = 0x575757ff, // ( 87, 87, 87)
                grey35                = 0x595959ff, // ( 89, 89, 89)
                grey36                = 0x5c5c5cff, // ( 92, 92, 92)
                steelblue2            = 0x5caceeff, // ( 92,172,238)
                mediumpurple4         = 0x5d478bff, // ( 93, 71,139)
                grey37                = 0x5e5e5eff, // ( 94, 94, 94)
                cadetblue             = 0x5f9ea0ff, // ( 95,158,160)
                lightskyblue4         = 0x607b8bff, // ( 96,123,139)
                grey38                = 0x616161ff, // ( 97, 97, 97)
                grey39                = 0x636363ff, // ( 99, 99, 99)
                steelblue1            = 0x63b8ffff, // ( 99,184,255)
                cornflowerblue1       = 0x6495edff, // (100,149,237)
                grey40                = 0x666666ff, // (102,102,102)
                paleturquoise4        = 0x668b8bff, // (102,139,139)
                chartreuse3           = 0x66cd00ff, // (102,205,  0)
                aquamarine3           = 0x66cdaaff, // (102,205,170)
                mediumaquamarine      = 0x66cdaaff, // (102,205,170)
                darkorchid4           = 0x68228bff, // (104, 34,139)
                lightblue4            = 0x68838bff, // (104,131,139)
                slateblue3            = 0x6959cdff, // (105, 89,205)
                dimgrey               = 0x696969ff, // (105,105,105)
                grey41                = 0x696969ff, // (105,105,105)
                olivedrab4            = 0x698b22ff, // (105,139, 34)
                darkseagreen4         = 0x698b69ff, // (105,139,105)
                slateblue             = 0x6a5acdff, // (106, 90,205)
                grey42                = 0x6b6b6bff, // (107,107,107)
                mediumforestgreen     = 0x6b8e23ff, // (107,142, 35)
                olivedrab             = 0x6b8e23ff, // (107,142, 35)
                slategrey4            = 0x6c7b8bff, // (108,123,139)
                skyblue3              = 0x6ca6cdff, // (108,166,205)
                grey43                = 0x6e6e6eff, // (110,110,110)
                lightsteelblue4       = 0x6e7b8bff, // (110,123,139)
                darkolivegreen4       = 0x6e8b3dff, // (110,139, 61)
                grey44                = 0x707070ff, // (112,112,112)
                slategrey             = 0x708090ff, // (112,128,144)
                grey45                = 0x737373ff, // (115,115,115)
                grey46                = 0x757575ff, // (117,117,117)
                chartreuse2           = 0x76ee00ff, // (118,238,  0)
                aquamarine2           = 0x76eec6ff, // (118,238,198)
                lightslategrey        = 0x778899ff, // (119,136,153)
                grey47                = 0x787878ff, // (120,120,120)
                darkslategrey3        = 0x79cdcdff, // (121,205,205)
                mediumorchid4         = 0x7a378bff, // (122, 55,139)
                slateblue2            = 0x7a67eeff, // (122,103,238)
                grey48                = 0x7a7a7aff, // (122,122,122)
                lightcyan4            = 0x7a8b8bff, // (122,139,139)
                cadetblue3            = 0x7ac5cdff, // (122,197,205)
                mediumslateblue       = 0x7b68eeff, // (123,104,238)
                palegreen3            = 0x7ccd7cff, // (124,205,124)
                lawngreen             = 0x7cfc00ff, // (124,252,  0)
                purple3               = 0x7d26cdff, // (125, 38,205)
                grey49                = 0x7d7d7dff, // (125,125,125)
                skyblue2              = 0x7ec0eeff, // (126,192,238)
                grey50                = 0x7f7f7fff, // (127,127,127)
                chartreuse            = 0x7fff00ff, // (127,255,  0)
                mediumspringgreen2    = 0x7fff00ff, // (127,255,  0)
                aquamarine            = 0x7fffd4ff, // (127,255,212)
                grey51                = 0x828282ff, // (130,130,130)
                slateblue1            = 0x836fffff, // (131,111,255)
                honeydew4             = 0x838b83ff, // (131,139,131)
                azure4                = 0x838b8bff, // (131,139,139)
                lightslateblue        = 0x8470ffff, // (132,112,255)
                grey52                = 0x858585ff, // (133,133,133)
                grey53                = 0x878787ff, // (135,135,135)
                skyblue               = 0x87ceebff, // (135,206,235)
                lightskyblue          = 0x87cefaff, // (135,206,250)
                skyblue1              = 0x87ceffff, // (135,206,255)
                mediumpurple3         = 0x8968cdff, // (137,104,205)
                blueviolet            = 0x8a2be2ff, // (138, 43,226)
                purple5               = 0x8a2be2ff, // (138, 43,226)
                grey54                = 0x8a8a8aff, // (138,138,138)
                darkred               = 0x8b0000ff, // (139,  0,  0)
                red4                  = 0x8b0000ff, // (139,  0,  0)
                darkmagenta           = 0x8b008bff, // (139,  0,139)
                magenta4              = 0x8b008bff, // (139,  0,139)
                deeppink4             = 0x8b0a50ff, // (139, 10, 80)
                firebrick4            = 0x8b1a1aff, // (139, 26, 26)
                maroon4               = 0x8b1c62ff, // (139, 28, 98)
                violetred4            = 0x8b2252ff, // (139, 34, 82)
                brown4                = 0x8b2323ff, // (139, 35, 35)
                orangered4            = 0x8b2500ff, // (139, 37,  0)
                tomato4               = 0x8b3626ff, // (139, 54, 38)
                hotpink4              = 0x8b3a62ff, // (139, 58, 98)
                coral4                = 0x8b3e2fff, // (139, 62, 47)
                mediumpink4           = 0x8b4462ff, // (139, 68, 98)
                darkorange4           = 0x8b4500ff, // (139, 69,  0)
                chocolate4            = 0x8b4513ff, // (139, 69, 19)
                saddlebrown           = 0x8b4513ff, // (139, 69, 19)
                sienna4               = 0x8b4726ff, // (139, 71, 38)
                palevioletred4        = 0x8b475dff, // (139, 71, 93)
                orchid4               = 0x8b4789ff, // (139, 71,137)
                salmon4               = 0x8b4c39ff, // (139, 76, 57)
                lightsalmon4          = 0x8b5742ff, // (139, 87, 66)
                orange4               = 0x8b5a00ff, // (139, 90,  0)
                tan4                  = 0x8b5a2bff, // (139, 90, 43)
                lightpink4            = 0x8b5f65ff, // (139, 95,101)
                pink4                 = 0x8b636cff, // (139, 99,108)
                darkgoldenrod4        = 0x8b6508ff, // (139,101,  8)
                mediumgoldenrod4      = 0x8b6508ff, // (139,101,  8)
                plum4                 = 0x8b668bff, // (139,102,139)
                goldenrod4            = 0x8b6914ff, // (139,105, 20)
                rosybrown4            = 0x8b6969ff, // (139,105,105)
                burlywood4            = 0x8b7355ff, // (139,115, 85)
                gold4                 = 0x8b7500ff, // (139,117,  0)
                peachpuff4            = 0x8b7765ff, // (139,119,101)
                navajowhite4          = 0x8b795eff, // (139,121, 94)
                thistle4              = 0x8b7b8bff, // (139,123,139)
                bisque4               = 0x8b7d6bff, // (139,125,107)
                mistyrose4            = 0x8b7d7bff, // (139,125,123)
                wheat4                = 0x8b7e66ff, // (139,126,102)
                lightgoldenrod4       = 0x8b814cff, // (139,129, 76)
                oldgoldenrod4         = 0x8b814cff, // (139,129, 76)
                antiquewhite4         = 0x8b8378ff, // (139,131,120)
                lavenderblush4        = 0x8b8386ff, // (139,131,134)
                khaki4                = 0x8b864eff, // (139,134, 78)
                seashell4             = 0x8b8682ff, // (139,134,130)
                cornsilk4             = 0x8b8878ff, // (139,136,120)
                lemonchiffon4         = 0x8b8970ff, // (139,137,112)
                snow4                 = 0x8b8989ff, // (139,137,137)
                yellow4               = 0x8b8b00ff, // (139,139,  0)
                oldmediumgoldenrod4   = 0x8b8b66ff, // (139,139,102)
                lightyellow4          = 0x8b8b7aff, // (139,139,122)
                ivory4                = 0x8b8b83ff, // (139,139,131)
                grey55                = 0x8c8c8cff, // (140,140,140)
                lightskyblue3         = 0x8db6cdff, // (141,182,205)
                darkslategrey2        = 0x8deeeeff, // (141,238,238)
                cadetblue2            = 0x8ee5eeff, // (142,229,238)
                grey56                = 0x8f8f8fff, // (143,143,143)
                darkseagreen          = 0x8fbc8fff, // (143,188,143)
                lightgreen            = 0x90ee90ff, // (144,238,144)
                palegreen2            = 0x90ee90ff, // (144,238,144)
                purple2               = 0x912ceeff, // (145, 44,238)
                grey57                = 0x919191ff, // (145,145,145)
                mediumpurple          = 0x9370dbff, // (147,112,219)
                darkviolet            = 0x9400d3ff, // (148,  0,211)
                grey58                = 0x949494ff, // (148,148,148)
                grey59                = 0x969696ff, // (150,150,150)
                paleturquoise3        = 0x96cdcdff, // (150,205,205)
                darkslategrey1        = 0x97ffffff, // (151,255,255)
                cadetblue1            = 0x98f5ffff, // (152,245,255)
                palegreen             = 0x98fb98ff, // (152,251,152)
                darkorchid            = 0x9932ccff, // (153, 50,204)
                grey60                = 0x999999ff, // (153,153,153)
                darkorchid3           = 0x9a32cdff, // (154, 50,205)
                lightblue3            = 0x9ac0cdff, // (154,192,205)
                olivedrab3            = 0x9acd32ff, // (154,205, 50)
                yellowgreen           = 0x9acd32ff, // (154,205, 50)
                palegreen1            = 0x9aff9aff, // (154,255,154)
                purple1               = 0x9b30ffff, // (155, 48,255)
                darkseagreen3         = 0x9bcd9bff, // (155,205,155)
                grey61                = 0x9c9c9cff, // (156,156,156)
                grey62                = 0x9e9e9eff, // (158,158,158)
                mediumpurple2         = 0x9f79eeff, // (159,121,238)
                slategrey3            = 0x9fb6cdff, // (159,182,205)
                purple                = 0xa020f0ff, // (160, 32,240)
                sienna                = 0xa0522dff, // (160, 82, 45)
                grey63                = 0xa1a1a1ff, // (161,161,161)
                lightsteelblue3       = 0xa2b5cdff, // (162,181,205)
                darkolivegreen3       = 0xa2cd5aff, // (162,205, 90)
                grey64                = 0xa3a3a3ff, // (163,163,163)
                lightskyblue2         = 0xa4d3eeff, // (164,211,238)
                brown                 = 0xa52a2aff, // (165, 42, 42)
                grey65                = 0xa6a6a6ff, // (166,166,166)
                grey66                = 0xa8a8a8ff, // (168,168,168)
                darkgrey              = 0xa9a9a9ff, // (169,169,169)
                mediumpurple1         = 0xab82ffff, // (171,130,255)
                grey67                = 0xabababff, // (171,171,171)
                grey68                = 0xadadadff, // (173,173,173)
                lightblue             = 0xadd8e6ff, // (173,216,230)
                greenyellow           = 0xadff2fff, // (173,255, 47)
                paleturquoise2        = 0xaeeeeeff, // (174,238,238)
                paleturquoise         = 0xafeeeeff, // (175,238,238)
                maroon                = 0xb03060ff, // (176, 48, 96)
                grey69                = 0xb0b0b0ff, // (176,176,176)
                lightsteelblue        = 0xb0c4deff, // (176,196,222)
                powderblue            = 0xb0e0e6ff, // (176,224,230)
                lightskyblue1         = 0xb0e2ffff, // (176,226,255)
                firebrick             = 0xb22222ff, // (178, 34, 34)
                darkorchid2           = 0xb23aeeff, // (178, 58,238)
                lightblue2            = 0xb2dfeeff, // (178,223,238)
                grey70                = 0xb3b3b3ff, // (179,179,179)
                olivedrab2            = 0xb3ee3aff, // (179,238, 58)
                mediumorchid3         = 0xb452cdff, // (180, 82,205)
                lightcyan3            = 0xb4cdcdff, // (180,205,205)
                darkseagreen2         = 0xb4eeb4ff, // (180,238,180)
                grey71                = 0xb5b5b5ff, // (181,181,181)
                darkgoldenrod         = 0xb8860bff, // (184,134, 11)
                mediumgoldenrod       = 0xb8860bff, // (184,134, 11)
                grey72                = 0xb8b8b8ff, // (184,184,184)
                slategrey2            = 0xb9d3eeff, // (185,211,238)
                mediumorchid          = 0xba55d3ff, // (186, 85,211)
                grey73                = 0xbababaff, // (186,186,186)
                paleturquoise1        = 0xbbffffff, // (187,255,255)
                rosybrown             = 0xbc8f8fff, // (188,143,143)
                lightsteelblue2       = 0xbcd2eeff, // (188,210,238)
                darkolivegreen2       = 0xbcee68ff, // (188,238,104)
                darkkhaki             = 0xbdb76bff, // (189,183,107)
                grey74                = 0xbdbdbdff, // (189,189,189)
                grey                  = 0xbebebeff, // (190,190,190)
                darkorchid1           = 0xbf3effff, // (191, 62,255)
                grey75                = 0xbfbfbfff, // (191,191,191)
                lightblue1            = 0xbfefffff, // (191,239,255)
                olivedrab1            = 0xc0ff3eff, // (192,255, 62)
                honeydew3             = 0xc1cdc1ff, // (193,205,193)
                azure3                = 0xc1cdcdff, // (193,205,205)
                darkseagreen1         = 0xc1ffc1ff, // (193,255,193)
                grey76                = 0xc2c2c2ff, // (194,194,194)
                grey77                = 0xc4c4c4ff, // (196,196,196)
                slategrey1            = 0xc6e2ffff, // (198,226,255)
                mediumvioletred       = 0xc71585ff, // (199, 21,133)
                grey78                = 0xc7c7c7ff, // (199,199,199)
                grey79                = 0xc9c9c9ff, // (201,201,201)
                lightsteelblue1       = 0xcae1ffff, // (202,225,255)
                darkolivegreen1       = 0xcaff70ff, // (202,255,112)
                grey80                = 0xccccccff, // (204,204,204)
                red3                  = 0xcd0000ff, // (205,  0,  0)
                magenta3              = 0xcd00cdff, // (205,  0,205)
                deeppink3             = 0xcd1076ff, // (205, 16,118)
                firebrick3            = 0xcd2626ff, // (205, 38, 38)
                maroon3               = 0xcd2990ff, // (205, 41,144)
                violetred3            = 0xcd3278ff, // (205, 50,120)
                brown3                = 0xcd3333ff, // (205, 51, 51)
                orangered3            = 0xcd3700ff, // (205, 55,  0)
                tomato3               = 0xcd4f39ff, // (205, 79, 57)
                indianred3            = 0xcd5555ff, // (205, 85, 85)
                coral3                = 0xcd5b45ff, // (205, 91, 69)
                indianred             = 0xcd5c5cff, // (205, 92, 92)
                hotpink3              = 0xcd6090ff, // (205, 96,144)
                mediumpink3           = 0xcd6490ff, // (205,100,144)
                darkorange3           = 0xcd6600ff, // (205,102,  0)
                chocolate3            = 0xcd661dff, // (205,102, 29)
                sienna3               = 0xcd6839ff, // (205,104, 57)
                palevioletred3        = 0xcd6889ff, // (205,104,137)
                orchid3               = 0xcd69c9ff, // (205,105,201)
                salmon3               = 0xcd7054ff, // (205,112, 84)
                lightsalmon3          = 0xcd8162ff, // (205,129, 98)
                orange3               = 0xcd8500ff, // (205,133,  0)
                peru                  = 0xcd853fff, // (205,133, 63)
                tan3                  = 0xcd853fff, // (205,133, 63)
                lightpink3            = 0xcd8c95ff, // (205,140,149)
                pink3                 = 0xcd919eff, // (205,145,158)
                darkgoldenrod3        = 0xcd950cff, // (205,149, 12)
                mediumgoldenrod3      = 0xcd950cff, // (205,149, 12)
                plum3                 = 0xcd96cdff, // (205,150,205)
                goldenrod3            = 0xcd9b1dff, // (205,155, 29)
                rosybrown3            = 0xcd9b9bff, // (205,155,155)
                burlywood3            = 0xcdaa7dff, // (205,170,125)
                gold3                 = 0xcdad00ff, // (205,173,  0)
                peachpuff3            = 0xcdaf95ff, // (205,175,149)
                navajowhite3          = 0xcdb38bff, // (205,179,139)
                thistle3              = 0xcdb5cdff, // (205,181,205)
                bisque3               = 0xcdb79eff, // (205,183,158)
                mistyrose3            = 0xcdb7b5ff, // (205,183,181)
                wheat3                = 0xcdba96ff, // (205,186,150)
                lightgoldenrod3       = 0xcdbe70ff, // (205,190,112)
                oldgoldenrod3         = 0xcdbe70ff, // (205,190,112)
                antiquewhite3         = 0xcdc0b0ff, // (205,192,176)
                lavenderblush3        = 0xcdc1c5ff, // (205,193,197)
                seashell3             = 0xcdc5bfff, // (205,197,191)
                khaki3                = 0xcdc673ff, // (205,198,115)
                cornsilk3             = 0xcdc8b1ff, // (205,200,177)
                lemonchiffon3         = 0xcdc9a5ff, // (205,201,165)
                snow3                 = 0xcdc9c9ff, // (205,201,201)
                yellow3               = 0xcdcd00ff, // (205,205,  0)
                oldmediumgoldenrod3   = 0xcdcd96ff, // (205,205,150)
                lightyellow3          = 0xcdcdb4ff, // (205,205,180)
                ivory3                = 0xcdcdc1ff, // (205,205,193)
                grey81                = 0xcfcfcfff, // (207,207,207)
                violetred             = 0xd02090ff, // (208, 32,144)
                mediumorchid2         = 0xd15feeff, // (209, 95,238)
                grey82                = 0xd1d1d1ff, // (209,209,209)
                lightcyan2            = 0xd1eeeeff, // (209,238,238)
                chocolate             = 0xd2691eff, // (210,105, 30)
                tan                   = 0xd2b48cff, // (210,180,140)
                lightgrey             = 0xd3d3d3ff, // (211,211,211)
                grey83                = 0xd4d4d4ff, // (212,212,212)
                grey84                = 0xd6d6d6ff, // (214,214,214)
                thistle               = 0xd8bfd8ff, // (216,191,216)
                grey85                = 0xd9d9d9ff, // (217,217,217)
                orchid                = 0xda70d6ff, // (218,112,214)
                goldenrod             = 0xdaa520ff, // (218,165, 32)
                palevioletred         = 0xdb7093ff, // (219,112,147)
                grey86                = 0xdbdbdbff, // (219,219,219)
                gainsboro             = 0xdcdcdcff, // (220,220,220)
                plum                  = 0xdda0ddff, // (221,160,221)
                burlywood             = 0xdeb887ff, // (222,184,135)
                grey87                = 0xdededeff, // (222,222,222)
                mediumorchid1         = 0xe066ffff, // (224,102,255)
                grey88                = 0xe0e0e0ff, // (224,224,224)
                honeydew2             = 0xe0eee0ff, // (224,238,224)
                azure2                = 0xe0eeeeff, // (224,238,238)
                lightcyan             = 0xe0ffffff, // (224,255,255)
                grey89                = 0xe3e3e3ff, // (227,227,227)
                grey90                = 0xe5e5e5ff, // (229,229,229)
                lavender              = 0xe6e6faff, // (230,230,250)
                grey91                = 0xe8e8e8ff, // (232,232,232)
                darksalmon            = 0xe9967aff, // (233,150,122)
                grey92                = 0xebebebff, // (235,235,235)
                grey93                = 0xedededff, // (237,237,237)
                red2                  = 0xee0000ff, // (238,  0,  0)
                magenta2              = 0xee00eeff, // (238,  0,238)
                deeppink2             = 0xee1289ff, // (238, 18,137)
                firebrick2            = 0xee2c2cff, // (238, 44, 44)
                maroon2               = 0xee30a7ff, // (238, 48,167)
                violetred2            = 0xee3a8cff, // (238, 58,140)
                brown2                = 0xee3b3bff, // (238, 59, 59)
                orangered2            = 0xee4000ff, // (238, 64,  0)
                tomato2               = 0xee5c42ff, // (238, 92, 66)
                indianred2            = 0xee6363ff, // (238, 99, 99)
                coral2                = 0xee6a50ff, // (238,106, 80)
                hotpink2              = 0xee6aa7ff, // (238,106,167)
                mediumpink2           = 0xee74a7ff, // (238,116,167)
                darkorange2           = 0xee7600ff, // (238,118,  0)
                chocolate2            = 0xee7621ff, // (238,118, 33)
                sienna2               = 0xee7942ff, // (238,121, 66)
                palevioletred2        = 0xee799fff, // (238,121,159)
                orchid2               = 0xee7ae9ff, // (238,122,233)
                salmon2               = 0xee8262ff, // (238,130, 98)
                violet                = 0xee82eeff, // (238,130,238)
                lightsalmon2          = 0xee9572ff, // (238,149,114)
                orange2               = 0xee9a00ff, // (238,154,  0)
                tan2                  = 0xee9a49ff, // (238,154, 73)
                lightpink2            = 0xeea2adff, // (238,162,173)
                pink2                 = 0xeea9b8ff, // (238,169,184)
                darkgoldenrod2        = 0xeead0eff, // (238,173, 14)
                mediumgoldenrod2      = 0xeead0eff, // (238,173, 14)
                plum2                 = 0xeeaeeeff, // (238,174,238)
                goldenrod2            = 0xeeb422ff, // (238,180, 34)
                rosybrown2            = 0xeeb4b4ff, // (238,180,180)
                burlywood2            = 0xeec591ff, // (238,197,145)
                gold2                 = 0xeec900ff, // (238,201,  0)
                peachpuff2            = 0xeecbadff, // (238,203,173)
                navajowhite2          = 0xeecfa1ff, // (238,207,161)
                thistle2              = 0xeed2eeff, // (238,210,238)
                bisque2               = 0xeed5b7ff, // (238,213,183)
                mistyrose2            = 0xeed5d2ff, // (238,213,210)
                wheat2                = 0xeed8aeff, // (238,216,174)
                lightgoldenrod2       = 0xeedc82ff, // (238,220,130)
                oldgoldenrod2         = 0xeedc82ff, // (238,220,130)
                lightgoldenrod        = 0xeedd82ff, // (238,221,130)
                oldgoldenrod          = 0xeedd82ff, // (238,221,130)
                antiquewhite2         = 0xeedfccff, // (238,223,204)
                lavenderblush2        = 0xeee0e5ff, // (238,224,229)
                seashell2             = 0xeee5deff, // (238,229,222)
                khaki2                = 0xeee685ff, // (238,230,133)
                palegoldenrod         = 0xeee8aaff, // (238,232,170)
                cornsilk2             = 0xeee8cdff, // (238,232,205)
                lemonchiffon2         = 0xeee9bfff, // (238,233,191)
                snow2                 = 0xeee9e9ff, // (238,233,233)
                yellow2               = 0xeeee00ff, // (238,238,  0)
                oldmediumgoldenrod2   = 0xeeeeaeff, // (238,238,174)
                oldmediumgoldenrod    = 0xeeeeafff, // (238,238,175)
                lightyellow2          = 0xeeeed1ff, // (238,238,209)
                ivory2                = 0xeeeee0ff, // (238,238,224)
                lightcoral            = 0xf08080ff, // (240,128,128)
                khaki                 = 0xf0e68cff, // (240,230,140)
                grey94                = 0xf0f0f0ff, // (240,240,240)
                aliceblue             = 0xf0f8ffff, // (240,248,255)
                honeydew              = 0xf0fff0ff, // (240,255,240)
                azure                 = 0xf0ffffff, // (240,255,255)
                grey95                = 0xf2f2f2ff, // (242,242,242)
                sandybrown            = 0xf4a460ff, // (244,164, 96)
                wheat                 = 0xf5deb3ff, // (245,222,179)
                beige                 = 0xf5f5dcff, // (245,245,220)
                grey96                = 0xf5f5f5ff, // (245,245,245)
                whitesmoke            = 0xf5f5f5ff, // (245,245,245)
                mintcream             = 0xf5fffaff, // (245,255,250)
                grey97                = 0xf7f7f7ff, // (247,247,247)
                ghostwhite            = 0xf8f8ffff, // (248,248,255)
                salmon                = 0xfa8072ff, // (250,128,114)
                antiquewhite          = 0xfaebd7ff, // (250,235,215)
                linen                 = 0xfaf0e6ff, // (250,240,230)
                lightgoldenrodyellow  = 0xfafad2ff, // (250,250,210)
                grey98                = 0xfafafaff, // (250,250,250)
                grey99                = 0xfcfcfcff, // (252,252,252)
                oldlace               = 0xfdf5e6ff, // (253,245,230)
                red                   = 0xff0000ff, // (255,  0,  0)
                magenta               = 0xff00ffff, // (255,  0,255)
                deeppink              = 0xff1493ff, // (255, 20,147)
                firebrick1            = 0xff3030ff, // (255, 48, 48)
                maroon1               = 0xff34b3ff, // (255, 52,179)
                violetred1            = 0xff3e96ff, // (255, 62,150)
                brown1                = 0xff4040ff, // (255, 64, 64)
                orangered             = 0xff4500ff, // (255, 69,  0)
                tomato                = 0xff6347ff, // (255, 99, 71)
                hotpink               = 0xff69b4ff, // (255,105,180)
                indianred1            = 0xff6a6aff, // (255,106,106)
                hotpink1              = 0xff6eb4ff, // (255,110,180)
                coral                 = 0xff7256ff, // (255,114, 86)
                mediumpink            = 0xff7db3ff, // (255,125,179)
                darkorange            = 0xff7f00ff, // (255,127,  0)
                chocolate1            = 0xff7f24ff, // (255,127, 36)
                coral1                = 0xff7f50ff, // (255,127, 80)
                sienna1               = 0xff8247ff, // (255,130, 71)
                palevioletred1        = 0xff82abff, // (255,130,171)
                orchid1               = 0xff83faff, // (255,131,250)
                darkorange1           = 0xff8c00ff, // (255,140,  0)
                salmon1               = 0xff8c69ff, // (255,140,105)
                lightsalmon           = 0xffa07aff, // (255,160,122)
                orange                = 0xffa500ff, // (255,165,  0)
                tan1                  = 0xffa54fff, // (255,165, 79)
                palepink              = 0xffaac8ff, // (255,170,200)
                lightpink1            = 0xffaeb9ff, // (255,174,185)
                pink1                 = 0xffb5c5ff, // (255,181,197)
                lightpink             = 0xffb6c1ff, // (255,182,193)
                darkgoldenrod1        = 0xffb90fff, // (255,185, 15)
                mediumgoldenrod1      = 0xffb90fff, // (255,185, 15)
                plum1                 = 0xffbbffff, // (255,187,255)
                pink                  = 0xffc0cbff, // (255,192,203)
                goldenrod1            = 0xffc125ff, // (255,193, 37)
                rosybrown1            = 0xffc1c1ff, // (255,193,193)
                burlywood1            = 0xffd39bff, // (255,211,155)
                gold                  = 0xffd700ff, // (255,215,  0)
                peachpuff             = 0xffdab9ff, // (255,218,185)
                navajowhite           = 0xffdeadff, // (255,222,173)
                thistle1              = 0xffe1ffff, // (255,225,255)
                moccasin              = 0xffe4b5ff, // (255,228,181)
                bisque                = 0xffe4c4ff, // (255,228,196)
                mistyrose             = 0xffe4e1ff, // (255,228,225)
                wheat1                = 0xffe7baff, // (255,231,186)
                blanchedalmond        = 0xffebcdff, // (255,235,205)
                lightgoldenrod1       = 0xffec8bff, // (255,236,139)
                oldgoldenrod1         = 0xffec8bff, // (255,236,139)
                papayawhip            = 0xffefd5ff, // (255,239,213)
                antiquewhite1         = 0xffefdbff, // (255,239,219)
                lavenderblush         = 0xfff0f5ff, // (255,240,245)
                seashell              = 0xfff5eeff, // (255,245,238)
                khaki1                = 0xfff68fff, // (255,246,143)
                cornsilk              = 0xfff8dcff, // (255,248,220)
                lemonchiffon          = 0xfffacdff, // (255,250,205)
                floralwhite           = 0xfffaf0ff, // (255,250,240)
                snow                  = 0xfffafaff, // (255,250,250)
                yellow                = 0xffff00ff, // (255,255,  0)
                oldmediumgoldenrod1   = 0xffffbbff, // (255,255,187)
                lightyellow           = 0xffffe0ff, // (255,255,224)
                ivory                 = 0xfffff0ff, // (255,255,240)
                grey100               = 0xffffffff, // (255,255,255)
                white                 = 0xffffffff, // (255,255,255)
                Count = 543
            };

           static inline const std::vector<sf::Color> Colors{
                sf::Color(black),
                sf::Color(grey0),
                sf::Color(navy),
                sf::Color(blue4),
                sf::Color(darkblue),
                sf::Color(blue3),
                sf::Color(mediumblue),
                sf::Color(blue2),
                sf::Color(blue),
                sf::Color(darkgreen),
                sf::Color(darkgreen1),
                sf::Color(deepskyblue4),
                sf::Color(turquoise4),
                sf::Color(green4),
                sf::Color(springgreen4),
                sf::Color(cyan4),
                sf::Color(darkcyan),
                sf::Color(deepskyblue3),
                sf::Color(deepskyblue2),
                sf::Color(deepskyblue),
                sf::Color(darkturquoise1),
                sf::Color(turquoise3),
                sf::Color(green3),
                sf::Color(springgreen3),
                sf::Color(cyan3),
                sf::Color(darkturquoise),
                sf::Color(mediumturquoise2),
                sf::Color(turquoise2),
                sf::Color(green2),
                sf::Color(springgreen2),
                sf::Color(cyan2),
                sf::Color(turquoise1),
                sf::Color(mediumspringgreen),
                sf::Color(green),
                sf::Color(springgreen),
                sf::Color(cyan),
                sf::Color(grey1),
                sf::Color(grey2),
                sf::Color(grey3),
                sf::Color(grey4),
                sf::Color(grey5),
                sf::Color(grey6),
                sf::Color(dodgerblue4),
                sf::Color(grey7),
                sf::Color(grey8),
                sf::Color(grey9),
                sf::Color(dodgerblue3),
                sf::Color(midnightblue1),
                sf::Color(midnightblue),
                sf::Color(grey10),
                sf::Color(grey11),
                sf::Color(dodgerblue2),
                sf::Color(dodgerblue),
                sf::Color(grey12),
                sf::Color(lightseagreen),
                sf::Color(grey13),
                sf::Color(cornflowerblue),
                sf::Color(forestgreen),
                sf::Color(grey14),
                sf::Color(grey15),
                sf::Color(royalblue4),
                sf::Color(grey16),
                sf::Color(grey17),
                sf::Color(grey18),
                sf::Color(seagreen),
                sf::Color(darkslategrey),
                sf::Color(grey19),
                sf::Color(limegreen),
                sf::Color(grey20),
                sf::Color(grey21),
                sf::Color(steelblue4),
                sf::Color(grey22),
                sf::Color(royalblue3),
                sf::Color(grey23),
                sf::Color(mediumseagreen),
                sf::Color(grey24),
                sf::Color(grey25),
                sf::Color(turquoise),
                sf::Color(royalblue),
                sf::Color(grey26),
                sf::Color(royalblue2),
                sf::Color(seagreen3),
                sf::Color(grey27),
                sf::Color(chartreuse4),
                sf::Color(aquamarine4),
                sf::Color(steelblue),
                sf::Color(slateblue4),
                sf::Color(grey28),
                sf::Color(darkslateblue),
                sf::Color(royalblue1),
                sf::Color(mediumturquoise),
                sf::Color(grey29),
                sf::Color(skyblue4),
                sf::Color(grey30),
                sf::Color(seagreen2),
                sf::Color(grey31),
                sf::Color(steelblue3),
                sf::Color(grey32),
                sf::Color(darkslategrey4),
                sf::Color(cadetblue4),
                sf::Color(grey33),
                sf::Color(palegreen4),
                sf::Color(seagreen1),
                sf::Color(purple4),
                sf::Color(darkolivegreen),
                sf::Color(grey34),
                sf::Color(grey35),
                sf::Color(grey36),
                sf::Color(steelblue2),
                sf::Color(mediumpurple4),
                sf::Color(grey37),
                sf::Color(cadetblue),
                sf::Color(lightskyblue4),
                sf::Color(grey38),
                sf::Color(grey39),
                sf::Color(steelblue1),
                sf::Color(cornflowerblue1),
                sf::Color(grey40),
                sf::Color(paleturquoise4),
                sf::Color(chartreuse3),
                sf::Color(aquamarine3),
                sf::Color(mediumaquamarine),
                sf::Color(darkorchid4),
                sf::Color(lightblue4),
                sf::Color(slateblue3),
                sf::Color(dimgrey),
                sf::Color(grey41),
                sf::Color(olivedrab4),
                sf::Color(darkseagreen4),
                sf::Color(slateblue),
                sf::Color(grey42),
                sf::Color(mediumforestgreen),
                sf::Color(olivedrab),
                sf::Color(slategrey4),
                sf::Color(skyblue3),
                sf::Color(grey43),
                sf::Color(lightsteelblue4),
                sf::Color(darkolivegreen4),
                sf::Color(grey44),
                sf::Color(slategrey),
                sf::Color(grey45),
                sf::Color(grey46),
                sf::Color(chartreuse2),
                sf::Color(aquamarine2),
                sf::Color(lightslategrey),
                sf::Color(grey47),
                sf::Color(darkslategrey3),
                sf::Color(mediumorchid4),
                sf::Color(slateblue2),
                sf::Color(grey48),
                sf::Color(lightcyan4),
                sf::Color(cadetblue3),
                sf::Color(mediumslateblue),
                sf::Color(palegreen3),
                sf::Color(lawngreen),
                sf::Color(purple3),
                sf::Color(grey49),
                sf::Color(skyblue2),
                sf::Color(grey50),
                sf::Color(chartreuse),
                sf::Color(mediumspringgreen2),
                sf::Color(aquamarine),
                sf::Color(grey51),
                sf::Color(slateblue1),
                sf::Color(honeydew4),
                sf::Color(azure4),
                sf::Color(lightslateblue),
                sf::Color(grey52),
                sf::Color(grey53),
                sf::Color(skyblue),
                sf::Color(lightskyblue),
                sf::Color(skyblue1),
                sf::Color(mediumpurple3),
                sf::Color(blueviolet),
                sf::Color(purple5),
                sf::Color(grey54),
                sf::Color(darkred),
                sf::Color(red4),
                sf::Color(darkmagenta),
                sf::Color(magenta4),
                sf::Color(deeppink4),
                sf::Color(firebrick4),
                sf::Color(maroon4),
                sf::Color(violetred4),
                sf::Color(brown4),
                sf::Color(orangered4),
                sf::Color(tomato4),
                sf::Color(hotpink4),
                sf::Color(coral4),
                sf::Color(mediumpink4),
                sf::Color(darkorange4),
                sf::Color(chocolate4),
                sf::Color(saddlebrown),
                sf::Color(sienna4),
                sf::Color(palevioletred4),
                sf::Color(orchid4),
                sf::Color(salmon4),
                sf::Color(lightsalmon4),
                sf::Color(orange4),
                sf::Color(tan4),
                sf::Color(lightpink4),
                sf::Color(pink4),
                sf::Color(darkgoldenrod4),
                sf::Color(mediumgoldenrod4),
                sf::Color(plum4),
                sf::Color(goldenrod4),
                sf::Color(rosybrown4),
                sf::Color(burlywood4),
                sf::Color(gold4),
                sf::Color(peachpuff4),
                sf::Color(navajowhite4),
                sf::Color(thistle4),
                sf::Color(bisque4),
                sf::Color(mistyrose4),
                sf::Color(wheat4),
                sf::Color(lightgoldenrod4),
                sf::Color(oldgoldenrod4),
                sf::Color(antiquewhite4),
                sf::Color(lavenderblush4),
                sf::Color(khaki4),
                sf::Color(seashell4),
                sf::Color(cornsilk4),
                sf::Color(lemonchiffon4),
                sf::Color(snow4),
                sf::Color(yellow4),
                sf::Color(oldmediumgoldenrod4),
                sf::Color(lightyellow4),
                sf::Color(ivory4),
                sf::Color(grey55),
                sf::Color(lightskyblue3),
                sf::Color(darkslategrey2),
                sf::Color(cadetblue2),
                sf::Color(grey56),
                sf::Color(darkseagreen),
                sf::Color(lightgreen),
                sf::Color(palegreen2),
                sf::Color(purple2),
                sf::Color(grey57),
                sf::Color(mediumpurple),
                sf::Color(darkviolet),
                sf::Color(grey58),
                sf::Color(grey59),
                sf::Color(paleturquoise3),
                sf::Color(darkslategrey1),
                sf::Color(cadetblue1),
                sf::Color(palegreen),
                sf::Color(darkorchid),
                sf::Color(grey60),
                sf::Color(darkorchid3),
                sf::Color(lightblue3),
                sf::Color(olivedrab3),
                sf::Color(yellowgreen),
                sf::Color(palegreen1),
                sf::Color(purple1),
                sf::Color(darkseagreen3),
                sf::Color(grey61),
                sf::Color(grey62),
                sf::Color(mediumpurple2),
                sf::Color(slategrey3),
                sf::Color(purple),
                sf::Color(sienna),
                sf::Color(grey63),
                sf::Color(lightsteelblue3),
                sf::Color(darkolivegreen3),
                sf::Color(grey64),
                sf::Color(lightskyblue2),
                sf::Color(brown),
                sf::Color(grey65),
                sf::Color(grey66),
                sf::Color(darkgrey),
                sf::Color(mediumpurple1),
                sf::Color(grey67),
                sf::Color(grey68),
                sf::Color(lightblue),
                sf::Color(greenyellow),
                sf::Color(paleturquoise2),
                sf::Color(paleturquoise),
                sf::Color(maroon),
                sf::Color(grey69),
                sf::Color(lightsteelblue),
                sf::Color(powderblue),
                sf::Color(lightskyblue1),
                sf::Color(firebrick),
                sf::Color(darkorchid2),
                sf::Color(lightblue2),
                sf::Color(grey70),
                sf::Color(olivedrab2),
                sf::Color(mediumorchid3),
                sf::Color(lightcyan3),
                sf::Color(darkseagreen2),
                sf::Color(grey71),
                sf::Color(darkgoldenrod),
                sf::Color(mediumgoldenrod),
                sf::Color(grey72),
                sf::Color(slategrey2),
                sf::Color(mediumorchid),
                sf::Color(grey73),
                sf::Color(paleturquoise1),
                sf::Color(rosybrown),
                sf::Color(lightsteelblue2),
                sf::Color(darkolivegreen2),
                sf::Color(darkkhaki),
                sf::Color(grey74),
                sf::Color(grey),
                sf::Color(darkorchid1),
                sf::Color(grey75),
                sf::Color(lightblue1),
                sf::Color(olivedrab1),
                sf::Color(honeydew3),
                sf::Color(azure3),
                sf::Color(darkseagreen1),
                sf::Color(grey76),
                sf::Color(grey77),
                sf::Color(slategrey1),
                sf::Color(mediumvioletred),
                sf::Color(grey78),
                sf::Color(grey79),
                sf::Color(lightsteelblue1),
                sf::Color(darkolivegreen1),
                sf::Color(grey80),
                sf::Color(red3),
                sf::Color(magenta3),
                sf::Color(deeppink3),
                sf::Color(firebrick3),
                sf::Color(maroon3),
                sf::Color(violetred3),
                sf::Color(brown3),
                sf::Color(orangered3),
                sf::Color(tomato3),
                sf::Color(indianred3),
                sf::Color(coral3),
                sf::Color(indianred),
                sf::Color(hotpink3),
                sf::Color(mediumpink3),
                sf::Color(darkorange3),
                sf::Color(chocolate3),
                sf::Color(sienna3),
                sf::Color(palevioletred3),
                sf::Color(orchid3),
                sf::Color(salmon3),
                sf::Color(lightsalmon3),
                sf::Color(orange3),
                sf::Color(peru),
                sf::Color(tan3),
                sf::Color(lightpink3),
                sf::Color(pink3),
                sf::Color(darkgoldenrod3),
                sf::Color(mediumgoldenrod3),
                sf::Color(plum3),
                sf::Color(goldenrod3),
                sf::Color(rosybrown3),
                sf::Color(burlywood3),
                sf::Color(gold3),
                sf::Color(peachpuff3),
                sf::Color(navajowhite3),
                sf::Color(thistle3),
                sf::Color(bisque3),
                sf::Color(mistyrose3),
                sf::Color(wheat3),
                sf::Color(lightgoldenrod3),
                sf::Color(oldgoldenrod3),
                sf::Color(antiquewhite3),
                sf::Color(lavenderblush3),
                sf::Color(seashell3),
                sf::Color(khaki3),
                sf::Color(cornsilk3),
                sf::Color(lemonchiffon3),
                sf::Color(snow3),
                sf::Color(yellow3),
                sf::Color(oldmediumgoldenrod3),
                sf::Color(lightyellow3),
                sf::Color(ivory3),
                sf::Color(grey81),
                sf::Color(violetred),
                sf::Color(mediumorchid2),
                sf::Color(grey82),
                sf::Color(lightcyan2),
                sf::Color(chocolate),
                sf::Color(tan),
                sf::Color(lightgrey),
                sf::Color(grey83),
                sf::Color(grey84),
                sf::Color(thistle),
                sf::Color(grey85),
                sf::Color(orchid),
                sf::Color(goldenrod),
                sf::Color(palevioletred),
                sf::Color(grey86),
                sf::Color(gainsboro),
                sf::Color(plum),
                sf::Color(burlywood),
                sf::Color(grey87),
                sf::Color(mediumorchid1),
                sf::Color(grey88),
                sf::Color(honeydew2),
                sf::Color(azure2),
                sf::Color(lightcyan),
                sf::Color(grey89),
                sf::Color(grey90),
                sf::Color(lavender),
                sf::Color(grey91),
                sf::Color(darksalmon),
                sf::Color(grey92),
                sf::Color(grey93),
                sf::Color(red2),
                sf::Color(magenta2),
                sf::Color(deeppink2),
                sf::Color(firebrick2),
                sf::Color(maroon2),
                sf::Color(violetred2),
                sf::Color(brown2),
                sf::Color(orangered2),
                sf::Color(tomato2),
                sf::Color(indianred2),
                sf::Color(coral2),
                sf::Color(hotpink2),
                sf::Color(mediumpink2),
                sf::Color(darkorange2),
                sf::Color(chocolate2),
                sf::Color(sienna2),
                sf::Color(palevioletred2),
                sf::Color(orchid2),
                sf::Color(salmon2),
                sf::Color(violet),
                sf::Color(lightsalmon2),
                sf::Color(orange2),
                sf::Color(tan2),
                sf::Color(lightpink2),
                sf::Color(pink2),
                sf::Color(darkgoldenrod2),
                sf::Color(mediumgoldenrod2),
                sf::Color(plum2),
                sf::Color(goldenrod2),
                sf::Color(rosybrown2),
                sf::Color(burlywood2),
                sf::Color(gold2),
                sf::Color(peachpuff2),
                sf::Color(navajowhite2),
                sf::Color(thistle2),
                sf::Color(bisque2),
                sf::Color(mistyrose2),
                sf::Color(wheat2),
                sf::Color(lightgoldenrod2),
                sf::Color(oldgoldenrod2),
                sf::Color(lightgoldenrod),
                sf::Color(oldgoldenrod),
                sf::Color(antiquewhite2),
                sf::Color(lavenderblush2),
                sf::Color(seashell2),
                sf::Color(khaki2),
                sf::Color(palegoldenrod),
                sf::Color(cornsilk2),
                sf::Color(lemonchiffon2),
                sf::Color(snow2),
                sf::Color(yellow2),
                sf::Color(oldmediumgoldenrod2),
                sf::Color(oldmediumgoldenrod),
                sf::Color(lightyellow2),
                sf::Color(ivory2),
                sf::Color(lightcoral),
                sf::Color(khaki),
                sf::Color(grey94),
                sf::Color(aliceblue),
                sf::Color(honeydew),
                sf::Color(azure),
                sf::Color(grey95),
                sf::Color(sandybrown),
                sf::Color(wheat),
                sf::Color(beige),
                sf::Color(grey96),
                sf::Color(whitesmoke),
                sf::Color(mintcream),
                sf::Color(grey97),
                sf::Color(ghostwhite),
                sf::Color(salmon),
                sf::Color(antiquewhite),
                sf::Color(linen),
                sf::Color(lightgoldenrodyellow),
                sf::Color(grey98),
                sf::Color(grey99),
                sf::Color(oldlace),
                sf::Color(red),
                sf::Color(magenta),
                sf::Color(deeppink),
                sf::Color(firebrick1),
                sf::Color(maroon1),
                sf::Color(violetred1),
                sf::Color(brown1),
                sf::Color(orangered),
                sf::Color(tomato),
                sf::Color(hotpink),
                sf::Color(indianred1),
                sf::Color(hotpink1),
                sf::Color(coral),
                sf::Color(mediumpink),
                sf::Color(darkorange),
                sf::Color(chocolate1),
                sf::Color(coral1),
                sf::Color(sienna1),
                sf::Color(palevioletred1),
                sf::Color(orchid1),
                sf::Color(darkorange1),
                sf::Color(salmon1),
                sf::Color(lightsalmon),
                sf::Color(orange),
                sf::Color(tan1),
                sf::Color(palepink),
                sf::Color(lightpink1),
                sf::Color(pink1),
                sf::Color(lightpink),
                sf::Color(darkgoldenrod1),
                sf::Color(mediumgoldenrod1),
                sf::Color(plum1),
                sf::Color(pink),
                sf::Color(goldenrod1),
                sf::Color(rosybrown1),
                sf::Color(burlywood1),
                sf::Color(gold),
                sf::Color(peachpuff),
                sf::Color(navajowhite),
                sf::Color(thistle1),
                sf::Color(moccasin),
                sf::Color(bisque),
                sf::Color(mistyrose),
                sf::Color(wheat1),
                sf::Color(blanchedalmond),
                sf::Color(lightgoldenrod1),
                sf::Color(oldgoldenrod1),
                sf::Color(papayawhip),
                sf::Color(antiquewhite1),
                sf::Color(lavenderblush),
                sf::Color(seashell),
                sf::Color(khaki1),
                sf::Color(cornsilk),
                sf::Color(lemonchiffon),
                sf::Color(floralwhite),
                sf::Color(snow),
                sf::Color(yellow),
                sf::Color(oldmediumgoldenrod1),
                sf::Color(lightyellow),
                sf::Color(ivory),
                sf::Color(grey100),
                sf::Color(white)
            };
            // clang-format on

            static inline const std::string palleteName { "x11_543" };
        };

        // 949 colors from a massive study of NAMES CHOSEN BY PEOPLE (www.xkcd.com/color)
        struct xkcd_949
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
                white                      = 0xffffffff, // (255, 255, 255)
                Count = 949
            };

            static inline const std::vector<sf::Color> Colors {
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
            // clang-format on

            static inline const std::string palleteName { "xkcd_949" };
        };
    } // namespace pallete
} // namespace color

#endif // SFUTIL_COLOR_NAMES_HPP_INCLUDED
